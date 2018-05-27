//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H
#define ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H

#include "base_render.h"

class ShapeRener: public BaseRender {
protected:

    enum {
        ATTRIBUTE_VERTEX
    };

public:
    ShapeRener(const char *vertex, const char *frag,  _jobject *assetManager, JavaVM *g_jvm);
    virtual ~ShapeRener();
    virtual void render();
    void initRenderObj();

private:
    GLuint EBO;
};


#endif //ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H
