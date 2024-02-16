#pragma once

#include <string>
#include <unordered_map>
#include "resource/glm/glm.hpp"
struct ShaderSource
{
    std::string VertexShader;
    std::string FragmentShader;
};
class Shader
{
private:
    std::string FilePath;
    unsigned int Rendered_ID;
    std::unordered_map<std::string,int> UniformLocations;
public:
    Shader(const std::string &filepath);
    ~Shader();
    void Bind() const;
    void UnBind() const;
    void SetUniform1i(const std::string &name, int value);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniformMatrix4fv(const std::string &name, const glm::mat4 &mat);
    inline unsigned int GetRendererID()const{ return Rendered_ID;};
private:
    unsigned int GetUniformLocation(const std::string& name);
    bool CompileShader();
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    ShaderSource parseShader(const std::string filepath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
};
