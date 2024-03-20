//
// Created by toor on 11/7/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H


#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"
#include "ATOM/Core/Window.h"
#include "ATOM/Core/Editor/ImGuiLayer.h"
#include "ATOM/Core/Editor/EditorLayer.h"
#include "ATOM/Sensors/Frame.h"
#include "ATOM/Core/ClientLayer.h"
#include "ATOM/AI/TrainEngine.h"
#include "ATOM/Thumbnail/Thumbnail.h"
#include "ATOM/Thumbnail/ThumbnailViewer.h"





namespace Atom {
    class Application {
    public:
        Application();

        ~Application();

        void Run();

        void PushLayer(Layer *layer);

        void PushOverlay(Layer *layer);

        static Application *s_Instance;
        inline static Application &GetApp() { return *s_Instance; }
        [[nodiscard]] inline Window &GetWindow() const { return *m_Window; }
        [[nodiscard]] inline Frame &GetFrame() const { return *m_Frame; }

        void WindowClose();


        void SelectIPPopUpWindow();

        void DrawUISetings();

        void DrawCameraSettings();

        void DrawMapSettings();

        void ConntrollCarUithGamepad();

        void DrawMenu();

    private:
        bool m_IsRuning = true;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime =
                std::chrono::high_resolution_clock::now();


        bool isConnected = false;
        bool m_VSync = true;

        Window *m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer *m_ImGuiLayer;
        EditorLayer *m_EditorLayer;
        Frame *m_Frame;
        ClientLayer *m_ClientLayer;
        TrainEngine *m_TrainEngine;
        ThumbnailGrid *m_ThumbnailGrid;
        ThumbnailViewer *m_ThumbnailViewer;

        std::string m_FilePathOpen = "";
        std::string m_FilePathSave = "";
        std::string m_FileName = "";


        std::vector<std::pair<float, float>> m_LidarData;
        bool m_NewLidarData = false;

        bool isOpenCameraComandSent = false;

        int counter = 0;
        float sliderValue = 0;
        float lastsliderValue = 0;
        float angle = 0;
        float lastAngle = 0;
        float speed = 0;
        float steering = 0;
        int axesCount{};
        float xAxisRightLast = 0;
        float xAxisLeftLast = 0;
        float xAxisRightMaxValue = 0;
        float xAxisLeftMaxValue = 0;



    };
}


#endif //ATOM_APPLICATION_H
