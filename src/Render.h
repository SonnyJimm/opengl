#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Render
{
public:
    Render();
    ~Render();
    void Clear() const;
    void Draw(const IndexBuffer& ib,const VertexArray& va,const Shader& shader) const;

};
