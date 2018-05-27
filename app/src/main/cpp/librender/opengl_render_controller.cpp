//
// Created by 唐宇 on 2018/5/20.
//

#include "opengl_render_controller.h"

#define LOG_TAG "PicPreviewController"


void *OpenGlRenderController::threadStartCallback(void *myself) {
    OpenGlRenderController *controller = (OpenGlRenderController *) myself;
    controller->renderLoop();
//    pthread_exit(0);
    return nullptr;
}

void OpenGlRenderController::renderLoop() {
    bool renderingEnabled = true;
    LOGI("renderLoop()");
    while (renderingEnabled) {
        pthread_mutex_lock(&mLock);
        /*process incoming messages*/
        switch (_msg) {
            case MSG_WINDOW_SET:
                initializeEnv();
                break;
            case MSG_RENDER_LOOP_EXIT:
                renderingEnabled = false;
                destroy();
                break;
            case MSG_RENDER_CHANGE:
                initializeRenderObj();
                break;
            default:
                break;
        }
        _msg = MSG_NONE;
        if (eglCore) {
            eglCore->makeCurrent(previewSurface);
            this->drawFrame();
            pthread_cond_wait(&mCondition, &mLock);
            usleep(100 * 1000);
        }

        pthread_mutex_unlock(&mLock);
    }
    LOGI("Render loop exits");

    return;
}

bool OpenGlRenderController::initializeEnv() {
    eglCore = new EGLCore();
    eglCore->init();
    previewSurface = eglCore->createWindowSurface(_window);
    eglCore->makeCurrent(previewSurface);
    bool isRendererInitialized = render->init(screenWidth, screenHeight);
    if (!isRendererInitialized) {
        LOGI("Renderer failed on initialized...");
        return false;
    }
    if(!eglInit){
        initializeRenderObj();
    }
    eglInit=true;
    LOGI("Initializing context Success");
    return true;
}


void OpenGlRenderController::drawFrame() {
    render->render();
    if (!eglCore->swapBuffers(previewSurface)) {
        LOGE("eglSwapBuffers() returned error %d", eglGetError());
    }


}

void OpenGlRenderController::destroy() {
    LOGI("dealloc renderer ...");
    if (nullptr != render) {
        render->dealloc();
        delete render;
        render = NULL;
    }
    if (eglCore) {
        eglCore->releaseSurface(previewSurface);
        eglCore->release();
        eglCore = NULL;
    }
    return;

}


OpenGlRenderController::~OpenGlRenderController() {
    LOGI("VideoDutePlayerController instance destroyed");
    pthread_mutex_destroy(&mLock);
    pthread_cond_destroy(&mCondition);
}

bool OpenGlRenderController::start() {
    LOGI("Creating VideoDutePlayerController thread");
    pthread_create(&_threadId, nullptr, threadStartCallback, this);
    return true;
}

void OpenGlRenderController::stop() {
    LOGI("Stopping VideoDutePlayerController Render thread");
    /*send message to render thread to stop rendering*/
    pthread_mutex_lock(&mLock);
    _msg = MSG_RENDER_LOOP_EXIT;
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);
    LOGI("we will join render thread stop");
    pthread_join(_threadId, nullptr);
    render->dealloc();
    delete render;
    render= nullptr;
    LOGI("VideoDutePlayerController Render thread stopped");
}

void OpenGlRenderController::setWindow(ANativeWindow *window) {
    pthread_mutex_lock(&mLock);
    _msg = MSG_WINDOW_SET;
    _window = window;
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);
}

void OpenGlRenderController::resetSize(int width, int height) {
    LOGI("VideoDutePlayerController::resetSize width:%d; height:%d", width, height);
    pthread_mutex_lock(&mLock);
    this->screenWidth = width;
    this->screenHeight = height;
    render->resetRenderSize(0, 0, width, height);
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);
}

OpenGlRenderController::OpenGlRenderController(JNIEnv *env, jobject assetManager,
                                           ANativeWindow *window) {
    LOGI("VideoDutePlayerController instance created");
    pthread_mutex_init(&mLock, nullptr);
    pthread_cond_init(&mCondition, nullptr);
    screenWidth = 720;
    screenHeight = 720;
    _msg = MSG_WINDOW_SET;
    _window = window;
    JavaVM *g_jvm = NULL;
    env->GetJavaVM(&g_jvm);
    render = new ShapeRener("vertex_shader.glsl", "fragment_shader.glsl",
                                  env->NewGlobalRef(assetManager), g_jvm);
}

void OpenGlRenderController::initializeRenderObj() {
    render->initRenderObj();
}
