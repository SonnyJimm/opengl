#include <iostream>

#include "VertexArray.h"

#include "Assertion.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &Renderer_ID));
};
VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &Renderer_ID));
};
void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        std::cout << layout.GetStride() << std::endl;
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
};
void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(Renderer_ID));
}
void VertexArray::UnBind() const
{
    GLCall(glBindVertexArray(0));
}
