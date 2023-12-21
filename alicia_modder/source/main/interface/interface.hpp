//
// Created by maros on 22/07/2021.
//

#ifndef ALICIA_MODDER_INTERFACE_HPP
#define ALICIA_MODDER_INTERFACE_HPP

#include "imgui.h"

#include <d3d9.h>
#include <functional>
#include <unordered_map>


using frame_populator = std::function<void(void)>;

namespace mod
{
class Interface
{

public:
  static constexpr int32_t WINDOW_WIDTH = 1280;
  static constexpr int32_t WINDOW_HEIGHT = 800;

private:
  bool m_running = false;
  bool m_restart = false;
  uint32_t m_framepopulatorId = -1;

  std::unordered_map<int, frame_populator> framePopulators;


private:
  HWND hwnd;
  WNDCLASSEX wndClass;

public:
  bool darkTheme = true;
  bool alwaysOnTop = true;


private:
  bool createWindow();
  bool destroyWindow();
  bool createDirectX();
  bool destroyDirectX();

public:
  /**
         * Starts UI
         */
  void start();
  /**
         * Stops UI
         * @return
         */
  void stop();

  void restart();
  [[nodiscard]] bool isRunning() const;


  int addFramePopulator(frame_populator populator);
  void remFramePopulator(int id);

  static void restartDevice();
};
}// namespace mod

#endif//ALICIA_MODDER_INTERFACE_HPP
