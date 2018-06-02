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
    glm::mat4 model = glm::mat4(1.0f);

    long currentTime;
    float degree=0,x=0,y=0,fov=45.0f;
    glm::vec3 lastCross;

public:
    CubeTextureRender(const char *vertex1, const char *frag1,
                      jobject assetManager, JavaVM *g_jvm1, jobject saber);

    ~CubeTextureRender();

    void initRenderObj() override;

    void render() override;

    void dealloc() override;

    void initTexture();

    void initMatrix();

    void rotate(jfloat x,jfloat y,jfloat degree);

    void setScale(jfloat d);


};


#endif //ANDORIDOPENGLNDK_CUBE_TEXTURE_RENDER_H
