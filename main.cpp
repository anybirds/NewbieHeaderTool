#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <vector>

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_type.hpp>
#include <cppast/visitor.hpp>
#include <cppast/libclang_parser.hpp>

#include <nlohmann/json.hpp>

// generate a serialization call for a member
void generate_serialize_member(std::ostream& out, const cppast::cpp_member_variable& member)
{
    // auto& type = cppast::remove_cv(member.type());

    if (!cppast::has_attribute(member, "NoSerialize")) {
        out << "  js.push_back(e->" << member.name() << ");\n";
    }
}

// generate a deserialization call for a member
void generate_deserialize_member(std::ostream& out, const cppast::cpp_member_variable& member)
{
    auto& type = cppast::remove_cv(member.type());

    if (!cppast::has_attribute(member, "NoSerialize")) {
        out << "  e->" << member.name() << " = js.back().get<" << cppast::to_string(type) << ">(); js.erase(--js.end());\n";
    }
}

// generate serialization function
void generate_serialize(const cppast::cpp_file& file)
{
    cppast::visit(file,
        [](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    // or all namespaces
                    || e.kind() == cppast::cpp_entity_kind::namespace_t;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                std::cout << "void " << class_.name() << "::Serialize(json &js, const Engine::Entity *entity) {\n";

                // serialize the base class
                std::cout << "  " << (*class_.bases().begin()).name() << "::StaticType()->Serialize(js, entity);\n";
                
                // serialize member variables
                std::cout << "  const " << class_.name() << " *e = (const " << class_.name() << " *)entity;\n";
                for (auto& member : class_)
                {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t)
                        generate_serialize_member(std::cout,
                                                static_cast<
                                                    const cppast::cpp_member_variable&>(
                                                    member));
                }

                std::cout << "}\n";
            } else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    // open namespace
                    std::cout << "namespace " << e.name() << " {\n";
                else // if (info.event == cppast::visitor_info::container_entity_exit)
                    // close namespace
                    std::cout << "}\n";
            }
        });   
}

// generate deserialization function
void generate_deserialize(const cppast::cpp_file& file) {
    cppast::visit(file,
        [](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    // or all namespaces
                    || e.kind() == cppast::cpp_entity_kind::namespace_t;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                std::cout << "void " << class_.name() << "::Deserialize(json &js, Engine::Entity *entity) {\n";
                
                // deserialize member variables
                std::cout << "  " << class_.name() << " *e = (" << class_.name() << " *)entity;\n";
                std::vector<const cppast::cpp_entity *> entities;
                for (auto &member : class_)  {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
                        entities.push_back(&member);
                    }
                }
                for (auto it = entities.rbegin(); it != entities.rend(); it++) {
                    auto &member = **it;
                    generate_deserialize_member(std::cout,
                                                static_cast<
                                                    const cppast::cpp_member_variable&>(
                                                    member));
                }

                // deserialize base classes
                std::cout << "  " << (*class_.bases().begin()).name() << "::StaticType()->Deserialize(js, entity);\n";

                std::cout << "}\n";
            } else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    // open namespace
                    std::cout << "namespace " << e.name() << " {\n";
                else // if (info.event == cppast::visitor_info::container_entity_exit)
                    // close namespace
                    std::cout << "}\n";
            }
        });   
}

// generate type_init function
void generate_typeinit(const cppast::cpp_file& file) {
    cppast::visit(file,
        [](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    // or all namespaces
                    || e.kind() == cppast::cpp_entity_kind::namespace_t;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);
                
                std::cout << "  " << class_.name() << "::StaticType(new Type(\""
                << class_.name() << "\", "
                << "instantiate<" << class_.name() << ", true>, "
                << class_.name() << "::Serialize, "
                << class_.name() << "::Deserialize));\n";
            } else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    std::cout << "  using namespace " << e.name() << ";\n";
            }
        });
}

// generate type_clear function
void generate_typeclear(const cppast::cpp_file& file) {
    cppast::visit(file,
        [](const cppast::cpp_entity& e) {
            // only visit non-templated class definitions that have the attribute set
            return (!cppast::is_templated(e)
                    && e.kind() == cppast::cpp_entity_kind::class_t
                    && cppast::is_definition(e)
                    && cppast::has_attribute(e, "Serialize"))
                    // or all namespaces
                    || e.kind() == cppast::cpp_entity_kind::namespace_t;
        },
        [](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity())
            {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);
                
                std::cout << "  delete " << class_.name() << "::StaticType();\n";
            } else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    std::cout << "  using namespace " << e.name() << ";\n";
            }
        });
}

