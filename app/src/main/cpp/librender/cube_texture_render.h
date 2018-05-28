//
// Created by TY on 2018/5/28.
//

#ifndef ANDORIDOPENGLNDK_CUBE_TEXTURE_RENDER_H
#define ANDORIDOPENGLNDK_CUBE_TEXTURE_RENDER_H


#include "base_render.h"
#include <android/bitmap.h>
#include <jni.h>


#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

class CubeTextureRender : public BaseRender {


private:
    jobject saber;
    JavaVM *g_jvm;
    GLuint texture;
    GLint textureLocation, modelMatLocation, viewMatLocation, projectionMatLocation;


public:
    CubeTextureRender(const char *vertex1, const char *frag1,
                      jobject assetManager, JavaVM *g_jvm1, jobject saber);

    ~CubeTextureRender();

    void initRenderObj() override;

    void render() override;

    void dealloc() override;

    void initTexture();
};


#endif //ANDORIDOPENGLNDK_CUBE_TEXTURE_RENDER_H
