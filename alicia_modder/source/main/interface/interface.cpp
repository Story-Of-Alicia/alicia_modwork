//
// Created by maros on 22/07/2021.
//

#include "interface.hpp"

#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>

#include <spdlog/spdlog.h>
#include <cstdint>


static LPDIRECT3D9 m_pd3d = NULL;
static LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS m_d3dpp = {};


LRESULT WINAPI WndHeartbeat(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void d3dRender();

bool mod::Interface::createWindow() {
    this->wndClass = {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            WndHeartbeat,
            0L, 0L,
            GetModuleHandle(NULL),
            NULL, NULL,
            NULL, NULL,
            "Alicia Modder",
            NULL
    };

    ::RegisterClassEx(&wndClass);
    this->hwnd = ::CreateWindowExA(
            this->alwaysOnTop ? WS_EX_TOPMOST : 0,
            wndClass.lpszClassName,
            "Alicia Modder",
            WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
            100, 100, WINDOW_WIDTH, WINDOW_HEIGHT,
            NULL, NULL,
            wndClass.hInstance,
            NULL
    );

    return true;
}

bool mod::Interface::destroyWindow() {
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
    return false;
}


void clearMessageQueue() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //Clear message queue!
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void mod::Interface::start() {
    this->m_running   = true;
    this->m_restart   = false;

    this->createWindow();
    if (!this->createDirectX()) {
        this->destroyDirectX();
        ::UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
        spdlog::error("Couldn't create DirectX.");
        return;
    }

    ::ShowWindow(this->hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(this->hwnd);


    ImGui::CreateContext();
    ImGuiIO &ioRef = ImGui::GetIO();
    ioRef.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(this->hwnd);
    ImGui_ImplDX9_Init(m_pd3dDevice);


    while (this->m_running) {
        if (!this->m_running || this->m_restart)
            break;

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                this->m_running = false;
            }
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        for (const auto &item : this->framePopulators) {
            item.second();
        }

        ImGui::EndFrame();
        d3dRender();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    this->destroyDirectX();
    this->destroyWindow();

    if (this->m_restart) {
        clearMessageQueue();
        this->start();
    }


}

void mod::Interface::stop() {
    this->m_running = false;
}


void mod::Interface::restart() {
    this->m_restart = true;
}

bool mod::Interface::isRunning() const {
    return this->m_running;
}

int mod::Interface::addFramePopulator(frame_populator populator) {
    this->framePopulators[++m_framepopulatorId] = std::move(populator);
    return m_framepopulatorId;
}

void mod::Interface::remFramePopulator(int id) {

}


bool mod::Interface::createDirectX() {
    if ((m_pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
        spdlog::error("Couldn't create D3D9");
        return false;
    }


    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync

    auto result = m_pd3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &m_d3dpp,
            &m_pd3dDevice
    );
    if (result != D3D_OK) {
        spdlog::error("Couldn't create D3D9 Device: 0x{}", result);
        return false;
    }
    spdlog::info("Created DirectX9");
    return true;
}

bool mod::Interface::destroyDirectX() {
    if (m_pd3dDevice) {
        m_pd3dDevice->Release();
        m_pd3dDevice = NULL;
    }
    if (m_pd3d) {
        m_pd3d->Release();
        m_pd3d = NULL;
    }

    spdlog::info("Destroyed DirectX9");
    return true;
}

void mod::Interface::restartDevice() {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = m_pd3dDevice->Reset(&m_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}


void d3dRender() {
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    D3DCOLOR clrClear = D3DCOLOR_RGBA((int) (0.073 * 255.0f), (int) (0.082 * 255.0f), (int) (.099 * 255.0f),
                                      (int) (0.1 * 255.0f));
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clrClear, 1.0f, 0);
    if (m_pd3dDevice->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        m_pd3dDevice->EndScene();
    }
    HRESULT result = m_pd3dDevice->Present(
            NULL, NULL,
            NULL, NULL
    );

    // Handle loss of D3D9 device
    if (result == D3DERR_DEVICELOST && m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        mod::Interface::restartDevice();
}


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Win32 message handler
LRESULT WINAPI WndHeartbeat(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_SIZE:
            if (m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {

                m_d3dpp.BackBufferWidth = LOWORD(lParam);
                m_d3dpp.BackBufferHeight = HIWORD(lParam);
                mod::Interface::restartDevice();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}