template <typename Callback>
int example_main(int argc, char* argv[], const cppast::cpp_entity_index& index, Callback sc, Callback dc, Callback ic, Callback cc) try
{
    cppast::libclang_compilation_database database(argv[1]); // the compilation database

    // simple_file_parser allows parsing multiple files and stores the results for us
    cppast::simple_file_parser<cppast::libclang_parser> parser(type_safe::ref(index));
    try
    {
        cppast::parse_database(parser, database); // parse all files in the database
    }
    catch (cppast::libclang_error& ex)
    {
        std::cerr << "fatal libclang error: " << ex.what() << '\n';
        return 1;
    }

    if (parser.error())
        // a non-fatal parse error
        // error has been logged to stderr
        return 1;

    for (auto &file : parser.files())
        sc(file);
    for (auto &file : parser.files())
        dc(file);

    /* type_init function that creates Type objects. */
    std::cout << "void type_init() {\n";
    for (auto& file : parser.files())
        ic(file);
    std::cout << "}\n";
    
    /* type_clear function that clear Type objects. */
    std::cout << "void type_clear() {\n";
    for (auto& file : parser.files())
        cc(file);
    std::cout << "}\n";
    
    return 0;
}
catch (std::exception& ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}

int main(int argc, char* argv[])
{
    /* create compile_commands.json file to the specified directory given in argv[1]. use argv[2] as a header file path */
    std::string build_dir(argv[1]);
    std::string src_dir(argv[2]);
    std::string module_name(argv[3]);
    std::string src(src_dir + "/" + module_name + ".hpp");
    std::ofstream fs(build_dir + "/compile_commands.json");
    if (fs.fail()) {
        std::cerr << "failed to open file compile_commands.json\n";
    }
    std::ofstream hs(src);
    if (hs.fail()) {
        std::cerr << "failed to open file" + src + "\n";
    }
    hs << "#pragma once\n";
    try {
        using json = nlohmann::json;
        json js;
        std::function<void(const std::string &, const std::string &)> write = [&write, &js, &build_dir, &src_dir, &module_name, &hs](const std::string &path, const std::string &relative) {
            for (auto &p : std::filesystem::directory_iterator(path)) {
                if (p.is_directory()) {
                    write(p.path().string(), relative + p.path().filename().string() + "/");
                } else if (p.path().extension() == ".hpp" && p.path().filename().string() != module_name + ".hpp") {
                    json obj;
                    std::string header = src_dir + "/" + relative + p.path().filename().string();
                    obj["directory"] = build_dir; 
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                    obj["command"] = "g++ -DGLFW_DLL -DJSON_USE_IMPLICIT_CONVERSIONS=1 -isystem C:/vcpkg/installed/x64-windows/include -isystem C:/vcpkg/installed/x64-windows/include/utf8cpp -isystem C:/vcpkg/installed/x64-windows/include/irrlicht -isystem C:/vcpkg/installed/x64-windows/include/kubazip -isystem C:/vcpkg/installed/x64-windows/include/poly2tri -std=c++1z -D_DEBUG_FUNCTIONAL_MACHINERY -IC:/Newbie/Engine -I" + src_dir + " -c " + header;
#else
                    obj["command"] = "g++ -DGLFW_DLL -DJSON_USE_IMPLICIT_CONVERSIONS=1 -isystem /vcpkg/installed/x64-windows/include -isystem /vcpkg/installed/x64-windows/include/utf8cpp -isystem /vcpkg/installed/x64-windows/include/irrlicht -isystem /vcpkg/installed/x64-windows/include/kubazip -isystem /vcpkg/installed/x64-windows/include/poly2tri -std=c++1z -D_DEBUG_FUNCTIONAL_MACHINERY -I/Newbie/Engine -I" + src_dir + " -c " + header;
#endif
                    obj["file"] = header;
                    js.push_back(obj); 

                    hs << "#include <" << relative << p.path().filename().string() << ">\n";
                }
            }
        };
        write(src_dir, "");
        fs << js;
        fs.flush();
        hs.flush();
    } catch(...) {}

    /* include headers */ 
    std::cout << "#include <type_traits>\n";
    std::cout << "#include <nlohmann/json.hpp>\n";
    std::cout << "#include <" << module_name << ".hpp>\n";
    std::cout << "using json = nlohmann::json;\n";

    return example_main(argc, argv, {}, generate_serialize, generate_deserialize, generate_typeinit, generate_typeclear);
}