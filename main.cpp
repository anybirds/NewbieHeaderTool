#include <iostream>
#include <fstream>
#include <functional>

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_type.hpp>
#include <cppast/visitor.hpp>

#include <nlohmann/json.hpp>

#include "example_parser.hpp"

// generate a serialization call for a member
void generate_serialize_member(std::ostream& out, const cppast::cpp_member_variable& member)
{
    // auto& type = cppast::remove_cv(member.type());

    if (!cppast::has_attribute(member, "NoSerialize")) {
        out << "  to_json(js, e->" << member.name() << ");\n";
    }
}

// generate a deserialization call for a member
void generate_deserialize_member(std::ostream& out, const cppast::cpp_member_variable& member)
{
    // auto& type = cppast::remove_cv(member.type());

    if (!cppast::has_attribute(member, "NoSerialize")) {
        out << "  from_json(js, e->" << member.name() << ");\n";
    }
}

// generate serialization function
void generate_serialize(const cppast::cpp_file& file, const std::string &name)
{
    /* include headers */ 
    std::cout << "#include <type_traits>\n";
    std::cout << "#include <nlohmann/json.hpp>\n";
    std::cout << "#include <" << name << ".hpp>\n";
    std::cout << "using json = nlohmann::json;\n";
    
    /* instantiate function that creates the object of the type. */
    std::cout << "namespace " << name << " {\n";
    std::cout << "template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>\n";
    std::cout << "Engine::Entity *instantiate() {\n";
    std::cout << "  return new T();\n}\n}\n";
    
    /* serialize function that serializes the object. */
    std::cout << "namespace " << name << " {\n";
    std::cout << "template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>\n";
    std::cout << "void serialize(json &js, const Engine::Entity *entity);\n}\n";
    cppast::visit(file,
        [&name](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    && e.parent().has_value() && e.parent().value().name() == name
                    // or the module namespace
                    || e.kind() == cppast::cpp_entity_kind::namespace_t
                    && e.name() == name;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                std::cout << "template <>\n";
                std::cout << "void serialize<" << class_.name() << ", true>(json &js, Engine::Entity *entity) {\n";

                // serialize base classes
                for (auto& base : class_.bases()) {
                    std::cout << " serialize<" << base.name() << ", true>(js, entity);\n";
                }

                std::cout << "  const " << class_.name() << " *e = (" << class_.name() << " *)entity;\n";                
                
                // serialize member variables
                for (auto& member : class_)
                {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t)
                        generate_serialize_member(std::cout,
                                                static_cast<
                                                    const cppast::cpp_member_variable&>(
                                                    member));
                }

                std::cout << "}\n";
            }
            else if (e.kind() == cppast::cpp_entity_kind::namespace_t)
            {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    // open namespace
                    std::cout << "namespace " << e.name() << " {\n";
                else // if (info.event == cppast::visitor_info::container_entity_exit)
                    // close namespace
                    std::cout << "}\n";
            }
        });   

    /* deserialize function that deserializes the object. */
    std::cout << "namespace " << name << " {\n";
    std::cout << "template <typename T, std::enable_if_t<std::is_base_of_v<Engine::Entity, T>, bool> = true>\n";
    std::cout << "void deserialize(const json &js, Engine::Entity *entity);\n}\n}\n";
    cppast::visit(file,
        [&name](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    && e.parent().has_value() && e.parent().value().name() == name
                    // or the module namespace
                    || e.kind() == cppast::cpp_entity_kind::namespace_t
                    && e.name() == name;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                std::cout << "template <>\n";
                std::cout << "void deserialize<" << class_.name() << ", true>(const json &js, Engine::Entity *entity) {\n";

                // serialize base classes
                for (auto& base : class_.bases()) {
                    std::cout << " deserialize<" << base.name() << ", true>(js, entity);\n";
                }

                std::cout << "  " << class_.name() << " *e = (" << class_.name() << " *)entity;\n";
                
                // serialize member variables
                for (auto& member : class_)
                {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t)
                        generate_deserialize_member(std::cout,
                                                static_cast<
                                                    const cppast::cpp_member_variable&>(
                                                    member));
                }

                std::cout << "}\n";
            }
            else if (e.kind() == cppast::cpp_entity_kind::namespace_t)
            {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    // open namespace
                    std::cout << "namespace " << e.name() << " {\n";
                else // if (info.event == cppast::visitor_info::container_entity_exit)
                    // close namespace
                    std::cout << "}\n";
            }
        });   

    /* type_init function that creates Type object for all types. */
    std::cout << "void type_init() {\n";
    std::cout << "  using namespace " << name << ";\n";
    cppast::visit(file,
        [&name](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    && e.parent().has_value() && e.parent().value().name() == name
                    // or the module namespace
                    || e.kind() == cppast::cpp_entity_kind::namespace_t
                    && e.name() == name;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);
                
                std::cout << "  " << class_.name() << "::type = new Engine::Type("
                << "instantiate<" << class_.name() << ", true>, "
                << "serialize<" << class_.name() << ", true>, "
                << "deserialize<" << class_.name() << ", true>);\n";
            }
        });   
    std::cout << "}\n";
}

int main(int argc, char* argv[])
{
    /* create compile_commands.json file to the specified directory given in argv[1]. use argv[2] as a header file path */
    std::string build_dir(argv[1]);
    std::string src(argv[2]);
    std::ofstream fs(build_dir + "/compile_commands.json");
    if (fs.fail()) {
        std::cerr << "failed to open file compile_commands.json\n";
    }
    try {
        using json = nlohmann::json;
        json js, obj;
        obj["directory"] = build_dir; 
        obj["command"] = "g++ -std=c++14 -D_DEBUG_FUNCTIONAL_MACHINERY -c " + src;
        obj["file"] = src;
        js.push_back(obj);
        fs << js;
        fs.flush();
    } catch(...) {}

    return example_main(argc, argv, {}, generate_serialize);
}