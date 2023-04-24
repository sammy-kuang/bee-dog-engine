#include "raylib.h"

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

struct BDTransform {
    float x;
    float y;
    float rotation;
};

struct Sprite {
    Texture2D texture;
};

#endif