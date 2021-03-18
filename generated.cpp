inline void serialize(const foo::serializer& s, const GameObject& obj) {
  s.serialize(obj.id);
  serialize(s, obj.name);
}