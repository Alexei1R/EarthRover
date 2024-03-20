#include "Thumbnail.h"
#include "imgui.h"

namespace Atom
{
    ThumbnailGrid::ThumbnailGrid(std::string path, TrainEngine* trainEngine)
        : m_Path(path), m_TrainEngine(trainEngine)
    {
        // Simulate a list of 10 thumbnails
        // for (int i = 0; i < 100; ++i)
        // {
        //     Thumbnail thumbnail;
        //     thumbnail.ImagePath = "Path to image " + std::to_string(i);
        //     thumbnail.Class = "Class:: " + std::to_string(i);
        //     thumbnail.Treshold = "Treshold:: " + std::to_string((rand() % 100) + 1);
        //     thumbnail.Time = "Time:: " + std::to_string((rand() % 24) + 1) + ":" + std::to_string((rand() % 60) + 1) +
        //         ":" + std::to_string((rand() % 60) + 1);
        //     thumbnail.Position = glm::vec2(0, 0);
        //     thumbnail.Size = glm::vec2(200, 250);
        //
        //     m_Thumbnails.push_back(thumbnail);
        //
        // }
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
        //Copy the thumbnails from the train engine
        m_Thumbnails = m_TrainEngine->GetThumbnails();

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


    void ThumbnailGrid::OnImGuiRender()
    {
        ImGui::Begin("Thumbnail Grid");



        // Button to save the thumbnails
        if (ImGui::Button("Save Thumbnails")) {
            // Use Zenity to get the folder path
            char path[1024];
            FILE* f = popen("zenity --file-selection --directory", "r");
            if (f == NULL) {
                ATLOG_ERROR("Failed to get the folder");
            }
            else {
                if (fgets(path, 1024, f) != NULL) {
                    // Folder selected
                    ATLOG_INFO("Folder selected: {0}", path);

                    // Remove newline character if present
                    path[strcspn(path, "\n")] = 0;

                    // Create the data.yaml file in the selected folder
                    std::string yamlFilePath = std::string(path) + "/data.yaml";
                    std::ofstream yamlFile(yamlFilePath);
                    if (!yamlFile.is_open()) {
                        ATLOG_ERROR("Failed to create YAML file in selected folder");
                    }
                    else {
                        yamlFile.close();
                        Serialize::SerializeData(yamlFilePath, m_Thumbnails);
                    }
                } else {
                    ATLOG_INFO("Folder selection canceled");
                }
                pclose(f);
            }
        }

        //Print in a list of thumbnails
        for (size_t i = 0; i < m_Thumbnails.size(); ++i)
        {
            ImGui::Text("%s", m_Thumbnails[i].Class.c_str());
            ImGui::SameLine();
            ImGui::Text("%s", m_Thumbnails[i].Treshold.c_str());
            ImGui::SameLine();
            ImGui::Text("%s", m_Thumbnails[i].Time.c_str());
            // ImGui::Image((void*)m_Thumbnails[i].m_Texture, ImVec2(200, 250));
        }

        //
        //
        // float availableWidth = ImGui::GetContentRegionAvail().x - 35;
        // m_ThumbnailsPerRow = std::max(1, (int)(availableWidth / m_ThumbnailSize.x));
        //
        // float totalPadding = std::max(
        //     0.0f, (availableWidth - m_ThumbnailsPerRow * m_ThumbnailSize.x) / (m_ThumbnailsPerRow + 1));
        //
        // // Begin scrolling region
        // ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);
        //
        // bool hovered = false;
        // bool selected = false;
        //
        // for (size_t i = 0; i < m_Thumbnails.size(); ++i)
        // {
        //     if (i % m_ThumbnailsPerRow != 0)
        //         ImGui::SameLine(0.0f, totalPadding);
        //
        //     DrawThumbnail(m_Thumbnails[i], i, hovered, selected);
        // }
        //
        // // End scrolling region
        // ImGui::EndChild();

        ImGui::End();
    }
    //
    //
    // void ThumbnailGrid::DrawThumbnail(Thumbnail& thumbnail, int index, bool& hovered, bool& selected)
    // {
    //     ImVec2 min = ImGui::GetCursorScreenPos();
    //     ImVec2 max = ImVec2(min.x + m_ThumbnailSize.x, min.y + m_ThumbnailSize.y);
    //
    //     if (ImGui::InvisibleButton(("Thumbnail" + std::to_string(index)).c_str(),
    //                                ImVec2(m_ThumbnailSize.x, m_ThumbnailSize.y)))
    //     {
    //         ATLOG_INFO("Thumbnail {0} clicked", index);
    //         selected = true;
    //     }
    //
    //     bool isHovered = ImGui::IsItemHovered();
    //     hovered |= isHovered;
    //
    //     selected &= ImGui::IsItemActive();
    //
    //     // Draw filled rectangle with round corners
    //     ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //     draw_list->AddRectFilled(min, max, IM_COL32(50, 50, 50, 255), 10.0f);
    //     //draw a red circle in the middle of the thumbnail
    //     draw_list->AddCircleFilled(ImVec2(min.x + m_ThumbnailSize.x / 2, min.y + m_ThumbnailSize.y / 2), 10,
    //                                IM_COL32(255, 0, 0, 255));
    //     //draw a rectagle 200 x 150 center top of the thumbnail
    //     draw_list->AddRectFilled(ImVec2(min.x + 3, min.y + 3), ImVec2(min.x + 197, min.y + 147), IM_COL32(0, 0, 0, 255),
    //                              10.0f);
    //     //resize the image to fit the rectangle
    //     draw_list->AddImage((void*)thumbnail.m_Texture, ImVec2(min.x + 3, min.y + 3), ImVec2(min.x + 197, min.y + 147),
    //                         ImVec2(0, 0), ImVec2(1, 1));
    //
    //
    //     //print the class treadhold and time on the top of the thumbnail
    //     draw_list->AddText(ImVec2(min.x + 5, min.y + 155), IM_COL32(255, 255, 255, 255), thumbnail.Class.c_str());
    //     draw_list->AddText(ImVec2(min.x + 5, min.y + 180), IM_COL32(255, 255, 255, 255), thumbnail.Treshold.c_str());
    //     draw_list->AddText(ImVec2(min.x + 5, min.y + 205), IM_COL32(255, 255, 255, 255), thumbnail.Time.c_str());
    //
    //
    //     // Tooltip if hovered
    //     if (isHovered)
    //     {
    //         ImGui::BeginTooltip();
    //         ImGui::Text("Class: %s", thumbnail.Class.c_str());
    //         ImGui::Text("Treshold: %s", thumbnail.Treshold.c_str());
    //         ImGui::Text("Time: %s", thumbnail.Time.c_str());
    //         ImGui::EndTooltip();
    //     }
    // }
}
