//
// Created by toor on 3/18/24.
//

#ifndef THUMBNAILVIEWER_H
#define THUMBNAILVIEWER_H


#include <optional>

#include "ATOM/atompch.h"
#include "Networking/Server.h"
#include "ATOM/AI/TrainEngine.h"
#include "ATOM/Serialize/Serialize.h"


namespace Atom
{
    class ThumbnailViewer : public Layer
    {
    public:
    public:
        ThumbnailViewer();

        ~ThumbnailViewer();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

    private:
        void DrawThumbnail(Thumbnail &thumbnail , int index, bool &hovered, bool &selected);
        void ExecuteSystemCommandAsync(const std::string& command);
        void OpenImageInDefaultViewerAsync(const std::string& imagePath);

    private:
        std::mutex m_Mutex;

        std::vector<Thumbnail> m_Thumbnails;
        std::vector<Thumbnail> m_FilteredThumbnails;
        //Size Thumbnail
        glm::vec2 m_ThumbnailSize = {200, 250};
        float m_VerticalPadding = 0;
        int m_ThumbnailsPerRow;
        bool m_Hovered = false;
        bool m_Selected = false;

        bool m_SortByTimeBeginWithNewest = true;


        std::optional<std::string> m_CurrentlyOpenedImage;

        cv::Mat m_FrameOnHover;
        int m_HoveredIndex = -1;
        GLuint m_HoveredTexture = 0;

        float m_ThumbnailTreshold = 0.5f;
    };
}

#endif //THUMBNAILVIEWER_H
