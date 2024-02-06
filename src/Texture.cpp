#include "Texture.h"
#include "Assertion.h"
#include "Render.h"
#include <iostream>
#include "resource/stb/stb_image.h"
Texture::Texture(std::string path)
:Renderer_ID(0), FilePath(path), LocalBuffer(nullptr),Width(0),Height(0),BPP(0)
{
    GLCall(glGenTextures(1,&Renderer_ID));
    GLCall(glBindTexture(GL_TEXTURE_2D,Renderer_ID));
    LocalBuffer = stbi_load(path.c_str(),&Width,&Height,&BPP,4);
    std::cout << "loaded ?" << std::endl;
    stbi_set_flip_vertically_on_load(1);
    // in case scaled up
    GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR));
    // in case scaled down
    GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR));
    // what if the porder is small and fit the container
    GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,Width,Height,0,GL_RGBA,GL_UNSIGNED_BYTE,LocalBuffer));
    glGenerateMipmap(GL_TEXTURE_2D);
    GLCall(glBindTexture(GL_TEXTURE_2D,0));
    if (LocalBuffer){
        stbi_image_free(LocalBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1,&Renderer_ID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0+slot));
    GLCall(glBindTexture(GL_TEXTURE_2D,Renderer_ID));
}

void Texture::UnBind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D,0));
}
