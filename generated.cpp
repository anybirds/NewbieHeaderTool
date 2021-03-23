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
  js.push_back(e->serial);
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
  serialize<Component, true>(js, entity);
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
  serialize<Asset, true>(js, entity);
  const AMaterial *e = (const AMaterial *)entity;
  js.push_back(e->vertexShader);
  js.push_back(e->fragmentShader);
  js.push_back(e->mainTexture);
}
template <>
void Engine::serialize<AMesh, true>(json &js, const Engine::Entity *entity) {
  serialize<Asset, true>(js, entity);
  const AMesh *e = (const AMesh *)entity;
  js.push_back(e->amodel);
  js.push_back(e->index);
}
template <>
void Engine::serialize<AModel, true>(json &js, const Engine::Entity *entity) {
  serialize<Asset, true>(js, entity);
  const AModel *e = (const AModel *)entity;
  js.push_back(e->path);
}
template <>
void Engine::serialize<Renderer, true>(json &js, const Engine::Entity *entity) {
  serialize<Component, true>(js, entity);
  const Renderer *e = (const Renderer *)entity;
  js.push_back(e->mesh);
  js.push_back(e->material);
}
template <>
void Engine::serialize<AShader, true>(json &js, const Engine::Entity *entity) {
  serialize<Asset, true>(js, entity);
  const AShader *e = (const AShader *)entity;
  js.push_back(e->path);
  js.push_back(e->shaderType);
}
template <>
void Engine::serialize<ATexture, true>(json &js, const Engine::Entity *entity) {
  serialize<Asset, true>(js, entity);
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
  serialize<Component, true>(js, entity);
  const Script *e = (const Script *)entity;
}
template <>
void Engine::serialize<Transform, true>(json &js, const Engine::Entity *entity) {
  serialize<Component, true>(js, entity);
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
void Engine::deserialize<Asset, true>(json &js, Engine::Entity *entity) {
  Asset *e = (Asset *)entity;
  e->name = js.back().get<std::string>(); js.erase(--js.end());
  e->serial = js.back().get<uint64_t>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<Component, true>(json &js, Engine::Entity *entity) {
  Component *e = (Component *)entity;
  e->gameObject = js.back().get<Engine::GameObject*>(); js.erase(--js.end());
  e->enabled = js.back().get<bool>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<GameObject, true>(json &js, Engine::Entity *entity) {
  GameObject *e = (GameObject *)entity;
  e->components = js.back().get<std::vector<Component *>>(); js.erase(--js.end());
  e->transform = js.back().get<Engine::Transform*>(); js.erase(--js.end());
  e->group = js.back().get<Engine::Group*>(); js.erase(--js.end());
  e->name = js.back().get<std::string>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<Camera, true>(json &js, Engine::Entity *entity) {
  Camera *e = (Camera *)entity;
  e->top = js.back().get<float>(); js.erase(--js.end());
  e->bottom = js.back().get<float>(); js.erase(--js.end());
  e->right = js.back().get<float>(); js.erase(--js.end());
  e->left = js.back().get<float>(); js.erase(--js.end());
  e->fr = js.back().get<float>(); js.erase(--js.end());
  e->nr = js.back().get<float>(); js.erase(--js.end());
  e->height = js.back().get<float>(); js.erase(--js.end());
  e->width = js.back().get<float>(); js.erase(--js.end());
  e->fovy = js.back().get<float>(); js.erase(--js.end());
  e->orthographic = js.back().get<bool>(); js.erase(--js.end());
  e->normalization = js.back().get<glm::mat4>(); js.erase(--js.end());
  deserialize<Component, true>(js, entity);
}
template <>
void Engine::deserialize<AMaterial, true>(json &js, Engine::Entity *entity) {
  AMaterial *e = (AMaterial *)entity;
  e->mainTexture = js.back().get<Engine::ATexture*>(); js.erase(--js.end());
  e->fragmentShader = js.back().get<Engine::AShader*>(); js.erase(--js.end());
  e->vertexShader = js.back().get<Engine::AShader*>(); js.erase(--js.end());
  deserialize<Asset, true>(js, entity);
}
template <>
void Engine::deserialize<AMesh, true>(json &js, Engine::Entity *entity) {
  AMesh *e = (AMesh *)entity;
  e->index = js.back().get<uint32_t>(); js.erase(--js.end());
  e->amodel = js.back().get<Engine::AModel*>(); js.erase(--js.end());
  deserialize<Asset, true>(js, entity);
}
template <>
void Engine::deserialize<AModel, true>(json &js, Engine::Entity *entity) {
  AModel *e = (AModel *)entity;
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  deserialize<Asset, true>(js, entity);
}
template <>
void Engine::deserialize<Renderer, true>(json &js, Engine::Entity *entity) {
  Renderer *e = (Renderer *)entity;
  e->material = js.back().get<std::shared_ptr<Material>>(); js.erase(--js.end());
  e->mesh = js.back().get<std::shared_ptr<Mesh>>(); js.erase(--js.end());
  deserialize<Component, true>(js, entity);
}
template <>
void Engine::deserialize<AShader, true>(json &js, Engine::Entity *entity) {
  AShader *e = (AShader *)entity;
  e->shaderType = js.back().get<uint16_t>(); js.erase(--js.end());
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  deserialize<Asset, true>(js, entity);
}
template <>
void Engine::deserialize<ATexture, true>(json &js, Engine::Entity *entity) {
  ATexture *e = (ATexture *)entity;
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  deserialize<Asset, true>(js, entity);
}
template <>
void Engine::deserialize<Group, true>(json &js, Engine::Entity *entity) {
  Group *e = (Group *)entity;
  e->idraws = js.back().get<std::vector<IDraw *>>(); js.erase(--js.end());
  e->irenders = js.back().get<std::vector<IRender *>>(); js.erase(--js.end());
  e->ibehaviors = js.back().get<std::vector<IBehavior *>>(); js.erase(--js.end());
  e->gameObjects = js.back().get<std::vector<GameObject *>>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<ProjectSetting, true>(json &js, Engine::Entity *entity) {
  ProjectSetting *e = (ProjectSetting *)entity;
  e->startSceneName = js.back().get<std::string>(); js.erase(--js.end());
  e->serial = js.back().get<uint64_t>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<SceneSetting, true>(json &js, Engine::Entity *entity) {
  SceneSetting *e = (SceneSetting *)entity;
  e->mainCamera = js.back().get<Engine::Camera*>(); js.erase(--js.end());
}
template <>
void Engine::deserialize<Script, true>(json &js, Engine::Entity *entity) {
  Script *e = (Script *)entity;
  deserialize<Component, true>(js, entity);
}
template <>
void Engine::deserialize<Transform, true>(json &js, Engine::Entity *entity) {
  Transform *e = (Transform *)entity;
  e->localToWorldMatrix = js.back().get<glm::mat4>(); js.erase(--js.end());
  e->updated = js.back().get<bool>(); js.erase(--js.end());
  e->children = js.back().get<std::vector<Transform *>>(); js.erase(--js.end());
  e->parent = js.back().get<Engine::Transform*>(); js.erase(--js.end());
  e->localEulerAngles = js.back().get<glm::vec3>(); js.erase(--js.end());
  e->localScale = js.back().get<glm::vec3>(); js.erase(--js.end());
  e->localRotation = js.back().get<glm::quat>(); js.erase(--js.end());
  e->localPosition = js.back().get<glm::vec3>(); js.erase(--js.end());
  deserialize<Component, true>(js, entity);
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
