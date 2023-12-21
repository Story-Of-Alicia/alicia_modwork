//
// Created by maros on 01/06/2021.
//


#include "alicia_modder.hpp"

#include <imgui.h>

#include <cctype>
#include <chrono>
#include <climits>
#include <string>


#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


mod::AliciaModder::AliciaModder()
{
  std::string name = ALICIA_GAME_PROCESS_NAME;
  this->debugger.setProcessName(name);
}

mod::AliciaModder::~AliciaModder()
{
}


void mod::AliciaModder::toggleGameWindow()
{
}

void mod::AliciaModder::toggleSplashWindow()
{
}

void mod::AliciaModder::toggleEngineConsoleWindow(bool value)
{
  if (this->consoleWindow == 0)
  {
    buffer::stack_buffer buf(&this->consoleWindow, sizeof(uint32_t));
    this->debugger.readMemory(this->consoleWindowAddr, buf);
    spdlog::info("COnsole window handle: 0x({:x}) 0x{:x}", 0x000E031E, (uint32_t) consoleWindow);
    spdlog::info("COnsole window handle: 0x({:x}) 0x{:x}", 0x000E031E, (uint32_t) consoleWindow);
  }

  ShowWindow((HWND) this->consoleWindow, value ? SW_SHOW : SW_HIDE);
}

void mod::AliciaModder::toggleFxEditorWindow(bool value)
{
}

void mod::AliciaModder::renderInterface()
{
  this->debugger.renderInterface();


  bool tmp;
  ImGui::Begin("Alicia Modder");
  if (ImGui::CollapsingHeader("Assets"))
  {
    ImGui::Checkbox("Disable patch system", &tmp);
  }


  if (ImGui::CollapsingHeader("Windows"))
  {
    ImGui::PushStyleColor(ImGuiCol_Button, {0.4, 0.4, 0.4, 1});
    if (ImGui::Checkbox("Show engine console", &this->ui_showConsoleWindow))
      this->toggleEngineConsoleWindow(this->ui_showConsoleWindow);
    if (ImGui::Checkbox("Show fx editor", &this->ui_showFxWindow))
      this->toggleFxEditorWindow(this->ui_showFxWindow);

    ImGui::Spacing();
    ImGui::Checkbox("Show game window", &this->ui_showGameScreen);
    ImGui::Checkbox("Show splash window", &this->ui_showSplashScreen);
    ImGui::PopStyleColor();
  }


  ImGui::End();
}
