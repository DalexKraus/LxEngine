#ifndef __LXVAO_H
#define __LXVAO_H

#include "lx.h"

struct lxVaoData
{
    GLuint id;
    GLuint iboId;
};
typedef struct lxVaoData* lxVao;

// Create / Destroy functions
lxVao       lxVaoCreate();
void        lxVaoDestroy(lxVao vao);

// VAO functions
void        lxVaoStoreData(lxVao vao, int attrIdx, GLfloat* data, size_t dataSize, int dimension);
void        lxVaoStoreIndicesList(lxVao vao, GLuint* data, size_t dataSize, int indicesCount);

inline void lxVaoBind(lxVao vao);
inline void lxVaoUnbind(lxVao vao);

#endif