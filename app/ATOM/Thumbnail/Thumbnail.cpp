#include "Thumbnail.h"
#include "imgui.h"

namespace Atom
{
    ThumbnailGrid::ThumbnailGrid(std::string path)
        : m_Path(path)
    {
        // Simulate a list of 10 thumbnails
        for (int i = 0; i < 100; ++i) {
            Thumbnail thumbnail;
            thumbnail.ImagePath = "Path to image " + std::to_string(i);
            thumbnail.Class = "Class " + std::to_string(i);
            thumbnail.Treshold = "Treshold " + std::to_string(i);
            thumbnail.Time = "Time " + std::to_string(i);
            thumbnail.Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White color for now
            m_Thumbnails.push_back(thumbnail);
        }
    }

    ThumbnailGrid::~ThumbnailGrid()
    {
    }

    void ThumbnailGrid::OnAttach()
    {
    }

    void ThumbnailGrid::OnDetach()
    {
    }

    void ThumbnailGrid::OnUpdate()
    {
    }

    void ThumbnailGrid::OnFixedUpdate()
    {
    }



    void ThumbnailGrid::OnImGuiRender() {
        ImGui::Begin("Thumbnail Grid");
        float availableWidth = ImGui::GetContentRegionAvail().x;
        m_ThumbnailsPerRow = std::max(1, (int)(availableWidth / m_ThumbnailSize.x));

        bool hovered = false;
        bool selected = false;

        for (size_t i = 0; i < m_Thumbnails.size(); ++i) {
            if (i % m_ThumbnailsPerRow != 0)
                ImGui::SameLine();

            DrawThumbnail(m_Thumbnails[i], i, hovered, selected);
        }
        ImGui::End();
    }









    void ThumbnailGrid::DrawThumbnail(Thumbnail& thumbnail, int index, bool& hovered, bool& selected) {
        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + m_ThumbnailSize.x, min.y + m_ThumbnailSize.y);

        if (ImGui::InvisibleButton(("Thumbnail" + std::to_string(index)).c_str(), ImVec2(m_ThumbnailSize.x, m_ThumbnailSize.y))) {
            ATLOG_INFO("Thumbnail {0} clicked", index);
            selected = true;
        }

        bool isHovered = ImGui::IsItemHovered();
        hovered |= isHovered;

        selected &= ImGui::IsItemActive();

        // Draw filled rectangle with round corners
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(min, max, IM_COL32(50, 50, 50, 255), 10.0f);
        //draw a red circle in the middle of the thumbnail
        draw_list->AddCircleFilled(ImVec2(min.x + m_ThumbnailSize.x / 2, min.y + m_ThumbnailSize.y / 2), 10, IM_COL32(255, 0, 0, 255));

        // Tooltip if hovered
        if (isHovered) {
            ImGui::BeginTooltip();
            ImGui::Text("Class: %s", thumbnail.Class.c_str());
            ImGui::Text("Treshold: %s", thumbnail.Treshold.c_str());
            ImGui::Text("Time: %s", thumbnail.Time.c_str());
            ImGui::EndTooltip();
        }
    }









}
