//
// Created by maros on 16/09/2021.
//

#include "assets_modder.hpp"
#include "main/helpers/filesystem.hpp"

#include <imgui.h>




void renderPakContents(const assets::PakContents& contents) {
    ImGui::Begin("Pak Contents");
    if(ImGui::CollapsingHeader("File header", ImGuiTreeNodeFlags_DefaultOpen)) {
        const auto header = contents.fileHeader;
        const ImVec4 fieldValueCol = {0.7,0.7,0.8,1};

        ImGui::Text("Magic:");ImGui::SameLine();ImGui::TextColored(fieldValueCol, "0x%X(%.4s)", header.header_magic, (char*) &header.header_magic);
        ImGui::Spacing();
        ImGui::Text("Unknown0:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X", header.unknown0);
        ImGui::Spacing();
        ImGui::Text("PakLib version:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X", header.paklib_version);
        ImGui::Spacing();
        ImGui::Text("Locale:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X", header.locale);
        ImGui::Spacing();
        ImGui::Text("Asset count:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%d)", header.asset_count, header.asset_count);
        ImGui::Spacing();
        ImGui::Text("Used Asset count:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%d)", header.used_assets_count,header.used_assets_count);
        ImGui::Spacing();
        ImGui::Text("Deleted Asset count:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%d)", header.deleted_assets_count,header.deleted_assets_count);
        ImGui::Spacing();
        ImGui::Text("File size:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%d)", header.file_size, header.file_size);
        ImGui::Spacing();
        ImGui::Text("Unknown 1:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%d)", header.unknown1, header.unknown1);
        ImGui::Spacing();
        ImGui::Text("Sign:");ImGui::SameLine();ImGui::TextColored(fieldValueCol,"0x%X(%.4s)", header.header_sign, (char*) &header.header_sign);
        ImGui::Spacing();
    }
    if(ImGui::CollapsingHeader("Content header", ImGuiTreeNodeFlags_DefaultOpen)) {

    }
    if(ImGui::CollapsingHeader("Data header", ImGuiTreeNodeFlags_DefaultOpen)) {

    }
    ImGui::End();
}

void mod::AssetsModder::renderInterface() {
    ImGui::SetNextWindowSize({420, 220}, ImGuiCond_FirstUseEver);


    ImGui::Begin("Asset Controller");

    ImGui::TextColored({0.7, 0.7, 0.7, 1}, "Ongoing: ");
    ImGui::SameLine();
    if(this->working) {
        ImGui::TextColored({0.8, 0.8, 0.8, 1}, "%s", workerDesc.c_str());
        ImGui::SameLine();
        ImGui::TextColored({0.6, 0.6, 0.6, 1}, "Elapsed: %.3fs", workerElapsed.elapsed().count());
    } else {
        ImGui::TextColored({0.5, 0.5, 0.5, 1}, "(nothing)");
    }

    if(pakContents.hasAssets()) {
        renderPakContents(pakContents);
    }

    if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen)) {
        {
            ImGui::TextColored({0.7, 0.7, 0.7, 1}, "Selected asset file: ");
            ImGui::SameLine();
            if (this->pak_path.empty()) {
                ImGui::TextColored({0.5, 0.5, 0.5, 1}, "(None)");
            } else {
                ImGui::TextColored({0.6, 0.6, 0.6, 1}, "%s", this->pak_path.c_str());
            }
            ImGui::Spacing();
        }

        {



            if (ImGui::Button("Read game assets.") && !this->working) {
                this->pak_path = filesystem::helper::UserSelectFile("Select PAK file", "PAK file (*.pak)\0*.pak\0");
                if(!this->pak_path.empty()) {
                    this->submitWork([this]() {
                        try {
                            assets::AssetsIO::read_pak_contents(this->pak_path, this->pakContents);
                        } catch (const std::system_error &error) {
                            filesystem::helper::UserMessage("Couldn't read assets.", error.what());
                        }
                    }, "Reading game assets.");
                }
            };
            ImGui::Spacing();
            if (!this->pakContents.hasAssets())
                ImGui::PushStyleColor(ImGuiCol_Button, {0.2, 0.2, 0.2, 1});
            else
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));

            if (ImGui::Button("Write game assets.") && !this->working) {
                this->pak_path = filesystem::helper::UserSelectFile("Select PAK file", "PAK file (*.pak)\0*.pak\0");
                this->submitWork([this]() {
                    assets::AssetsIO::write_pak_contents(this->pak_path, this->pakContents);
                }, "Writing game assets.");
            }

            if (ImGui::Button("Export game assets.") && !this->working) {
                std::string parent_dir = filesystem::helper::UserSelectFolder("Select folder to export to");
                this->submitWork([this, parent_dir]() {
                    for (const auto &asset : this->pakContents.assets) {
                        assets::AssetsIO::export_pak_asset(parent_dir, asset);
                    }
                }, "Exporting game assets.");
            }
            ImGui::Spacing();
            ImGui::PopStyleColor();
        }
    }

    if (ImGui::CollapsingHeader("Patches", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextColored({0.7, 0.7, 0.7, 1}, "Selected patch file: ");
        ImGui::SameLine();
        if (this->info_path.empty())
            ImGui::TextColored({0.5, 0.5, 0.5, 1}, "(None)");
        else
            ImGui::TextColored({0.6, 0.6, 0.6, 1}, "%s", this->info_path.c_str());

        ImGui::Spacing();

        ImGui::Button("Read game patch.");
        ImGui::Spacing();
        ImGui::Button("Write game patch.");
        ImGui::Spacing();
    }
    ImGui::End();
}

template<typename Func>
bool mod::AssetsModder::submitWork(const Func &function, const std::string &desc) {
    if (working)
        return false;
    if (worker.joinable())
        worker.join();

    this->working = true;
    worker = std::thread([function, this]() {
        function();
        this->working = false;
    });
    workerDesc = desc;
    workerElapsed.reset();
}
