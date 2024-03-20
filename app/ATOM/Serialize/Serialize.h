//
// Created by toor on 3/18/24.
//

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "ATOM/atompch.h"
#include "yaml-cpp/yaml.h"

class Serialize
{
public:
    Serialize();
    static void SerializeData(std::string path, const std::vector<Thumbnail>& thumbnails);
    static std::vector<Thumbnail> DeSerializeData(std::string path);

private:
};


#endif //SERIALIZE_H
