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
        
        //Filter the thumbnails by treadhold
        m_FilteredThumbnails.clear();
        for (auto& thumbnail : m_Thumbnails)
        {
            if (std::stof(thumbnail.Treshold) >= m_ThumbnailTreshold)
            {
                m_FilteredThumbnails.push_back(thumbnail);
            }
        }

        //Sort the thumbnails by time bool m_SortByTimeBeginWithNewest = true;
        if (m_SortByTimeBeginWithNewest)
        {
            std::sort(m_FilteredThumbnails.begin(), m_FilteredThumbnails.end(),
                      [](const Thumbnail& a, const Thumbnail& b) { return a.Time > b.Time; });
        }
        else
        {
            std::sort(m_FilteredThumbnails.begin(), m_FilteredThumbnails.end(),
                      [](const Thumbnail& a, const Thumbnail& b) { return a.Time < b.Time; });
        }

    }


    void ThumbnailViewer::OnImGuiRender()
    {
        ImGui::Begin("Thumbnail Viewer");

        //Minim first window size to 200 x 200
        ImGui::SetWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        ImGui::SetWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);


        if (ImGui::Button("Open Folder"))
        {
            std::thread t(&ThumbnailViewer::ExecuteSystemCommandAsync, this, "zenity --file-selection --directory --modal --title=\"Select a folder\"");
            t.detach();
        }
        ImGui::SameLine();
        ImGui::Text("Thumbnails: %d", m_FilteredThumbnails.size());

        //Checkbox for sorting by time
        ImGui::Checkbox("Sort by time", &m_SortByTimeBeginWithNewest);

        ImGui::Spacing();
        //Slider for the treadhold
        ImGui::SliderFloat("Treadhold", &m_ThumbnailTreshold, 0.0f, 1.0f);
        ImGui::Spacing();




        if (m_FilteredThumbnails.size() > 1)
        {
            float availableWidth = ImGui::GetContentRegionAvail().x - 35;
            m_ThumbnailsPerRow = std::max(1, (int)(availableWidth / m_ThumbnailSize.x));

            float totalPadding = std::max(
                0.0f, (availableWidth - m_ThumbnailsPerRow * m_ThumbnailSize.x) / (m_ThumbnailsPerRow + 1));

            // Begin scrolling region
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);

            bool hovered = false;
            bool selected = false;

            for (size_t i = 0; i < m_FilteredThumbnails.size(); ++i)
            {
                if (i % m_ThumbnailsPerRow != 0)
                    ImGui::SameLine(0.0f, totalPadding);

                DrawThumbnail(m_FilteredThumbnails[i], i, hovered, selected);
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
            std::thread t(&ThumbnailViewer::OpenImageInDefaultViewerAsync, this, thumbnail.ImagePath + thumbnail.UUID + ".png");
            t.detach();
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


        //print the class treadhold and time on the top of the thumbnail
        draw_list->AddText(ImVec2(min.x + 5, min.y + 155), IM_COL32(255, 255, 255, 255), thumbnail.Class.c_str());
        draw_list->AddText(ImVec2(min.x + 5, min.y + 180), IM_COL32(255, 255, 255, 255), thumbnail.Treshold.c_str());
        draw_list->AddText(ImVec2(min.x + 5, min.y + 205), IM_COL32(255, 255, 255, 255), thumbnail.Time.c_str());


        // Tooltip if hovered
        if (isHovered)
        {
            ImGui::BeginTooltip();
            if (m_HoveredIndex != index)
            {
                // m_FrameOnHover read using opencv , create a texture and display it
                m_FrameOnHover = cv::imread(thumbnail.ImagePath + thumbnail.UUID + ".png");
                if (!m_FrameOnHover.empty())
                {
                    // m_HoveredTexture
                    glGenTextures(1, &m_HoveredTexture);
                    glBindTexture(GL_TEXTURE_2D, m_HoveredTexture);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_FrameOnHover.cols, m_FrameOnHover.rows, 0, GL_BGR,
                                 GL_UNSIGNED_BYTE, m_FrameOnHover.data);
                }
                m_HoveredIndex = index;
            }
            if (m_HoveredTexture != 0)
            {
                ImGui::Image((void*)(intptr_t)m_HoveredTexture, ImVec2(480, 360));
            }
            ImGui::EndTooltip();
        }

    }




    void ThumbnailViewer::ExecuteSystemCommandAsync(const std::string& command)
    {
        // Acquire lock
        std::lock_guard<std::mutex> lock(m_Mutex);

        char path[1024];
        FILE* f = popen(command.c_str(), "r");
        if (f)
        {
            fgets(path, 1024, f);
            path[strcspn(path, "\n")] = 0;
            pclose(f);

            std::string yamlFilePath = std::string(path) + "/data.yaml";
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
        }
        else
        {
            ATLOG_ERROR("Failed to execute system command.");
        }
    }

    void ThumbnailViewer::OpenImageInDefaultViewerAsync(const std::string& imagePath)
    {
        // Acquire lock
        std::lock_guard<std::mutex> lock(m_Mutex);

        std::string command = "xdg-open " + imagePath;
        int result = system(command.c_str());
        if (result != 0)
        {
            ATLOG_ERROR("Failed to open image in default viewer.");
        }

        // Update the currently opened image path
        m_CurrentlyOpenedImage = imagePath;
    }

}
