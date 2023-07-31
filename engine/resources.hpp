#include <map>
#include <iostream>
#include <string>
#include "raylib.h"
#include "entt/entt.hpp"


#ifndef RESOURCES_HPP
#define RESOURCES_HPP

// i don't really like how entt handles resource handles
// so im going to deploy my own, which is probably less efficient

std::string get_asset_path(std::string resource_name) ;

template<typename T>
class ResourceCache {
public:
	virtual T fetch_resource_from_rl(std::string resource_name) {
		return T();
	}

	T load_resource(std::string resource_name) {
		if (cache.find(resource_name) == cache.end()) {
			cache[resource_name] = fetch_resource_from_rl(resource_name);
		}
		return cache[resource_name];
	}
private:
	std::map<std::string, T> cache;
};

class TextureCache : public ResourceCache<Texture2D> {
public:
	Texture2D fetch_resource_from_rl(std::string resource_name) {
		return LoadTexture(get_asset_path(resource_name).c_str());
	}
};



#endif