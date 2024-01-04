//
// Created by maros on 16/09/2021.
//

#include "assets_modder.hpp"
#include "main/helpers/filesystem.hpp"
#include <fstream>

#include <imgui.h>


void renderPakContents(const assets::PakContents& contents)
{
}

void mod::AssetsModder::renderInterface()
{
  ImGui::SetNextWindowSize({420, 220}, ImGuiCond_FirstUseEver);
  ImGui::Begin("Asset Controller");
  ImGui::TextColored({0.7, 0.7, 0.7, 1}, "Ongoing: ");
  ImGui::SameLine();
  if (this->working)
  {
    ImGui::TextColored({0.8, 0.8, 0.8, 1}, "%s", workerDesc.c_str());
    ImGui::SameLine();
    ImGui::TextColored({0.6, 0.6, 0.6, 1}, "Elapsed: %.3fs", workerElapsed.elapsed().count());
  }
  else
  {
    ImGui::TextColored({0.5, 0.5, 0.5, 1}, "(nothing)");
  }
  if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
  {
    {
      ImGui::TextColored({0.7, 0.7, 0.7, 1}, "Selected asset file: ");
      ImGui::SameLine();
      if (this->pak_path.empty())
      {
        ImGui::TextColored({0.5, 0.5, 0.5, 1}, "(None)");
      }
      else
      {
        ImGui::TextColored({0.6, 0.6, 0.6, 1}, "%s", this->pak_path.c_str());
      }
      ImGui::Spacing();
    }

    {
      if (ImGui::Button("Read game assets.") && !this->working)
      {
        this->pak_path = filesystem::helper::UserSelectFile("Select PAK file", "PAK file (*.pak)\0*.pak;*.pak.*;*.pak.bak\0");
        if (!this->pak_path.empty())
        {
          this->submitWork([this]()
                           {
                        try {
                            assets::AssetsIO::read_pak_contents(this->pak_path, this->pakContents);
                        } catch (const std::system_error &error) {
                            filesystem::helper::UserMessage("Couldn't read assets.", error.what());
                        } }, "Reading game assets.");
        }
      };
      if (!this->pakContents.hasAssets())
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2, 0.2, 0.2, 1});
      else
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));

      if (ImGui::Button("Write game assets.") && !this->working)
      {
        this->pak_path = filesystem::helper::UserSelectFile("Select PAK file", "PAK file (*.pak)\0*.pak\0");
        this->submitWork([this]()
                         { assets::AssetsIO::write_pak_contents(this->pak_path, this->pakContents); }, "Writing game assets.");
      }

      ImGui::Spacing();
      if (ImGui::Button("Export game assets.") && !this->working)
      {
        std::string parent_dir = filesystem::helper::UserSelectFolder("Select folder to export to");
        this->submitWork([this, parent_dir]()
                         {
                    for (const auto &asset: this->pakContents.assets) {
                        assets::AssetsIO::export_pak_asset(parent_dir, asset);
                    } }, "Exporting game assets.");
      }
      if (ImGui::Button("Clear game assets.") && !this->working)
      {
        this->submitWork([this]()
                         { this->pakContents.assets.clear(); }, "Clearing game assets.");
      }
      ImGui::PopStyleColor();
    }
  }
  ImGui::End();

  ImGui::Begin("Assets");
  if (pakContents.hasAssets())
  {
    ImGui::SetNextWindowPos({425, 0});
    ImGui::SetNextWindowSize({420, 220}, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Headers"))
    {
      if (ImGui::CollapsingHeader("File header"))
      {
        const auto header = pakContents.fileHeader;
        const ImVec4 fieldValueCol = {0.7, 0.7, 0.8, 1};

        ImGui::Text("Magic:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%.4s)", header.header_magic, (char*) &header.header_magic);
        ImGui::Spacing();
        ImGui::Text("Unknown0:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X", header.unknown0);
        ImGui::Spacing();
        ImGui::Text("PakLib version:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X", header.paklib_version);
        ImGui::Spacing();
        ImGui::Text("Locale:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X", header.locale);
        ImGui::Spacing();
        ImGui::Text("Asset count:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%d)", header.asset_count, header.asset_count);
        ImGui::Spacing();
        ImGui::Text("Used Asset count:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%d)", header.used_assets_count, header.used_assets_count);
        ImGui::Spacing();
        ImGui::Text("Deleted Asset count:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%d)", header.deleted_assets_count, header.deleted_assets_count);
        ImGui::Spacing();
        ImGui::Text("File size:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%d)", header.file_size, header.file_size);
        ImGui::Spacing();
        ImGui::Text("Unknown 1:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%d)", header.unknown1, header.unknown1);
        ImGui::Spacing();
        ImGui::Text("Sign:");
        ImGui::SameLine();
        ImGui::TextColored(fieldValueCol, "0x%X(%.4s)", header.header_sign, (char*) &header.header_sign);
        ImGui::Spacing();
      }
      if (ImGui::CollapsingHeader("Content header"))
      {
      }
      if (ImGui::CollapsingHeader("Data header"))
      {
      }
    }
    if (ImGui::CollapsingHeader("Assets"))
    {
      static char filter[256] = {0};

      ImGui::InputText("Filter", filter, 256);
      for (const auto& item: pakContents.assets)
      {
        if (item->header.common.is_asset_deleted)
          continue;
        auto path_string_view = std::string_view(item->header.path);
        int32_t match = path_string_view.rfind(filter);
        if (*filter != 0x0 && match < 0)
          continue;
        if (ImGui::CollapsingHeader(item->header.path))
        {
          if (ImGui::Button("Select modified file"))
          {
            this->modified_path = filesystem::helper::UserSelectFile("Select modified asset file", "Asset file (*.*)\0*.*\0");
            if (!this->modified_path.empty())
            {
              this->submitWork([this, item]()
                               {
                                try {
                                    // fd for asset file
                                    std::ifstream file(this->modified_path, std::ios::binary);
                                    // retrieve file size
                                    file.seekg(0, std::ios::end);
                                    auto file_size = file.tellg();
                                    file.seekg(0, std::ios::beg);

                                    // file buffer
                                    auto data = new uint8_t[file_size];
                                    file.read((char *) data, file_size);

                                    // make function for all of this
                                    // set new data, unique_ptr deletes old data
                                    item->data.reset(data);
                                    item->header.common.data_decompressed_length = file_size;
                                    item->header.common.data_decompressed_length0 = file_size;
                                    item->header.common.data_decompressed_length1 = file_size;
                                } catch (const std::system_error &error) {
                                    filesystem::helper::UserMessage("Couldn't patch asset.", error.what());
                                } }, fmt::format("Patching asset: {}", item->header.path));
            }
          }
        };
      }
    }
  }
  ImGui::End();
}

template<typename Func>
bool mod::AssetsModder::submitWork(const Func& function, const std::string& desc)
{
  if (working)
    return false;
  if (worker.joinable())
    worker.join();

  this->working = true;
  worker = std::thread([function, this]()
                       {
        function();
        this->working = false; });
  workerDesc = desc;
  workerElapsed.reset();
}
