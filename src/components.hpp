#include "raylib.h"

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

struct BDTransform {
    float x = 0;
    float y = 0;
    float rotation = 0;
    float scale = 1;
};

struct Velocity {
    float x;
    float y;
    bool cancel_tick = false; // should we cancel the current apply_velocity?
};

struct BoxCollider {
    Rectangle box;
};

struct Player {
    int id;
};

struct Sprite {
    Texture2D texture;
};

#endif