#pragma once
#include <unordered_map>
#include <tuple>
#include <vector>
#include "components.hpp"

const int SPATIAL_HASH_SIZE = 1000;

// https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-tuples-in-c/
// A hashing function involving a simple XOR
struct HashIntTuple
{
    size_t operator()(const std::tuple<int, int> &t) const;
};

class SpatialHash
{
public:
    std::unordered_map<std::tuple<int, int>, std::vector<entt::entity> *, HashIntTuple> map;
};


int round_map_size(int a);
std::tuple<int, int> pos_to_tuple(int x, int y);
std::vector<entt::entity> *get_spatial_map(entt::registry &registry, std::tuple<int, int> position_key);
void remove_from_spatial_map(entt::registry &registry, entt::entity entity, std::tuple<int, int> position_key);
void add_to_spatial_map(entt::registry &registry, entt::entity entity, std::tuple<int, int> position_key);
void update_global_spatial_maps(entt::registry &registry);
std::vector<entt::entity> get_surrounding_maps(entt::registry &registry, std::tuple<int, int> position_key);