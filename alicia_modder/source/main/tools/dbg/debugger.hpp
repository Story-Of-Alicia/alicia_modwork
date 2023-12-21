//
// Created by maros on 22/07/2021.
//

#ifndef ALICIA_MODDER_DEBUGGER_HPP
#define ALICIA_MODDER_DEBUGGER_HPP

#include <string>
#include <thread>
#include <vector>

#include <Windows.h>

#include <main/buffer/buffer.hpp>

namespace dbg
{

/**
     * Memory API
     */
class MemoryController
{
public:
  HANDLE* m_processHandle = nullptr;

public:
  /**
         * Reads process memory at specified address.
         * @param procHandle Process Handle
         * @param addr   Address to read
         * @param buffer Buffer
         * @return Zero value if successful. Otherwise non-zero winerror.
         */
  uint32_t readMemory(const HANDLE procHandle, uint64_t addr, buffer::stack_buffer& buffer);

  /**
         * Writes process memory at specified address.
         * @param procHandle Process Handle
         * @param addr   Address to write
         * @param buffer Buffer
         * @return Zero value if successful. Otherwise non-zero winerror.
         */
  uint32_t writeMemory(const HANDLE procHandle, uint64_t addr, const buffer::stack_buffer& buffer);
};

/**
     * Debugging API
     */
class DebuggingTool
{
public:
  uint32_t m_processId = 0;
  HANDLE m_processHandle = nullptr;
  uint64_t m_allocationBase = 0x0;

  std::string m_processName;

public:
  std::unique_ptr<std::thread> m_debuggingThread;
  std::unique_ptr<std::thread> m_autoAttachingThread;

public:
  std::vector<std::string> m_eventLog;

public:
  dbg::MemoryController memory;

public:
  bool m_ui_debug = true;
  bool m_shouldDebug = true;


  bool m_ui_autoAttach = false;
  bool m_shouldAutoAttach = false;
  int m_autoAttachDelayMs = 10;

  /**
         * Output opts
         */

  bool m_ui_outputAutoScroll = true;
  bool m_ui_outputLockPlace = true;
  bool m_ui_showOutput = true;

  /**
         * Memory opts
         */
  bool m_ui_memoryFollowOPS = true;
  bool m_ui_memoryLockPlace = true;
  bool m_ui_showMemory = true;
  uint64_t m_memoryPtr = 0x0;

  bool m_isDebugging = false;
  bool m_isAttached = false;

public:
  ~DebuggingTool();

public:
  /**
         * Handles WinAPI errors
         */
  static void winapi_debugErrors(const char* func);

  /**
         * Tries to access process ID
         * @return success/failure
         */
  bool winapi_inquiryProcessID();

  /**
         * Tries to open alicia process
         * @return success/failure
         */
  bool winapi_inquiryProcessHandle();

private:
  /**
         * Initializes debugging thread
         */
  void initializeDebugger();

  /**
         * Terminates debugging thread
         */
  void terminateDebugger();

  /**
         *
         * @return Boolean true if debugging is active. Otherwise false.
         */
  bool isDebugging() const;

  /**
         * Initializes auto attaching thread
         */
  void initializeAutoAttacher();

  /**
         * Terminates auto attaching thread
         */
  void terminateAutoAttacher();

public:
  void setProcessName(std::string& name);

  void killProcessOnDebuggerExit(bool val);

  /**
         * Attaches to process
         * @return Boolean true if attaching was successful. Otherwise false.
         */
  bool attach();

  /**
         * Detaches from process
         */
  void detach();

  /**
         * @return Boolean true if attached to the process. Otherwise false.
         */
  bool isAttached();

public:
  uint32_t readMemory(uint64_t addr, buffer::stack_buffer& buffer);
  uint32_t writeMemory(uint64_t addr, const buffer::stack_buffer& buffer);


public:
  void renderInterface();
};


}// namespace dbg


#endif//ALICIA_MODDER_DEBUGGER_HPP
