//
// Created by toor on 2/16/24.
//

#ifndef TRAINENGINE_H
#define TRAINENGINE_H

#include "ATOM/atompch.h"
#include "YoloV8.h"
#include <time.h>


namespace Atom {
    class TrainEngine : public Layer {
    public:
        TrainEngine();

        ~TrainEngine();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        std::vector<Thumbnail> &GetThumbnails() { return m_Thumbnails; }

        void OpenVideoCapture();

        void Shutdown();



    private:
        std::vector<Thumbnail> m_Thumbnails;
        const double m_MinDetectionDuration = 1.0;

        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime =
                std::chrono::high_resolution_clock::now();

        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);

#ifdef NDEBUG



        YoloV8Config config;
        YoloV8* m_Model;
        cv::Mat *m_Frame;
        cv::Mat m_LocalFrame;
        GLuint m_Texture;
        bool isTextureGenerated = false;
        float m_AspectRatio = 1.0f;
#endif

    };







}



#endif //TRAINENGINE_H
