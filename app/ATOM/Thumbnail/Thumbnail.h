//
// Created by toor on 3/17/24.
//

#ifndef ThumbnailGrid_H
#define ThumbnailGrid_H

#include "ATOM/atompch.h"
#include "Networking/Server.h"
#include "ATOM/AI/TrainEngine.h"
#include "ATOM/Serialize/Serialize.h"

namespace Atom {




    class ThumbnailGrid : public Layer {
    public:
    public:
        ThumbnailGrid(std::string path , TrainEngine *trainEngine);

        ~ThumbnailGrid();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

    private:
        // void DrawThumbnail(Thumbnail &thumbnail , int index, bool &hovered, bool &selected);

    private:
        TrainEngine *m_TrainEngine;
        std::vector<Thumbnail> m_Thumbnails;
        //Size Thumbnail
        glm::vec2 m_ThumbnailSize = {200, 250};
        float m_VerticalPadding = 0;
        int m_ThumbnailsPerRow ;
        bool m_Hovered = false;
        bool m_Selected = false;


        std::string m_Path;


    };
}




#endif //ThumbnailGrid_H
