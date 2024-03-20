//
// Created by toor on 11/7/23.
//

#ifndef ATOM_ATOMPCH_H
#define ATOM_ATOMPCH_H
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <cctype>

#include "ATOM/Core/Editor/stb_image.h"

#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"


#include "glm/glm.hpp"

#include "ATOM/Core/macros.h"
#include "ATOM/Core/Loging/Log.h"


#include "imgui.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#ifdef _WIN32
#include <windows.h> // Ug, for NukeProcess -- see below
#else
#include <unistd.h>
#include <signal.h>
#endif

#include "opencv4/opencv2/opencv.hpp"


typedef struct Thumbnail
{
    //Image
    std::string ImagePath;
    //UUID
    std::string UUID;
    //Class
    std::string Class;
    //Treshold
    std::string Treshold;
    //Time
    std::string Time;
    //Position
    glm::vec2 Position;
    //Size
    glm::vec2 Size;
    //Frame
    cv::Mat m_Frame;
    //Texture
    GLuint m_Texture;
    //Bool IS Texture Generated
    bool isTextureGenerated = false;
} Thumbnail;






#endif //ATOM_ATOMPCH_H
