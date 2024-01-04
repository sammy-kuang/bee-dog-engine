#include "prefabs.hpp"
#include "systems.hpp"
#include "raymath.h"

// Create an entity with a transform, sprite, box collider, box area, and a velocity.
entt::entity create_sprite_entity(entt::registry &registry, const char* resource_name, float x , float y , float z)
{
    auto entity = registry.create();
    registry.emplace<BDTransform>(entity, x, y, z);
    registry.emplace<Sprite>(entity, resource_name);
    auto tex = registry.ctx().get<TextureCache>().load_resource(resource_name);
    registry.emplace<Velocity>(entity);
    registry.emplace<BoxCollider>(entity, Rectangle{(float)(x - tex->width / 2), (float)(y - tex->height / 2), (float)tex->width, (float)tex->height});
    registry.emplace<BoxArea>(entity, Rectangle{(float)(x - tex->width / 2), (float)(y - tex->height / 2), (float)tex->width, (float)tex->height});
    auto name = std::to_string(registry.alive()+1);
    registry.emplace<Name>(entity, name);
    sort_sprites_by_z(registry);
    return entity;
}

// Create and add the camera to the registry's context
Camera2D& add_camera(entt::registry &registry) {
    registry.ctx().emplace<Camera2D>(Vector2{0, 0}, Vector2{0, 0}, 0.f, 1.f);
    return registry.ctx().get<Camera2D>();
}

void spawn_particle_system(entt::registry &registry, ParticleSystem part_system, Vector2 position) {
    part_system.direction = Vector2Normalize(part_system.direction);
    Vector2 left_bound = Vector2Rotate(part_system.direction, -part_system.spread);
    Vector2 right_bound = Vector2Rotate(part_system.direction, part_system.spread);

    for (int i = 0; i < part_system.count; i++) {
        SetRandomSeed((unsigned int)(i * GetTime() * 100 * GetFPS()));
        float lerp = GetRandomValue(0, 100) / 100.f;
        Vector2 rand_direction = Vector2Lerp(left_bound, right_bound, lerp);
        Vector2 velocity = Vector2Scale(rand_direction, part_system.velocity + (part_system.velocity / 10) * lerp);
        entt::entity p = registry.create();
        registry.emplace<BDTransform>(p, position.x, position.y);
        registry.emplace<Velocity>(p, velocity.x, velocity.y);
        registry.emplace<Particle>(p, part_system.draw, part_system.lifetime);
    }
}

void draw_red_circle(int x, int y) {
    DrawCircle(x, y, 2, RED);
}