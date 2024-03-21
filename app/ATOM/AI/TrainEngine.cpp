//
// Created by toor on 2/16/24.
//

#include "TrainEngine.h"
#include "ATOM/Application.h"
#include <filesystem>


namespace Atom
{
    TrainEngine::TrainEngine()
        : Layer("TrainEngine")
    {
#ifdef NDEBUG
        m_Model = new YoloV8("/home/toor/Code/EarthRover/app/ASSETS/roadsign.onnx", config);
        m_Frame = &Application::GetApp().GetFrame().GetNativeFrame();
#endif
    }

    TrainEngine::~TrainEngine()
    {
        delete m_Model;
        delete m_Frame;
        delete localTime;
    }

    void TrainEngine::OnAttach()
    {
    }

    void TrainEngine::OnDetach()
    {
    }

    void TrainEngine::OnUpdate()
    {
    }

    void TrainEngine::OnFixedUpdate()
    {
#ifdef NDEBUG
        if (!m_Frame->empty())
        {
            if (!isTextureGenerated)
            {
                glGenTextures(1, &m_Texture);
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Frame->cols,
                             m_Frame->rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                             m_Frame->data);
                glBindTexture(GL_TEXTURE_2D, 0);

                m_AspectRatio = (float)m_Frame->cols / (float)m_Frame->rows;
                isTextureGenerated = true;
            }

            if (isTextureGenerated && m_Texture != 0)
            {
                cv::copyTo(*m_Frame, m_LocalFrame, cv::noArray());
                // m_LocalFrame
                cv::flip(m_LocalFrame, m_LocalFrame,
                         90);
                auto detectionStartTime = std::chrono::high_resolution_clock::now();

                const auto objects = m_Model->detectObjects(m_LocalFrame);
                // Stop measuring object detection time
                auto detectionEndTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> detectionTimeSpan = detectionEndTime - detectionStartTime;
                detectionTime = detectionTimeSpan.count();


                // Draw the bounding boxes on the image
                m_Model->drawObjectLabels(m_LocalFrame, objects);
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                //get type of image
                if (m_LocalFrame.type() == CV_8UC3)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_LocalFrame.cols,
                                 m_LocalFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                                 m_LocalFrame.data);
                }
                else if (m_LocalFrame.type() == CV_8UC4)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_LocalFrame.cols,
                                 m_LocalFrame.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                                 m_LocalFrame.data);
                }
                glBindTexture(GL_TEXTURE_2D, 0);

                if (ImGui::IsKeyPressed(ImGuiKey_Space))
                {
                    for (const auto& object : objects)
                    {
                        Thumbnail thumbnail;
                        thumbnail.ImagePath = std::filesystem::current_path().string() + "/ASSETS/";
                        // 16 caracters long
                        thumbnail.UUID = std::to_string(rand() % 10000000000000000);
                        //save the image at path ImagePath + UUID + ".png"
                        cv::imwrite(thumbnail.ImagePath + thumbnail.UUID + ".png", m_LocalFrame);
                        ATLOG_INFO("Image saved at: {0}", thumbnail.ImagePath + thumbnail.UUID + ".png");
                        thumbnail.Class = m_Model->getLabels()[object.label];
                        thumbnail.Treshold = std::to_string(object.probability);
                        auto now = std::time(nullptr);
                        localTime = std::localtime(&now);
                        thumbnail.Time = std::to_string(localTime->tm_hour) + ":" + std::to_string(localTime->tm_min) +
                            ":" + std::to_string(localTime->tm_sec);
                        thumbnail.Position = glm::vec2(object.rect.x, object.rect.y);
                        thumbnail.Size = glm::vec2(object.rect.width, object.rect.height);
                        m_Thumbnails.push_back(thumbnail);
                    }

                    // lastTime = currentTime;
                }

                //if button enter is presed take a  photo
                if (ImGui::IsKeyPressed(ImGuiKey_Equal))
                {
                    std::string randname = std::to_string(rand() % 10000000000000000);
                    cv::imwrite(std::filesystem::current_path().string() + "/ASSETS/" + randname + ".png", *m_Frame);
                }
            }
        }
#endif
    }

    void TrainEngine::OnImGuiRender()
    {
        ImGui::Begin("Ai Engine");
#ifdef NDEBUG
        // print detection time and frame rate
        ImGui::Text("Detection Time: %f ms", detectionTime);
        ImGui::Text("Frame Rate: %d", m_FrameRate);

        if (isTextureGenerated == true)
        {
            ImVec2 canvas_left_top = ImGui::GetCursorScreenPos();
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            ImVec2 canvas_bottom_right = ImVec2(canvas_left_top.x + canvas_size.x, canvas_left_top.y + canvas_size.y);
            ImVec2 available = ImGui::GetContentRegionAvail();
            if (available.x > available.y * m_AspectRatio)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (available.x / 2 - available.y * m_AspectRatio / 2));
                canvas_left_top.x = ImGui::GetCursorScreenPos().x;
                canvas_bottom_right.x = canvas_left_top.x + available.y * m_AspectRatio;
                available.x = available.y * m_AspectRatio;
            }
            else
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));
                canvas_left_top.y = ImGui::GetCursorScreenPos().y;
                canvas_bottom_right.y = canvas_left_top.y + available.x / m_AspectRatio;
                available.y = available.x / m_AspectRatio;
            }
            ImGuiIO& io = ImGui::GetIO();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_left_top, canvas_bottom_right, IM_COL32(50, 50, 50, 255));


            //draw image to background
            draw_list->AddImage((void*)m_Texture, canvas_left_top, canvas_bottom_right, ImVec2(0, 0),
                                ImVec2(1, 1));


            draw_list->AddRect(canvas_left_top, canvas_bottom_right, IM_COL32(255, 255, 255, 255));
            draw_list->PushClipRect(canvas_left_top, canvas_bottom_right, true);
            draw_list->PopClipRect();
        }
        else
        {
            ImGui::Text("Please Open camera before");
        }

#endif

        ImGui::End();
    }
}
