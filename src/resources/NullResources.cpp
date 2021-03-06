//
// Created by cilan on 1/18/2017.
//

#include "crispsynth/resources/NullResources.h"
#include "crispsynth/mesh/StaticMesh.h"
#include "crispsynth/mesh/BonedMesh.h"

sf::Texture NullResources::loadTexture(std::string state, std::string id, sf::IntRect* rect) {
    // load texture and return thing
    std::cout << "null texture being loaded named " << id << " during " << state << " state" << " with rect width "
              << rect->width << " and rect height " << rect->height << std::endl;
    return sf::Texture();
}

std::unique_ptr<BonedMesh> NullResources::loadBonedMesh(std::string group, std::string id,
                                                        std::map<std::string, Texture>& textures) {
    // load boned mesh and return thing
    std::cout << "null boned mesh loaded named " << id << " from " << group << " group" << std::endl;
    return std::make_unique<BonedMesh>();
}

std::unique_ptr<StaticMesh> NullResources::loadStaticMesh(std::string group, std::string id,
                                                          std::map<std::string, Texture>& textures) {
    // load boned mesh and return thing
    std::cout << "null boned mesh loaded named " << id << " from " << group << " group" << std::endl;
    return std::make_unique<StaticMesh>();
}

std::unique_ptr<sf::Music> NullResources::loadMusic(std::string state, std::string id) {
    // load music and return thing
    std::cout << "null music being loaded named " << id << " during " << state << " state" << std::endl;
    return std::make_unique<sf::Music>();
}

std::string NullResources::loadPath(std::string id) {
    // returns YAML path
    std::cout << "null path being loaded of file named " << id << std::endl;
    return "";
}

sf::Font NullResources::loadFont(std::string id) {
    // load font and return thing
    std::cout << "null font being loaded named " << id << std::endl;
    return sf::Font();
}

sf::Image NullResources::loadImage(std::string state, std::string id) {
    // load image and return thing
    std::cout << "null image being loaded named " << id << std::endl;
    return sf::Image();
}

BoxCollection NullResources::loadBoxes(std::string group, std::string id) {
    // load box collection and return thing
    std::cout << "null box collection being loaded named " << id << std::endl;
    return BoxCollection();
}
