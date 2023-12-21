//
// Created by maros on 12/09/2021.
//

#include "data_view.hpp"

#include <imgui.h>

void mod::DataView::render_data_view(uint8_t* data, size_t data_length)
{
  ImGui::Begin("Data view", nullptr, ImGuiWindowFlags_MenuBar);
  {
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("Opts"))
    {
      if (ImGui::RadioButton("ASCII view", this->type == ViewType::ASCII))
        this->type = ViewType::ASCII;
      if (ImGui::RadioButton("BINARY view", this->type == ViewType::BINARY))
        this->type = ViewType::BINARY;

      ImGui::Separator();
      if (ImGui::RadioButton("MIXED view", this->type == ViewType::MIXED))
        this->type = ViewType::MIXED;
      ImGui::Separator();
      ImGui::InputScalar("Binary align", ImGuiDataType_U32, &align, (void*) 0, (void*) 10);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (data == nullptr)
  {
    ImGui::End();
    return;
  }

  if (align <= 0)
    align = 1;
  switch (this->type)
  {
    case ViewType::ASCII:
    {
      ImGui::PushStyleColor(ImGuiCol_Text, {0.8, 0.8, .8, 1});
      ImGui::Text("%.*s", (int) data_length, (char*) data);
      ImGui::PopStyleColor();
      break;
    }
    case ViewType::BINARY:
    {
      ImGui::PushStyleColor(ImGuiCol_Text, {0.8, 0.8, .8, 1});
      for (int i = 0; i < data_length; ++i)
      {
        uint8_t byte = data[i];
        if (i % align != 0)
          ImGui::SameLine();
        ImGui::Text("%02x", byte);
      }
      ImGui::PopStyleColor();
      break;
    }
  }

  ImGui::End();
}
