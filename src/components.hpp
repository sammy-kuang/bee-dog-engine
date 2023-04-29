#include "raylib.h"

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

struct BDTransform {
    float x = 0;
    float y = 0;
    float z = 0;
    float rotation = 0;
    float scale = 1;
};

struct Velocity {
    float x = 0;
    float y = 0;
    bool cancel_x = false;
    bool cancel_y = false;
};

static int COLLISION_THRESHOLD = 5;

struct BoxCollider {
    Rectangle box;

    void move(int x, int y) {
        box.x = x - box.width / 2;
        box.y = y - box.height / 2;
    }

    Rectangle create_x_box(int nx, int ny) {
        return Rectangle{(float)(nx - box.width / 2), (float)(ny + COLLISION_THRESHOLD - box.height / 2), box.width, box.height - COLLISION_THRESHOLD * 2};
    }

    Rectangle create_y_box(int nx, int ny) {
        return Rectangle{(float)(nx+COLLISION_THRESHOLD - box.width / 2), (float)(ny - box.height / 2), box.width - COLLISION_THRESHOLD * 2, box.height};
    }
};

struct Player {
    int id;
};

struct Sprite {
    Texture2D texture;
};

struct Floor {
    int id = 0;
};

#endif