#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Assertion.h"

Shader::Shader(const std::string& filepath)
    :FilePath(filepath), Rendered_ID(0)
{
    ShaderSource source = parseShader(filepath);
    Rendered_ID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(Rendered_ID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(Rendered_ID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name),value));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3));
}

unsigned int Shader::GetUniformLocation(const std::string &name)
{
    if(UniformLocations.find(name) != UniformLocations.end()){
        return UniformLocations[name];
    }
    GLCall(int location = glGetUniformLocation(Rendered_ID,name.c_str()));
    if (location == -1){
        std::cout<<"Warning uniform" << name <<" doesnt exist" <<std::endl;
    }
    UniformLocations[name] = location;
    
    return location;
}
unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    int success;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Program validation error:\n%s\n", infoLog);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
ShaderSource Shader::parseShader(const std::string filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE SHADER " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
    }

    return id;
}
