//
// Created by toor on 3/18/24.
//

#include "Serialize.h"


#include "Serialize.h"

Serialize::Serialize() {}




void Serialize::SerializeData(std::string path, const std::vector<Thumbnail>& thumbnails) {
    YAML::Emitter out;
    out << YAML::BeginSeq;
    for (const auto& thumbnail : thumbnails) {
        out << YAML::BeginMap;
        out << YAML::Key << "ImagePath" << YAML::Value << thumbnail.ImagePath;
        out << YAML::Key << "UUID" << YAML::Value << thumbnail.UUID;
        out << YAML::Key << "Class" << YAML::Value << thumbnail.Class;
        out << YAML::Key << "Treshold" << YAML::Value << thumbnail.Treshold;
        out << YAML::Key << "Time" << YAML::Value << thumbnail.Time;
        out << YAML::Key << "Position" << YAML::Flow << YAML::BeginSeq << thumbnail.Position.x << thumbnail.Position.y << YAML::EndSeq;
        out << YAML::Key << "Size" << YAML::Flow << YAML::BeginSeq << thumbnail.Size.x << thumbnail.Size.y << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    std::ofstream fout(path);
    fout << out.c_str();
}

std::vector<Thumbnail> Serialize::DeSerializeData(std::string path) {
    std::vector<Thumbnail> thumbnails;
    YAML::Node node = YAML::LoadFile(path);
    for (const auto& thumbnailNode : node) {
        Thumbnail thumbnail;
        thumbnail.ImagePath = thumbnailNode["ImagePath"].as<std::string>();
        thumbnail.UUID = thumbnailNode["UUID"].as<std::string>();
        thumbnail.Class = thumbnailNode["Class"].as<std::string>();
        thumbnail.Treshold = thumbnailNode["Treshold"].as<std::string>();
        thumbnail.Time = thumbnailNode["Time"].as<std::string>();
        YAML::Node positionNode = thumbnailNode["Position"];
        thumbnail.Position.x = positionNode[0].as<float>();
        thumbnail.Position.y = positionNode[1].as<float>();
        YAML::Node sizeNode = thumbnailNode["Size"];
        thumbnail.Size.x = sizeNode[0].as<float>();
        thumbnail.Size.y = sizeNode[1].as<float>();
        thumbnails.push_back(thumbnail);
    }
    return thumbnails;
}
