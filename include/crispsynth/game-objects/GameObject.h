//
// Created by cilan on 3/18/2017.
//

#ifndef I_AM_SYNTHETIC_C_SFML_GAMEOBJECT_H
#define I_AM_SYNTHETIC_C_SFML_GAMEOBJECT_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "components/Component.h"
#include "../Game.h"

class GameObject {
protected:
    std::vector<std::shared_ptr<Component>> components;
    float _x, _y, _z, _sX = 1, _sY = 1, _sZ = 1, width, height;
    virtual void update() {};
public:
    float &x = _x, &y = _y, &z = _z, &w = width, &h = height, &sX = _sX, &sY = _sY, &sZ = _sZ;
    virtual void update(Game &game, sf::Event& event) = 0;
    virtual void render(Game &game, float delta) {};
    virtual void updatePosition(float tx, float ty) {};
    virtual void updatePosition(float tx, float ty, float tz) {};
};

#endif
