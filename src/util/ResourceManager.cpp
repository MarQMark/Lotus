#include "util/ResourceManager.h"

std::map<uint64_t, Resource*> ResourceManager::s_textures;

std::map<std::string, uint32_t> ResourceManager::s_resource_ids;
uint32_t ResourceManager::s_last_id = 0;

Kikan::Texture2D* Resource::_null_tex = nullptr;
