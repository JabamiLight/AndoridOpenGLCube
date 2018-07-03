//
// Created by 唐宇 on 2018/5/19.
//

#include "egl_core.h"
#define LOG_TAG "EGLCORE"

EGLCore::EGLCore() {
    pfneglPresentationTimeANDROID = 0;
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

EGLCore::~EGLCore() {


}

bool EGLCore::init() {
    return this->init(NULL);

}

bool EGLCore::init(EGLContext sharedContext) {
    EGLint numConfigs;
    EGLint width;
    EGLint height;
    EGLint major;//主版本号
    EGLint minor;//次版本号
    //通过属性去筛选合适的配置
    const EGLint attibutes[] = {
//            EGL_BUFFER_SIZE, 32,
            EGL_ALPHA_SIZE, 0,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, //opengl es 3
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_DEPTH_SIZE, 16, //请求深度缓冲区
            EGL_STENCIL_SIZE,0,
            EGL_NONE
    };

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay() returned error %d", eglGetError());
        return false;
    }
    if (!eglInitialize(display, &major, &minor)) {
        LOGE("eglInitialize() returned error %d", eglGetError());
        return false;
    }
    //这里只取一个config
    if (!eglChooseConfig(display, attibutes, &config, 1, &numConfigs)) {
        LOGE("eglChooseConfig() returned error %d", eglGetError());
        release();
        return false;
    }
    EGLint eglContextAttribute[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    if (!(context = eglCreateContext(display, config,
                                     NULL == sharedContext ? EGL_NO_CONTEXT : sharedContext,
                                     eglContextAttribute))) {
        LOGE("eglCreateContext() returned error %d", eglGetError());
        release();
        return false;
    }


    pfneglPresentationTimeANDROID = reinterpret_cast<PFNEGLPRESENTATIONTIMEANDROIDPROC>(eglGetProcAddress(
            "eglPresentationTimeANDROID"));
    if (!pfneglPresentationTimeANDROID) {
        LOGE("eglPresentationTimeANDROID is not available!");
    }

    return true;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow *_window) {
    EGLSurface surface = NULL;
    EGLint format;


    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
        release();
        return surface;
    }
    //TODO 尝试不要
    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);
    if (!(surface=eglCreateWindowSurface(display, config, _window, 0))) {
        LOGE("eglCreateWindowSurface() returned error %d", eglGetError());
    }
    return surface;

}

EGLSurface EGLCore::createOffscreenSurface(int width, int height) {
    EGLSurface surface;
    EGLint PbufferAttributes[] = {EGL_WIDTH, width, EGL_HEIGHT, height, EGL_NONE, EGL_NONE};
    if (!(surface = eglCreatePbufferSurface(display, config, PbufferAttributes))) {
        LOGE("eglCreatePbufferSurface() returned error %d", eglGetError());
    }

    return surface;
}

bool EGLCore::makeCurrent(EGLSurface eglSurface) {
    return static_cast<bool>(eglMakeCurrent(display, eglSurface, eglSurface, context));
}

void EGLCore::doneCurrent() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}


bool EGLCore::swapBuffers(EGLSurface eglSurface) {
    return static_cast<bool>(eglSwapBuffers(display, eglSurface));
}

EGLint EGLCore::querySurface(EGLSurface surface, EGLint what) {
    EGLint value = -1;
    eglQuerySurface(display, surface, what, &value);
    return value;
}

int EGLCore::setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs) {
    pfneglPresentationTimeANDROID(display, surface, nsecs);
    return 0;
}

void EGLCore::releaseSurface(EGLSurface &eglSurface) {
    eglDestroySurface(display, eglSurface);

}

void EGLCore::release() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    LOGI("after eglMakeCurrent...");
    eglDestroyContext(display, context);
    LOGI("after eglDestroyContext...");
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

EGLContext EGLCore::getContext() {
    LOGI("return EGLCore getContext...");
    return context;
}

EGLDisplay EGLCore::getDisplay() {
    return display;
}

EGLConfig EGLCore::getConfig() {
    return config;
}

void EGLCore::destorySurface(EGLSurface surface) {
    eglDestroySurface(display,surface);

}
