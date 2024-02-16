#include <GL/glew.h>

#include "Render.h"
#include "Assertion.h"

Render::Render()
{
}

Render::~Render()
{
}

void Render::Clear() const
{
    // glClearColor(1.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Render::Draw(const IndexBuffer &ib, const VertexArray &va, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}