#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:
    unsigned int Renderer_ID;

public:
    VertexArray();
    ~VertexArray();
    void Bind() const;
    void UnBind() const;
    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &bl);
};