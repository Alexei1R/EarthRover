//
// Created by toor on 3/18/24.
//

#include "ThumbnailViewer.h"


#include "Thumbnail.h"
#include "imgui.h"

namespace Atom
{
    ThumbnailViewer::ThumbnailViewer()
    {
    }

    ThumbnailViewer::~ThumbnailViewer()
    {
    }

    void ThumbnailViewer::OnAttach()
    {
    }

    void ThumbnailViewer::OnDetach()
    {
    }

    void ThumbnailViewer::OnUpdate()
    {
    }

    void ThumbnailViewer::OnFixedUpdate()
    {
        //Copy the thumbnails from the train engine
        // m_Thumbnails = m_TrainEngine->GetThumbnails();

        // glGenTextures(1, &textureID);
        // glBindTexture(GL_TEXTURE_2D, textureID);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        // glBindTexture(GL_TEXTURE_2D, 0);
        //create a texture for each thumbnail


        // for (size_t i = 0; i < m_Thumbnails.size(); ++i)
        // {
        //     if (!m_Thumbnails[i].m_Frame.empty())
        //     {
        //         if(m_Thumbnails[i].isTextureGenerated == false)
        //         {
        //             glGenTextures(1, &m_Thumbnails[i].m_Texture);
        //             glBindTexture(GL_TEXTURE_2D, m_Thumbnails[i].m_Texture);
        //             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Thumbnails[i].m_Frame.cols, m_Thumbnails[i].m_Frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_Thumbnails[i].m_Frame.data);
        //             glBindTexture(GL_TEXTURE_2D, 0);
        //             m_Thumbnails[i].isTextureGenerated = true;
        //         }
        //     }
        // }
    }


    void ThumbnailViewer::OnImGuiRender()
    {
        ImGui::Begin("Thumbnail Viewer");

        //Minim first window size to 200 x 200
        ImGui::SetWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        ImGui::SetWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);






        if (ImGui::Button("Open Folder"))
        {
            ATLOG_INFO("Save File");
            char path[1024];
            FILE* f = popen("zenity --file-selection --directory", "r");
            fgets(path, 1024, f);
            ATLOG_INFO("path: {0}", path);

            // Remove trailing newline character if present
            path[strcspn(path, "\n")] = 0;

            // Construct path to YAML file
            std::string yamlFilePath = std::string(path) + "/data.yaml";

            // Check if the YAML file exists
            std::ifstream file(yamlFilePath);
            if (!file)
            {
                ATLOG_ERROR("Failed to open YAML file: {0}", yamlFilePath);
            }
            else
            {
                // Deserialize thumbnails from the YAML file
                m_Thumbnails = Serialize::DeSerializeData(yamlFilePath);
            }

            pclose(f);
        }





        if (m_Thumbnails.size() > 1)
        {
            float availableWidth = ImGui::GetContentRegionAvail().x - 35;
            m_ThumbnailsPerRow = std::max(1, (int)(availableWidth / m_ThumbnailSize.x));

            float totalPadding = std::max(
                0.0f, (availableWidth - m_ThumbnailsPerRow * m_ThumbnailSize.x) / (m_ThumbnailsPerRow + 1));

            // Begin scrolling region
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);

            bool hovered = false;
            bool selected = false;

            for (size_t i = 0; i < m_Thumbnails.size(); ++i)
            {
                if (i % m_ThumbnailsPerRow != 0)
                    ImGui::SameLine(0.0f, totalPadding);

                DrawThumbnail(m_Thumbnails[i], i, hovered, selected);
            }

            // End scrolling region
            ImGui::EndChild();
        }

        ImGui::End();
    }


    void ThumbnailViewer::DrawThumbnail(Thumbnail& thumbnail, int index, bool& hovered, bool& selected)
    {
        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + m_ThumbnailSize.x, min.y + m_ThumbnailSize.y);

        if (ImGui::InvisibleButton(("Thumbnail" + std::to_string(index)).c_str(),
                                   ImVec2(m_ThumbnailSize.x, m_ThumbnailSize.y)))
        {
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
        draw_list->AddCircleFilled(ImVec2(min.x + m_ThumbnailSize.x / 2, min.y + m_ThumbnailSize.y / 2), 10,
                                   IM_COL32(255, 0, 0, 255));
        //draw a rectagle 200 x 150 center top of the thumbnail
        draw_list->AddRectFilled(ImVec2(min.x + 3, min.y + 3), ImVec2(min.x + 197, min.y + 147), IM_COL32(0, 0, 0, 255),
                                 10.0f);
        //resize the image to fit the rectangle
        draw_list->AddImage((void*)thumbnail.m_Texture, ImVec2(min.x + 3, min.y + 3), ImVec2(min.x + 197, min.y + 147),
                            ImVec2(0, 0), ImVec2(1, 1));


        //print the class treadhold and time on the top of the thumbnail
        draw_list->AddText(ImVec2(min.x + 5, min.y + 155), IM_COL32(255, 255, 255, 255), thumbnail.Class.c_str());
        draw_list->AddText(ImVec2(min.x + 5, min.y + 180), IM_COL32(255, 255, 255, 255), thumbnail.Treshold.c_str());
        draw_list->AddText(ImVec2(min.x + 5, min.y + 205), IM_COL32(255, 255, 255, 255), thumbnail.Time.c_str());


        // Tooltip if hovered
        if (isHovered)
        {
            ImGui::BeginTooltip();
            ImGui::Text("Class: %s", thumbnail.Class.c_str());
            ImGui::Text("Treshold: %s", thumbnail.Treshold.c_str());
            ImGui::Text("Time: %s", thumbnail.Time.c_str());
            ImGui::EndTooltip();
        }
    }
}
