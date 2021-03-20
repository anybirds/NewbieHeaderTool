#include <type_traits>
#include <nlohmann/json.hpp>
#include <Engine.hpp>
using json = nlohmann::json;
using namespace std;
using namespace glm;
using namespace Engine;
template <>
void Engine::serialize<GameObject, true>(json &js, const Engine::Entity *entity) {
  const GameObject *e = (const GameObject *)entity;
  js.push_back(e->name);
  js.push_back(e->group);
  js.push_back(e->transform);
  js.push_back(e->components);
}
template <>
void Engine::serialize<Camera, true>(json &js, const Engine::Entity *entity) {
  const Camera *e = (const Camera *)entity;
  js.push_back(e->normalization);
  js.push_back(e->orthographic);
  js.push_back(e->fovy);
  js.push_back(e->width);
  js.push_back(e->height);
  js.push_back(e->nr);
  js.push_back(e->fr);
  js.push_back(e->left);
  js.push_back(e->right);
  js.push_back(e->bottom);
  js.push_back(e->top);
}
template <>
void Engine::serialize<Transform, true>(json &js, const Engine::Entity *entity) {
  const Transform *e = (const Transform *)entity;
  js.push_back(e->localPosition);
  js.push_back(e->localRotation);
  js.push_back(e->localScale);
  js.push_back(e->localEulerAngles);
  js.push_back(e->parent);
  js.push_back(e->children);
  js.push_back(e->updated);
  js.push_back(e->localToWorldMatrix);
}
template <>
void Engine::deserialize<GameObject, true>(const json &js, Engine::Entity *entity) {
  GameObject *e = (GameObject *)entity;
  e->name = js[0].get<std::string>();
  e->group = js[1].get<Engine::Group*>();
  e->transform = js[2].get<Engine::Transform*>();
  e->components = js[3].get<std::vector<Component *>>();
}
template <>
void Engine::deserialize<Camera, true>(const json &js, Engine::Entity *entity) {
  Camera *e = (Camera *)entity;
  e->normalization = js[0].get<glm::mat4>();
  e->orthographic = js[1].get<bool>();
  e->fovy = js[2].get<float>();
  e->width = js[3].get<float>();
  e->height = js[4].get<float>();
  e->nr = js[5].get<float>();
  e->fr = js[6].get<float>();
  e->left = js[7].get<float>();
  e->right = js[8].get<float>();
  e->bottom = js[9].get<float>();
  e->top = js[10].get<float>();
}
template <>
void Engine::deserialize<Transform, true>(const json &js, Engine::Entity *entity) {
  Transform *e = (Transform *)entity;
  e->localPosition = js[0].get<glm::vec3>();
  e->localRotation = js[1].get<glm::quat>();
  e->localScale = js[2].get<glm::vec3>();
  e->localEulerAngles = js[3].get<glm::vec3>();
  e->parent = js[4].get<Engine::Transform*>();
  e->children = js[5].get<std::vector<Transform *>>();
  e->updated = js[6].get<bool>();
  e->localToWorldMatrix = js[7].get<glm::mat4>();
}
void type_init() {
  using namespace Engine;
  GameObject::type = new Type("GameObject", instantiate<GameObject, true>, serialize<GameObject, true>, deserialize<GameObject, true>);
  Camera::type = new Type("Camera", instantiate<Camera, true>, serialize<Camera, true>, deserialize<Camera, true>);
  Transform::type = new Type("Transform", instantiate<Transform, true>, serialize<Transform, true>, deserialize<Transform, true>);
}
