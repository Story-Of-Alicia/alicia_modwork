//
// Created by maros on 22/07/2021.
//

#include "debugger.hpp"

#include <cstdint>
#include <climits>

#include <sstream>

#include <spdlog/spdlog.h>
#include <fmt/chrono.h>

#include <TlHelp32.h>
#include <Psapi.h>
#include <atlbase.h>

#include "main/interface/interface.hpp"


uint32_t dbg::MemoryController::readMemory(HANDLE procHandle, const uint64_t addr, buffer::stack_buffer &buffer) {
    if (!ReadProcessMemory(procHandle, (LPCVOID) addr, (LPVOID) buffer.dataPtr(),
                           (SIZE_T) buffer.getSize(), nullptr)) {
        return GetLastError();
    }
    return 0;
}

uint32_t dbg::MemoryController::writeMemory(HANDLE procHandle, const uint64_t addr, const buffer::stack_buffer &buffer) {
    if (!WriteProcessMemory(procHandle, (LPVOID) addr, (LPCVOID) buffer.dataPtr(),
                            (SIZE_T) buffer.getSize(), nullptr)) {
        return GetLastError();
    }
    return 0;
}


dbg::DebuggingTool::~DebuggingTool() {
    detach();
}


void dbg::DebuggingTool::winapi_debugErrors(const char *action) {
    const DWORD error = GetLastError();
    spdlog::error("WINAPI: '{}' caused error: {}(0x{:x})", action, (int64_t) error, (int64_t) error);
}

bool dbg::DebuggingTool::winapi_inquiryProcessID() {
    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, (DWORD) NULL);
    if (Process32Next(snapshot, &process) == TRUE)
        do {
            if (!strcmp(process.szExeFile, this->m_processName.c_str())) {
                this->m_processId = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process) == TRUE);
    CloseHandle(snapshot);

    if (this->m_processId == 0x0)
        return false;
    return true;
}

bool dbg::DebuggingTool::winapi_inquiryProcessHandle() {
    this->m_processHandle = OpenProcess(
            PROCESS_ALL_ACCESS,
            false,
            this->m_processId
    );

    if (!this->m_processHandle) {
        winapi_debugErrors("Opening process (Try running as administrator?)");
        return false;
    }

    HMODULE *pModules;
    uint32_t moduleCount;

    ::EnumProcessModulesEx(this->m_processHandle, nullptr, 0, (LPDWORD) &moduleCount, LIST_MODULES_ALL);
    spdlog::debug("Module count: {}", moduleCount);

    return true;
}


