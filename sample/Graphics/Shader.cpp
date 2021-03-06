#include <fstream>
#include <iostream>
#include <sstream>

#include <Graphics/Shader.hpp>
#include <Project.hpp>

using namespace std;
using namespace Engine;

shared_ptr<Resource> AShader::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Shader>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Shader::Shader(AShader *ashader) : Resource(ashader), id(0) {
    Apply();
}

Shader::~Shader() {
    glDeleteShader(id);
}

void Shader::Apply() {
    glDeleteShader(id);
    id = glCreateShader(GetShaderType());
    if (!id) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create a new shader" << '\n';
        throw exception();
    }

    ifstream file(Project::GetInstance().GetDirectoy() + "/" + GetPath());
    if (!file.is_open()) {
        glDeleteShader(id);
        cerr << '[' << __FUNCTION__ << ']' << " failed to find a shader: " << GetPath() << '\n';
        throw exception();
    }

    stringstream buf;
    buf << file.rdbuf();
    string source = buf.str();
    const char *source_char = source.c_str();
    glShaderSource(id, 1, &source_char, NULL);

    glCompileShader(id);
    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(id);
        cerr << '[' << __FUNCTION__ << ']' << " failed to Compile a shader: " << GetPath() << '\n';
        throw exception();
    }
}