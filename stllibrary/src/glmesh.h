#ifndef GLMESH_H
#define GLMESH_H

#include <QtOpenGL/QGLBuffer>
#include <QtOpenGL/QGLFunctions>
#include "stllibrary_global.h"

class Mesh;

class STLLIBRARYSHARED_EXPORT GLMesh : protected QGLFunctions
{
public:
    GLMesh(const Mesh* const mesh);
    void draw(GLuint vp);
private:
    QGLBuffer vertices;
    QGLBuffer indices;
};

#endif // GLMESH_H
