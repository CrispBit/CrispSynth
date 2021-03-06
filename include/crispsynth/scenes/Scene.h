//
// Created by cilan on 3/15/2017.
//

#ifndef I_AM_SYNTHETIC_C_SFML_SCENE_H
#define I_AM_SYNTHETIC_C_SFML_SCENE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../buttons/Button.h"
#include "../game-objects/GameObject.h"
#include "../Game.h"

class Scene {
public:
    Scene(Game &game);
    virtual void updateObjects(sf::Event &event);
    virtual void renderObjects(float delta);
    virtual void onEnter() {};
protected:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    Game &game;
};

#endif
