//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H
#define ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H

#include <android/native_window.h>
#include "shape_render.h"
#include "egl_core.h"
#include <unistd.h>


class OpenGlRenderController {

public:
    OpenGlRenderController(JNIEnv *env, jobject assetManager
                             );
    OpenGlRenderController(JNIEnv *env, jobject assetManager,jobject saber
                             );
    OpenGlRenderController(JNIEnv *env,jobject thiz,jobject assetManager,jobjectArray saber
                             );
    virtual ~OpenGlRenderController();
    bool start();
    void stop();
    void setWindow(ANativeWindow* window);
    void resetSize(jint width, jint height, ANativeWindow *pWindow);
    void rotate(jfloat x,jfloat y ,jfloat degree);

    void scale(jfloat scale);
    int fps();

private:
    BaseRender* render;

    int screenWidth;
    int screenHeight;
    bool eglInit= false;
    bool isRendererInitialized=false;
    enum RenderThreadMessage{
        MSG_NONE=0,MSG_WINDOW_SET,MSG_RENDER_LOOP_EXIT,MSG_RENDER_CHANGE
    };
    pthread_t _threadId;
    pthread_mutex_t mLock;
    pthread_cond_t mCondition;
    enum RenderThreadMessage _msg;

    // android window, supported by NDK r5 and newer
    ANativeWindow* _window;
    EGLCore* eglCore= nullptr;
    EGLSurface previewSurface;

    JavaVM *g_jvm;
    jobject jObj;
    // Helper method for starting the thread
    static void* threadStartCallback(void *myself);
    // RenderLoop is called in a rendering thread started in start() method
    // It creates rendering context and renders scene until stop() is called
    void renderLoop();
    bool initializeEnv();
    void updateTexImage();
    void drawFrame();
    void destroy();


    void initializeRenderObj();


};



#endif //ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H
