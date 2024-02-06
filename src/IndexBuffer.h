#pragma once
class IndexBuffer
{
private:
    unsigned int Renderer_ID;
    unsigned int Count;

public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();
    void Bind() const;
    void UnBind() const;
    inline unsigned int GetCount() const { return Count; }
};