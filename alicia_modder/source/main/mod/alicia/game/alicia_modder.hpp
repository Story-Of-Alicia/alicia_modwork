//
// Created by maros on 01/06/2021.
//

#ifndef ALICIA_MODDER_ALICIA_MODDER_HPP
#define ALICIA_MODDER_ALICIA_MODDER_HPP

#include <cstdint>
#include <thread>
#include <utility>


#include "main/tools/dbg/debugger.hpp"

namespace mod
{
class AliciaModder
{
public:
  static constexpr const char* ALICIA_GAME_PROCESS_NAME = "Alicia.exe";
  static constexpr const char* ALICIA_LAUNCHER_PROCESS_NAME = "Launcher.exe";

protected:
  dbg::DebuggingTool debugger;

protected:
  size_t consoleWindowAddr = 0x01ea6a10;
  uint64_t consoleWindow = 0;

  size_t gameWindowAddr = 0x01ea6a10;
  HWND gameWindow = nullptr;

  size_t splashWindowAddr = 0x01ea6a10;
  HWND splashWindow = nullptr;

protected:
  bool ui_showConsoleWindow = false;
  bool ui_showFxWindow = true;

  bool ui_showSplashScreen = false;
  bool ui_showGameScreen = true;

protected:
  bool wasAttached = false;

public:
  AliciaModder();
  ~AliciaModder();

private:
  void toggleGameWindow();
  void toggleSplashWindow();
  void toggleEngineConsoleWindow(bool value);
  void toggleFxEditorWindow(bool value);

public:
  void renderInterface();
};
}// namespace mod


#endif//ALICIA_MODDER_ALICIA_MODDER_HPP