void dbg::DebuggingTool::initializeDebugger() {
    if (this->m_debuggingThread != nullptr)
        return;
    this->m_shouldDebug = true;

    this->m_debuggingThread = std::make_unique<std::thread>([this]() {

        if (!DebugActiveProcess(this->m_processId)) {
            DebuggingTool::winapi_debugErrors("Initializing process debug");
            return;
        } else {
            spdlog::info("Debugging thread initialized.");
        }
        killProcessOnDebuggerExit(false);

        while (this->m_shouldDebug) {
            this->m_isDebugging = true;

            DEBUG_EVENT event = {};
            ZeroMemory(&event, sizeof(event));

            WaitForDebugEvent(&event, 1000);
            if (event.dwDebugEventCode == 0)
                continue; // skip

            std::string eventlog;
            switch (event.dwDebugEventCode) {
                case EXCEPTION_DEBUG_EVENT: {
                    const auto info = event.u.Exception;
                    const auto exception = info.ExceptionRecord;

                    eventlog = fmt::format("[{:%H:%M:%S}] Exception 0x{:x} caught on address 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           (uint64_t) exception.ExceptionCode,
                                           (uint64_t) exception.ExceptionAddress);
                    break;
                }
                case CREATE_THREAD_DEBUG_EVENT: {
                    const auto info = event.u.CreateThread;

                    eventlog = fmt::format("[{:%H:%M:%S}] Create thread 0x{:x} on address 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           (uint64_t) info.hThread,
                                           (uint64_t) info.lpStartAddress);
                    break;
                }

                case EXIT_THREAD_DEBUG_EVENT: {
                    const auto info = event.u.CreateThread;

                    eventlog = fmt::format("[{:%H:%M:%S}] Destroy thread 0x{:x} on address 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           (uint64_t) info.hThread,
                                           (uint64_t) info.lpStartAddress);
                    break;
                }


                case CREATE_PROCESS_DEBUG_EVENT: {
                    USES_CONVERSION;
                    const auto info = event.u.CreateProcessInfo;
                    uint8_t buf[255];
                    ReadProcessMemory(this->m_processHandle, info.lpImageName, buf, 255, 0);

                    eventlog = fmt::format("[{:%H:%M:%S}] Create process {}'{}'",
                                           fmt::localtime(std::time(nullptr)),
                                           info.fUnicode ? "L" : "",
                                           info.fUnicode ? W2A((wchar_t *) buf) : (char*) buf
                                           );
                    break;
                }

                case EXIT_PROCESS_DEBUG_EVENT: {
                    const auto info = event.u.ExitProcess;
                    eventlog = fmt::format("[{:%H:%M:%S}] Destroy process with code 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           (uint64_t) info.dwExitCode);
                    break;
                }


                case LOAD_DLL_DEBUG_EVENT: {
                    USES_CONVERSION;

                    const auto info = event.u.LoadDll;
                    uint8_t buf[512];
                    if(info.lpImageName != nullptr)
                        ReadProcessMemory(this->m_processHandle, info.lpImageName, buf, 255, 0);

                    eventlog = fmt::format("[{:%H:%M:%S}] Load DLL {}'{}' on address 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           info.fUnicode && info.lpImageName != nullptr ? "L" : "",
                                           info.lpImageName != nullptr ? info.fUnicode ? W2A((wchar_t *) buf) : (char*) buf : "null",
                                           (uint64_t) info.lpBaseOfDll);
                    break;
                }

                case UNLOAD_DLL_DEBUG_EVENT: {
                    const auto info = event.u.UnloadDll;

                    eventlog = fmt::format("[{:%H:%M:%S}] Unload DLL on address 0x{:x}",
                                           fmt::localtime(std::time(nullptr)),
                                           (uint64_t) info.lpBaseOfDll);
                    break;
                }


                case OUTPUT_DEBUG_STRING_EVENT: {
                    const auto info = event.u.DebugString;
                    char *msg = new char[info.nDebugStringLength];

                    this->m_memoryPtr = (uint64_t) info.lpDebugStringData;
                    ReadProcessMemory(this->m_processHandle,
                                      info.lpDebugStringData,
                                      msg,
                                      info.nDebugStringLength,
                                      nullptr);

                    eventlog = fmt::format("[{:%H:%M:%S}] {}",
                                           fmt::localtime(std::time(nullptr)),
                                           msg);
                    delete[] msg;
                    break;
                }

            }
            this->m_eventLog.emplace_back(eventlog);
            ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
        }

        this->m_isDebugging = false;

        if (!DebugActiveProcessStop(this->m_processId)) {
            this->winapi_debugErrors("Terminating process debug");
        } else
            spdlog::info("Debugging thread terminated.");
    });

}

void dbg::DebuggingTool::terminateDebugger() {
    this->m_shouldDebug = false;
    if (this->m_debuggingThread != nullptr)
        this->m_debuggingThread->join();
    this->m_debuggingThread.reset();
    this->m_ui_debug = false;
}

bool dbg::DebuggingTool::isDebugging() const {
    return this->m_isDebugging;
}


void dbg::DebuggingTool::initializeAutoAttacher() {
    if (this->m_autoAttachingThread != nullptr || this->m_shouldAutoAttach)
        return;
    this->m_shouldAutoAttach = true;


    this->m_autoAttachingThread = std::make_unique<std::thread>([this]() {
        while (this->m_shouldAutoAttach) {
            while (!this->isAttached() &&
                   this->m_shouldAutoAttach) {
                this->attach();
                std::this_thread::sleep_for(std::chrono::milliseconds(this->m_autoAttachDelayMs));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(this->m_autoAttachDelayMs));
        }
    });
}

void dbg::DebuggingTool::terminateAutoAttacher() {
    this->m_shouldAutoAttach = false;
    if (this->m_autoAttachingThread != nullptr)
        this->m_autoAttachingThread->join();
    this->m_autoAttachingThread.reset();
    this->m_ui_autoAttach = false;
}

void dbg::DebuggingTool::setProcessName(std::string &name) {
    this->m_processName = name;
}


bool dbg::DebuggingTool::attach() {
    if (this->m_processName.empty())
        return false;

    if (!this->winapi_inquiryProcessID())
        return false;

    if (!this->winapi_inquiryProcessHandle())
        return false;
    this->m_isAttached = true;

    if (this->m_ui_debug)
        this->initializeDebugger();

    this->memory.m_processHandle = &this->m_processHandle;
    return true;
}

void dbg::DebuggingTool::detach() {
    this->m_isAttached = false;

    if (this->m_shouldAutoAttach)
        terminateAutoAttacher();

    if (this->m_isDebugging)
        this->terminateDebugger();

    if (this->m_processHandle != nullptr)
        CloseHandle(this->m_processHandle);
}


bool dbg::DebuggingTool::isAttached() {
    return this->m_isAttached;
}

void dbg::DebuggingTool::killProcessOnDebuggerExit(bool val) {
    if (!DebugSetProcessKillOnExit(false)) {
        const DWORD error = GetLastError();
        spdlog::error("DebugSetProcessKillOnExit caused error: {}(0x{:x})", (int64_t) error, (int64_t) error);
    }
}

uint32_t dbg::DebuggingTool::readMemory(uint64_t addr, buffer::stack_buffer &buffer) {
    this->m_memoryPtr = addr;
    return this->memory.readMemory(this->m_processHandle, addr, buffer);
}

uint32_t dbg::DebuggingTool::writeMemory(uint64_t addr, const buffer::stack_buffer &buffer) {
    this->m_memoryPtr = addr;
    return this->memory.writeMemory(this->m_processHandle, addr, buffer);
}

void dbg::DebuggingTool::renderInterface() {
    ImGui::Begin("Debugger Tool");
    ImGui::SetWindowSize({220, 300}, ImGuiCond_FirstUseEver);

    // ATTACHER
    {
        if (ImGui::CollapsingHeader("Attacher", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {

            bool resetStyle = false;
            if (this->m_shouldAutoAttach && !this->isAttached()) {
                ImGui::PushStyleColor(ImGuiCol_Button, {0.1, 0.1, 0.1, 0.5});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.1, 0.1, 0.1, 0.5});
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.1, 0.1, 0.1, 0.5});
                resetStyle = true;
            }

            if (ImGui::Button(this->isAttached() ? "Manual Detach" : "Manual Attach", {150, 30})) {
                if (!this->m_shouldAutoAttach && !this->m_isAttached)
                    attach();
                else
                    detach();
            }

            if (resetStyle) {
                ImGui::PopStyleColor(3);
            }

            if (ImGui::Checkbox("Auto attach", &this->m_ui_autoAttach)) {
                if (!this->isAttached() && this->m_ui_autoAttach)
                    this->initializeAutoAttacher();
                else if (this->isAttached() && !this->m_ui_autoAttach)
                    this->terminateAutoAttacher();
            }

            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Attached:       ");
            ImGui::SameLine();
            if (!this->m_isAttached)
                ImGui::TextColored({0.9, 0.5, 0.5, 1}, "FALSE");
            else {
                ImGui::TextColored({0.5, 0.8, 0.5, 1}, "TRUE");
            }

            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Auto-Attacher:  ");
            ImGui::SameLine();
            if (!this->m_shouldAutoAttach && !this->isAttached())
                ImGui::TextColored({0.9, 0.5, 0.5, 1}, "INACTIVE");
            else if (this->isAttached()) {
                ImGui::TextColored({0.4, 0.4, 0.4, 1}, "ATTACHED");
            } else {
                ImGui::TextColored({0.5, 0.8, 0.5, 1}, "ACTIVE");
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Process Name:   ");
            ImGui::SameLine();
            if (!this->m_processName.empty())
                ImGui::TextColored({0.6, 0.6, 0.6, 1}, "%s", this->m_processName.c_str());
            else
                ImGui::TextColored({0.3, 0.3, 0.3, 1}, "---");

            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Process ID:     ");
            ImGui::SameLine();
            if (this->isAttached())
                ImGui::TextColored({0.6, 0.6, 0.6, 1}, "0x%x", this->m_processId);
            else
                ImGui::TextColored({0.3, 0.3, 0.3, 1}, "---");
            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Process Handle: ");
            ImGui::SameLine();
            if (this->isAttached())
                ImGui::TextColored({0.6, 0.6, 0.6, 1}, "0x%llx", (uint64_t) this->m_processHandle);
            else
                ImGui::TextColored({0.3, 0.3, 0.3, 1}, "---");
        }
    }


    // DEBUGGER
    {
        if (ImGui::CollapsingHeader("Debugger", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Use debugger", &this->m_ui_debug)) {
                if (this->isAttached() && !this->m_ui_debug && this->isDebugging())
                    this->terminateDebugger();
                else if (this->isAttached() && this->m_ui_debug && !this->isDebugging())
                    this->initializeDebugger();
            }
            ImGui::Spacing();
            ImGui::Checkbox("Show output", &this->m_ui_showOutput);
            ImGui::Checkbox("Show memory", &this->m_ui_showMemory);
            ImGui::Spacing();
            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Debugger: ");

            ImGui::SameLine();
            if (!this->m_isDebugging)
                ImGui::TextColored({0.9, 0.5, 0.5, 1}, "INACTIVE");
            else {
                ImGui::TextColored({0.5, 0.8, 0.5, 1}, "ACTIVE");
            }
        }

    }
    ImVec2 nextWindowPos{ImGui::GetWindowPos().x + 220, ImGui::GetWindowPos().y};
    ImGui::End();


    if (this->m_ui_showOutput) {
        ImGui::SetNextWindowPos(nextWindowPos, this->m_ui_outputLockPlace ? 0 : ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({800, 300}, ImGuiCond_FirstUseEver);

        ImGui::Begin(
                fmt::format("(Debugger Tool) Output - {}", this->m_processName).c_str(),
                nullptr,
                (this->m_ui_outputLockPlace ? ImGuiWindowFlags_NoCollapse : 0)
        );
        {
            if (ImGui::CollapsingHeader("Output options")) {
                ImGui::Checkbox("Lock Window", &this->m_ui_outputLockPlace);
                ImGui::SameLine();
                ImGui::Checkbox("Enable auto-scroll", &this->m_ui_outputAutoScroll);
            }
            if (ImGui::CollapsingHeader("Debugger options")) {
                ImGui::Checkbox("Lock Window", &this->m_ui_outputLockPlace);
            }

            ImGui::BeginChild("Output");
            for (const auto &dbgStr : this->m_eventLog)
                ImGui::Text("%s", dbgStr.c_str());
            if (this->m_ui_outputAutoScroll)
                ImGui::SetScrollY(ImGui::GetScrollMaxY());
            ImGui::EndChild();
        }
        ImGui::End();
    }

    nextWindowPos.y+=300;
    if(this->m_ui_showMemory) {
        ImGui::SetNextWindowSize({800, 300}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(nextWindowPos);

        if (ImGui::Begin(fmt::format("(Debugger Tool) Memory - {}", this->m_processName).c_str(), nullptr,
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        )) {
            // Opts
            {
                if (ImGui::CollapsingHeader("Memory options"))
                    ImGui::Checkbox("Follow r/w operations", &this->m_ui_memoryFollowOPS);
            }

            uint8_t underlying[0xFF];
            buffer::stack_buffer memoryBuffer(underlying, 0xFF);
            const auto result = this->memory.readMemory(this->m_processHandle,this->m_memoryPtr, memoryBuffer);
            {
                char addrStr[12] = {0};
                if (ImGui::InputText("Address", addrStr, 12,
                                     ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal)) {
                    try {
                        this->m_memoryPtr = std::stoul(addrStr, nullptr, 16);
                    } catch (...) {}
                }

                ImGui::TextColored({0.8, 0.8, 0.8, 1.}, "Current address: 0x%llX", this->m_memoryPtr);
                if (result != 0) {
                    ImGui::SameLine();
                    ImGui::TextColored({0.7, 0.5, 0.5, 1}, "Read error: 0x%X", result);
                }

                ImGui::Spacing();
                int32_t scroll = 0;
                ImGui::VSliderInt("", {10, ImGui::GetWindowHeight() - 80},
                                  &scroll, -12, 12, "", 0);
                this->m_memoryPtr -= scroll * 8;
            }

            ImGui::SameLine();
            ImGui::BeginChild("memory_view_area", {0, 0}, false,
                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            for (size_t offset = 0; offset < 0xFF; offset += 8) {
                ImGui::TextColored({0.5, 0.5, 0.5, 1}, "%08llX: ", this->m_memoryPtr + memoryBuffer.getCursor());
                uint8_t ascii[9] = {0};

                bool tmpSelected = false;
                ImGui::SameLine();
                for (uint8_t byteIndex = 0; byteIndex < 8; byteIndex++) {
                    uint8_t readByte = memoryBuffer.readByte();

                    ImGui::Text("%s", fmt::format("{:02x} ", readByte).c_str());
                    ImGui::SameLine();
                    if (std::isalnum(readByte))
                        ascii[byteIndex] = readByte;
                    else
                        ascii[byteIndex] = '.';
                }
                ImGui::TextColored({0.5, 0.5, 0.5, 1}, "|");
                ImGui::SameLine();
                ImGui::Text("%s", ascii);
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

}
