//
// Created by 唐宇 on 2018/5/19.
//

#ifndef ANDROIDOPENGLDEMO_EGL_CORE_H
#define ANDROIDOPENGLDEMO_EGL_CORE_H

#include "../libcommon/CommonTools.h"
#include <pthread.h>
#include <android/native_window.h>
#include <EGL/egl.h>

//定义一个函数指针
typedef EGLBoolean (EGLAPIENTRYP PFNEGLPRESENTATIONTIMEANDROIDPROC)(EGLDisplay display, EGLSurface surface, khronos_stime_nanoseconds_t time);

class EGLCore {

public:
    EGLCore();
    virtual ~EGLCore();
    bool  init();
    bool init(EGLContext sharedContext);
    EGLSurface createWindowSurface(ANativeWindow* _window);
    EGLSurface createOffscreenSurface(int width, int height);

    bool makeCurrent(EGLSurface eglSurface);

    void doneCurrent();
    bool  swapBuffers(EGLSurface eglSurface);
    EGLint querySurface(EGLSurface surface, int what);
    int setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs);

    void releaseSurface(EGLSurface& eglSurface);
    void release();

    EGLContext getContext();
    EGLDisplay getDisplay();
    EGLConfig getConfig();

private:
    EGLDisplay display;
    EGLConfig config;
    EGLContext  context;
    PFNEGLPRESENTATIONTIMEANDROIDPROC pfneglPresentationTimeANDROID;

};


#endif //ANDROIDOPENGLDEMO_EGL_CORE_H
