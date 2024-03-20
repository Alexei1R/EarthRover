//
// Created by toor on 11/8/23.
//

#include "EditorLayer.h"


namespace Atom
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    EditorLayer::~EditorLayer()
    {
    }




    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate()
    {
    }

    void EditorLayer::OnFixedUpdate() {
        Layer::OnFixedUpdate();
    }

    void EditorLayer::OnImGuiRender()
    {


        for (auto& func : m_FunctionsVenctor)
        {
            func();
        }


        // ImGui::Begin("Viewport");
        // ImVec2 windowSize = ImGui::GetContentRegionAvail();
        // if (windowSize.x != previousWindowSize.x || windowSize.y != previousWindowSize.y) {
        //     previousWindowSize = windowSize;
        //     if (m_ViewportCallbackFunction) {
        //         m_ViewportCallbackFunction(windowSize.x, windowSize.y);
        //         // m_FrameBuffer->SetFramebufferTextureSizeCallback(windowSize.x, windowSize.y);
        //         // glViewport(0, 0, windowSize.x, windowSize.y);
        //     }
        // }
        //
        // ImGui::Text("Renderer2D Stats:");
        // ImGui::End();
    }
}
