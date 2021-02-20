#include "Renderer.h"

void Renderer::DrawElement(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.Getcount(), GL_UNSIGNED_INT, nullptr);
}

