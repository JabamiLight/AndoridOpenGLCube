//
// Created by 唐宇 on 2018/5/27.
//

#ifndef OPENGLNDK_BASE_RENDER_H
#define OPENGLNDK_BASE_RENDER_H

#include "../libcommon/CommonTools.h"
#include <android/asset_manager.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "assets_reader.h"
class BaseRender {
protected:
    char* PIC_PREVIEW_FRAG_SHADER_2;
    char* PIC_PREVIEW_VERTEX_SHADER_2;
    const char* vertexPath;
    const char* fragPath;
    GLint _backingLeft;
    GLint _backingTop;
    GLint _backingWidth;
    GLint _backingHeight;

    GLuint vertShader;
    GLuint fragShader;
    GLuint  uniformSampler;
    GLuint program;
    AssetReader* assetReader;
    GLuint *VBO= nullptr,*VAO= nullptr;
    int useProgram();
    int initShader();
    int bindLocation();
    GLuint compileShader(GLenum type, const char *source);
    bool checkGlError(const char* op);


public:
    BaseRender(const char *vertex, const char *frag, jobject assetManager, JavaVM* g_jvm);
    virtual ~BaseRender();
    virtual bool init(int width, int height);
    virtual void render()=0;
    virtual void dealloc();

    virtual void resetRenderSize(int left, int top, int width, int height);

    virtual void initRenderObj()=0;
    bool isRenderContinus= false;

};


#endif //OPENGLNDK_BASE_RENDER_H
