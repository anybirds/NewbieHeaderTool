#include <cstdint>

#include <Entity.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

unordered_map<uint64_t, Entity *> Entity::temp;
void Entity::Serialize(json &js, const Entity *entity) {}
void Entity::Deserialize(json &js, Entity *entity) {}