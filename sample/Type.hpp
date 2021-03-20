#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

#define TYPE(T) \
    public: \
    static Type *type; \
    virtual Type *GetType() const override { return T::type; } \
    private: \
    template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool>> \
    friend void serialize(nlohmann::json &, const Entity *); \
    template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool>> \
    friend void deserialize(const nlohmann::json &, Entity *);

/* type_init function that creates Type object for all types. */
void type_init();

/* serializing and deserializing possible for glm vector and matrix types */
namespace glm {
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec3 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec4 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat3 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat4 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::quat &q);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec3 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec4 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat3 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat4 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::quat &q);
}

namespace Engine {
    
    class Entity;

    /* instantiate function that creates the object of the type. */
    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
    Engine::Entity *instantiate() {
    return new T();
    }

    /* serialize function that serializes the object. */
    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
    void serialize(nlohmann::json &js, const Entity *entity);

    /* deserialize function that deserializes the object. */
    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
    void deserialize(const nlohmann::json &js, Entity *entity);

    /*
    Type information needed for object serialization and deserialization.
    */
    class ENGINE_EXPORT Type final {
    public:
        typedef Entity *(*FInstantiate)();
        typedef void (*FSerialize)(nlohmann::json &, const Entity *);
        typedef void (*FDeserialize)(const nlohmann::json &, Entity *);

    private:
        static std::unordered_map<std::string, const Type *> types;

    public:
        static const Type *GetType(const std::string &name) {
            auto it = types.find(name);
            if (it == types.end()) {
                return nullptr;
            }
            return it->second;
        }

    private:
        std::string name;
        FInstantiate instantiate;
        FSerialize serialize;
        FDeserialize deserialize;

    public:
        Type(const std::string &name, FInstantiate instantiate, FSerialize serialize, FDeserialize deserialize);
        ~Type();

        const std::string &GetName() const { return name; }
        Entity *Instantiate() const { return instantiate(); }
        void Serialize(nlohmann::json &js, const Entity *entity) const { serialize(js, entity); }
        void Deserialize(const nlohmann::json &js, Entity *entity) const { deserialize(js, entity); }
    };
}