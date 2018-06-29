//
// Created by TY on 2018/6/26.
//

#ifndef ANDORIDOPENGLCUBE_PBO_RENDER_H
#define ANDORIDOPENGLCUBE_PBO_RENDER_H


#include "base_render.h"
#include <android/bitmap.h>
#include <memory.h>

class PboRender: public BaseRender {
private:
    JavaVM *g_jvm;
    jobject* pics;
    GLuint texture;
    jsize length;
    char textureIndex=0;
    GLint textureLocation;
    GLuint * pbos;
    int index=0,nextIndex=0;
    bool  init=true;
    enum PBOTYPE{
        NONE,ONE,TWO,THREE
    };
    PBOTYPE pboType=THREE;


public:

    PboRender(const char vertex1[29], const char frag1[31], jobject assetManager1, JavaVM *g_jvm1,
                  jobject* bitmapArray, jsize frag);

    void initRenderObj() override;

    void initTexture();
    void render() override;
    void resetTexture();
    void unBindPbo();

    void initPob();

    int mPboSize;
};


#endif //ANDORIDOPENGLCUBE_PBO_RENDER_H
