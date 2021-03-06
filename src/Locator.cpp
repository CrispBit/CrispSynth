//
// Created by cilan on 1/20/2017.
//

#include "crispsynth/Locator.h"

std::shared_ptr<IResources> Locator::nullResourcesService_ = std::make_shared<NullResources>();
std::shared_ptr<IResources> Locator::resourcesService_ = Locator::nullResourcesService_;
std::unique_ptr<sf::Music> Locator::music = std::make_unique<sf::Music>();

YAML::Node Locator::defaultConfig;
YAML::Node Locator::currentConfig;

boost::filesystem::path Locator::rootPath;

Locator::Locator() {
    Locator::resourcesService_.reset();
}

void Locator::provideResourcesService(std::shared_ptr<IResources> service) {
    resourcesService_.reset();
    if (service == nullptr) {
        resourcesService_ = nullResourcesService_;
    } else {
        resourcesService_ = std::move(service);
    }
}

void Locator::provideArgs(char arg[]) {
    Locator::rootPath = boost::filesystem::canonical(arg).remove_filename().parent_path();
    if (!rootPath.has_filename()) std::cout << "TO-DO" << std::endl;
}

void Locator::provideConfig(std::string path, Configs config) {
    switch (config) {
        case Configs::DEFAULT:
            Locator::defaultConfig = YAML::LoadFile(path); // TODO: work for other config files
            break;
        case Configs::CURRENT:
            Locator::currentConfig = YAML::LoadFile(path);
            break;
    }
}

void Locator::provideMusic(std::unique_ptr<sf::Music> copyMusic) {
    Locator::music = std::move(copyMusic);
}

std::unique_ptr<sf::Music>& Locator::getMusic() {
    return Locator::music;
}
