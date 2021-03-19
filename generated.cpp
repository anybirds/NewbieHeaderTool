#include <type_traits>
#include <nlohmann/json.hpp>
#include <User.hpp>
using json = nlohmann::json;
namespace User {
template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>
Entity *instantiate() {
  return new T();
}
}
namespace User {
template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>
void serialize(json &js, const Entity *entity);
}
namespace User {
template <>
void serialize<Error, true>(json &js, Entity *entity) {
  const Error *e = (Error *)entity;
  to_json(js, e->id);
  to_json(js, e->name);
}
}
namespace User {
template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>
void deserialize(const json &js, Entity *entity);
}
}
namespace User {
template <>
void deserialize<Error, true>(const json &js, Entity *entity) {
  Error *e = (Error *)entity;
  from_json(js, e->id);
  from_json(js, e->name);
}
}
void type_init() {
  using namespace User;
  Error::type = new Engine::Type(instantiate<Error, true>, serialize<Error, true>, deserialize<Error, true>);
}
