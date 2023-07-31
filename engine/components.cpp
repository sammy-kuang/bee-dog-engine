#include "components.hpp"
#include "spatial_hash.hpp"

void BoxCollider::move(int x, int y)
{
    auto nx = x - box.width / 2;
    auto ny = y - box.height / 2;
    box.x = nx;
    box.y = ny;
}

void BoxCollider::rebase_on_sprite(entt::registry &registry, entt::entity entity)
{
    auto sprite = registry.try_get<Sprite>(entity);

    if (sprite != nullptr)
    {
        auto tex = registry.ctx().get<TextureCache>().load_resource(sprite->path);
        box.width = (float)tex.width;
        box.height = (float)tex.height;
    }
}

Rectangle BoxCollider::create_x_box(int nx, int ny)
{
    return Rectangle{(float)(nx - box.width / 2), (float)(ny + COLLISION_THRESHOLD - box.height / 2), box.width, box.height - COLLISION_THRESHOLD * 2};
}

Rectangle BoxCollider::create_y_box(int nx, int ny)
{
    return Rectangle{(float)(nx + COLLISION_THRESHOLD - box.width / 2), (float)(ny - box.height / 2), box.width - COLLISION_THRESHOLD * 2, box.height};
}

void BoxArea::move(int x, int y)
{
    box.x = x - (box.width + get_margin()) / 2;
    box.y = y - (box.height + get_margin()) / 2;
}

float BoxArea::get_margin()
{
    return COLLISION_THRESHOLD * 1.6f;
}

void BoxArea::rebase_on_sprite(entt::registry &registry, entt::entity entity)
{
    auto sprite = registry.try_get<Sprite>(entity);

    if (sprite != nullptr)
    {
        auto tex = registry.ctx().get<TextureCache>().load_resource(sprite->path);
        box.width = (float)tex.width;
        box.height = (float)tex.height;
    }
}

Rectangle BoxArea::create_area_rectangle()
{

    return Rectangle{box.x, box.y, box.width + get_margin(), box.height + get_margin()};
}

vector<entt::entity> BoxArea::get_colliding_entities(entt::registry &registry)
{
    vector<entt::entity> list;

    auto view = registry.view<BoxCollider>();

    for (auto &entity : view)
    {
        if (&registry.get<BoxArea>(entity) == this)
            continue;

        BoxCollider &b = registry.get<BoxCollider>(entity);
        auto b_box = b.box;

        if (CheckCollisionRecs(create_area_rectangle(), b_box))
            list.push_back(entity);
    }

    return list;
}
