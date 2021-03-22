#include <type_traits>
#include <nlohmann/json.hpp>
#include <Engine.hpp>
using json = nlohmann::json;
using namespace std;
using namespace glm;
using namespace Engine;
template <>
void Engine::serialize<Asset, true>(json &js, const Engine::Entity *entity) {
  const Asset *e = (const Asset *)entity;
  js.push_back(e->name);
}
template <>
void Engine::serialize<Component, true>(json &js, const Engine::Entity *entity) {
  const Component *e = (const Component *)entity;
  js.push_back(e->enabled);
  js.push_back(e->gameObject);
}
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
void Engine::serialize<AMaterial, true>(json &js, const Engine::Entity *entity) {
  const AMaterial *e = (const AMaterial *)entity;
  js.push_back(e->vertexShader);
  js.push_back(e->fragmentShader);
  js.push_back(e->mainTexture);
}
template <>
void Engine::serialize<AMesh, true>(json &js, const Engine::Entity *entity) {
  const AMesh *e = (const AMesh *)entity;
  js.push_back(e->amodel);
  js.push_back(e->index);
}
template <>
void Engine::serialize<AModel, true>(json &js, const Engine::Entity *entity) {
  const AModel *e = (const AModel *)entity;
  js.push_back(e->path);
}
template <>
void Engine::serialize<Renderer, true>(json &js, const Engine::Entity *entity) {
  const Renderer *e = (const Renderer *)entity;
  js.push_back(e->mesh);
  js.push_back(e->material);
}
template <>
void Engine::serialize<AShader, true>(json &js, const Engine::Entity *entity) {
  const AShader *e = (const AShader *)entity;
  js.push_back(e->path);
  js.push_back(e->shaderType);
}
template <>
void Engine::serialize<ATexture, true>(json &js, const Engine::Entity *entity) {
  const ATexture *e = (const ATexture *)entity;
  js.push_back(e->path);
}
template <>
void Engine::serialize<Group, true>(json &js, const Engine::Entity *entity) {
  const Group *e = (const Group *)entity;
  js.push_back(e->gameObjects);
  js.push_back(e->ibehaviors);
  js.push_back(e->irenders);
  js.push_back(e->idraws);
}
template <>
void Engine::serialize<ProjectSetting, true>(json &js, const Engine::Entity *entity) {
  const ProjectSetting *e = (const ProjectSetting *)entity;
  js.push_back(e->serial);
  js.push_back(e->startSceneName);
}
template <>
void Engine::serialize<SceneSetting, true>(json &js, const Engine::Entity *entity) {
  const SceneSetting *e = (const SceneSetting *)entity;
  js.push_back(e->mainCamera);
}
template <>
void Engine::serialize<Script, true>(json &js, const Engine::Entity *entity) {
  const Script *e = (const Script *)entity;
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
void Engine::deserialize<Asset, true>(const json &js, Engine::Entity *entity) {
  Asset *e = (Asset *)entity;
  e->name = js[0].get<std::string>();
}
template <>
void Engine::deserialize<Component, true>(const json &js, Engine::Entity *entity) {
  Component *e = (Component *)entity;
  e->enabled = js[0].get<bool>();
  e->gameObject = js[1].get<Engine::GameObject*>();
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
void Engine::deserialize<AMaterial, true>(const json &js, Engine::Entity *entity) {
  AMaterial *e = (AMaterial *)entity;
  e->vertexShader = js[0].get<Engine::AShader*>();
  e->fragmentShader = js[1].get<Engine::AShader*>();
  e->mainTexture = js[2].get<Engine::ATexture*>();
}
template <>
void Engine::deserialize<AMesh, true>(const json &js, Engine::Entity *entity) {
  AMesh *e = (AMesh *)entity;
  e->amodel = js[0].get<Engine::AModel*>();
  e->index = js[1].get<uint32_t>();
}
template <>
void Engine::deserialize<AModel, true>(const json &js, Engine::Entity *entity) {
  AModel *e = (AModel *)entity;
  e->path = js[0].get<std::string>();
}
template <>
void Engine::deserialize<Renderer, true>(const json &js, Engine::Entity *entity) {
  Renderer *e = (Renderer *)entity;
  e->mesh = js[0].get<std::shared_ptr<Mesh>>();
  e->material = js[1].get<std::shared_ptr<Material>>();
}
template <>
void Engine::deserialize<AShader, true>(const json &js, Engine::Entity *entity) {
  AShader *e = (AShader *)entity;
  e->path = js[0].get<std::string>();
  e->shaderType = js[1].get<uint16_t>();
}
template <>
void Engine::deserialize<ATexture, true>(const json &js, Engine::Entity *entity) {
  ATexture *e = (ATexture *)entity;
  e->path = js[0].get<std::string>();
}
template <>
void Engine::deserialize<Group, true>(const json &js, Engine::Entity *entity) {
  Group *e = (Group *)entity;
  e->gameObjects = js[0].get<std::vector<GameObject *>>();
  e->ibehaviors = js[1].get<std::vector<Component *>>();
  e->irenders = js[2].get<std::vector<Component *>>();
  e->idraws = js[3].get<std::vector<Component *>>();
}
template <>
void Engine::deserialize<ProjectSetting, true>(const json &js, Engine::Entity *entity) {
  ProjectSetting *e = (ProjectSetting *)entity;
  e->serial = js[0].get<uint64_t>();
  e->startSceneName = js[1].get<std::string>();
}
template <>
void Engine::deserialize<SceneSetting, true>(const json &js, Engine::Entity *entity) {
  SceneSetting *e = (SceneSetting *)entity;
  e->mainCamera = js[0].get<Engine::Camera*>();
}
template <>
void Engine::deserialize<Script, true>(const json &js, Engine::Entity *entity) {
  Script *e = (Script *)entity;
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
  Asset::StaticType(new Type("Asset", instantiate<Asset, true>, serialize<Asset, true>, deserialize<Asset, true>));
  Component::StaticType(new Type("Component", instantiate<Component, true>, serialize<Component, true>, deserialize<Component, true>));
  GameObject::StaticType(new Type("GameObject", instantiate<GameObject, true>, serialize<GameObject, true>, deserialize<GameObject, true>));
  Camera::StaticType(new Type("Camera", instantiate<Camera, true>, serialize<Camera, true>, deserialize<Camera, true>));
  AMaterial::StaticType(new Type("AMaterial", instantiate<AMaterial, true>, serialize<AMaterial, true>, deserialize<AMaterial, true>));
  AMesh::StaticType(new Type("AMesh", instantiate<AMesh, true>, serialize<AMesh, true>, deserialize<AMesh, true>));
  AModel::StaticType(new Type("AModel", instantiate<AModel, true>, serialize<AModel, true>, deserialize<AModel, true>));
  Renderer::StaticType(new Type("Renderer", instantiate<Renderer, true>, serialize<Renderer, true>, deserialize<Renderer, true>));
  AShader::StaticType(new Type("AShader", instantiate<AShader, true>, serialize<AShader, true>, deserialize<AShader, true>));
  ATexture::StaticType(new Type("ATexture", instantiate<ATexture, true>, serialize<ATexture, true>, deserialize<ATexture, true>));
  Group::StaticType(new Type("Group", instantiate<Group, true>, serialize<Group, true>, deserialize<Group, true>));
  ProjectSetting::StaticType(new Type("ProjectSetting", instantiate<ProjectSetting, true>, serialize<ProjectSetting, true>, deserialize<ProjectSetting, true>));
  SceneSetting::StaticType(new Type("SceneSetting", instantiate<SceneSetting, true>, serialize<SceneSetting, true>, deserialize<SceneSetting, true>));
  Script::StaticType(new Type("Script", instantiate<Script, true>, serialize<Script, true>, deserialize<Script, true>));
  Transform::StaticType(new Type("Transform", instantiate<Transform, true>, serialize<Transform, true>, deserialize<Transform, true>));
}
void type_clear() {
  delete Asset::StaticType();
  delete Component::StaticType();
  delete GameObject::StaticType();
  delete Camera::StaticType();
  delete AMaterial::StaticType();
  delete AMesh::StaticType();
  delete AModel::StaticType();
  delete Renderer::StaticType();
  delete AShader::StaticType();
  delete ATexture::StaticType();
  delete Group::StaticType();
  delete ProjectSetting::StaticType();
  delete SceneSetting::StaticType();
  delete Script::StaticType();
  delete Transform::StaticType();
}
