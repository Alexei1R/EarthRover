//
// Created by toor on 3/18/24.
//

#ifndef THUMBNAILVIEWER_H
#define THUMBNAILVIEWER_H


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

    private:
        std::vector<Thumbnail> m_Thumbnails;
        //Size Thumbnail
        glm::vec2 m_ThumbnailSize = {200, 250};
        float m_VerticalPadding = 0;
        int m_ThumbnailsPerRow;
        bool m_Hovered = false;
        bool m_Selected = false;
    };
}

#endif //THUMBNAILVIEWER_H
