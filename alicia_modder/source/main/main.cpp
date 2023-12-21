#include <cstdio>
#include <fmt/chrono.h>
#include <spdlog/spdlog.h>
#include <thread>


#include "main/interface/interface.hpp"
#include "main/mod/alicia/assets/assets_modder.hpp"
#include "main/mod/alicia/game/alicia_modder.hpp"

#include <spdlog/stopwatch.h>

int main(int, char**)
{
  const auto start = std::chrono::steady_clock::now();

  static bool showGameModder = false;
  static bool showAssetModder = false;

  mod::Interface ui{};

  mod::AliciaModder gameModder{};
  mod::AssetsModder assetsModder{};

  ui.addFramePopulator([&ui, &start]() {
    ImGui::SetNextWindowPos({10, 10});
    ImGui::Begin("App controls", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("App"))
    {
     if (ImGui::MenuItem("Stop application", "Ctrl+ESC")) { ui.stop(); }
     ImGui::EndMenu();
    }
    ImGui::EndMenuBar();

    if (ImGui::Checkbox("Dark theme", &ui.darkTheme))
    {
     if (ui.darkTheme)
       ImGui::StyleColorsDark();
     else
       ImGui::StyleColorsLight();
    }
    if (ImGui::Checkbox("Always on top", &ui.alwaysOnTop))
     ui.restart();

    ImGui::Separator();
    ImGui::Checkbox("Game modder", &showGameModder);
    ImGui::Checkbox("Asset modder", &showAssetModder);
    ImGui::Separator();

    ImGui::TextColored({0.5, 0.5, 0.5, 1}, "Uptime: %s", fmt::format("{:%H:%M:%S}", std::chrono::steady_clock::now() - start).c_str());
    ImGui::End();
  });


  ui.addFramePopulator([&] {
    if (showGameModder)
        gameModder.renderInterface();
    if (showAssetModder)
        assetsModder.renderInterface();
  });

  std::thread interfaceThread([&ui] {
      spdlog::info("Interface thread lifetime begin.");
      ui.start();
      ui.stop();
      spdlog::info("Interface thread lifetime end.");
  });

  interfaceThread.join();

  return 0;
}