#include "lx.h"

//Global VAO count
static int g_vaoCount = 0;

/**
 * Creates a new Vertex Array Object and returns it.
 */
lxVao lxVaoCreate()
{
    lxVao vaoInstance = malloc(sizeof(lxVao));
    glGenVertexArrays(1, &vaoInstance->id);
    g_vaoCount++;
}

/**
 * Destroys a previously created Vertex Array Object.
 */
void lxVaoDestroy(lxVao vao)
{
    _ASSERT(vao != NULL, "The VAO instance must not be null!");
    
    //Delete all VBOs attached to this VAO first
    GLint maxAttributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    for (int attr = 0; attr < maxAttributes; attr++)
    {
        GLint vboId = 0;
        glGetVertexAttribiv(attr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
        if (vboId <= 0) continue;
        glDeleteBuffers(1, &vboId);
    }

    glDeleteVertexArrays(1, &vao->id);
    free(vao);
    g_vaoCount--;
}

/**
 * Stores the given data in a new attribute array.
 */
void lxVaoStoreData(lxVao vao, int attrIdx, GLfloat* data, size_t dataSize, int dimension)
{
    _ASSERT(vao     != NULL, "The VAO instance must not be null!");
    _ASSERT(data    != NULL, "The data must not be null!");

    //Generate new VBO id
    GLuint vboId;
    glGenBuffers(1, &vboId);

    //Store data in vbo
    lxVaoBind(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW); //TODO: Change to dynamic for batch rendering
    glEnableVertexAttribArray(attrIdx);
    glVertexAttribPointer(attrIdx, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(float), 0);
    glDisableVertexAttribArray(attrIdx);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Stores the index buffer.
 */
void lxVaoStoreIndicesList(lxVao vao, GLuint* data, size_t dataSize, int indicesCount)
{
    _ASSERT(vao     != NULL,    "The VAO instance must not be null!");
    _ASSERT(data    != NULL,    "The data must not be null!");
    _ASSERT(indicesCount == 0,  "The indices count is zero!");
    
    glGenBuffers(1, &vao->iboId);
    lxVaoBind(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), data, GL_STATIC_DRAW); //TODO: Change to dynamic for batch rendering
}

inline void lxVaoBind(lxVao vao)
{
    _ASSERT(vao != NULL, "The VAO instance must not be null!");
    glBindVertexArray(vao->id);
}

inline void lxVaoUnbind(lxVao vao)
{
    _ASSERT(vao != NULL, "The VAO instance must not be null!");
    glBindVertexArray(0);
}