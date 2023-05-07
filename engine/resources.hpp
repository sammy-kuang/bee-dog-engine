#include <map>
#include <iostream>
#include "raylib.h"

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

// i don't really like how entt handles resource handles
// so im going to deploy my own, which is probably less efficient

template<typename T>
class ResourceCache {
public:
	virtual T fetch_resource_from_rl(const char* path) {
		return T();
	}

	T load_resource(const char* path) {
		if (cache.find(path) == cache.end()) {
			cache[path] = fetch_resource_from_rl(path);
		}
		return cache[path];
	}
private:
	std::map<const char*, T> cache;
};

class TextureCache : public ResourceCache<Texture2D> {
public:
	Texture2D fetch_resource_from_rl(const char* path) {
		return LoadTexture(path);
	}
};


#endif