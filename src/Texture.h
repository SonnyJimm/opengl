#pragma once

#include "Render.h"

class Texture{
    private:
        unsigned int Renderer_ID;
        std::string FilePath;
        unsigned char* LocalBuffer;
        int Width, Height, BPP;
    public:
        Texture(std::string path);
        ~Texture();
        void Bind(unsigned int slot = 0) const;
        void UnBind();
        inline int GetWidth() const {return Width;};
        inline int GetHeight() const {return Height;};
};