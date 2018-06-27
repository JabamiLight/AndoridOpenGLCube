#include <jni.h>
#include <string>
#include "jni_opengl_controler.h"
#include "librender/opengl_render_controller.h"
#include <android/native_window_jni.h>

OpenGlRenderController *openGlRenderController;


JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_init
        (JNIEnv *env, jobject thiz, jobject assetManager) {
    if(openGlRenderController) {
        Java_com_example_ty_openglndk_NativeOpenglControler_stop(nullptr, nullptr);
    }
    openGlRenderController = new OpenGlRenderController(env, assetManager);
    openGlRenderController->start();
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_initCube
        (JNIEnv *env, jobject thiz, jobject assetManager, jobject saber) {
    if(openGlRenderController) {
        Java_com_example_ty_openglndk_NativeOpenglControler_stop(nullptr, nullptr);
    }
    openGlRenderController = new OpenGlRenderController(env, assetManager, saber);
    openGlRenderController->start();
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_resetSize
        (JNIEnv *env, jobject, jint width, jint height, jobject surface) {
    if (openGlRenderController) {
        openGlRenderController->resetSize(width, height, ANativeWindow_fromSurface(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_stop
        (JNIEnv *, jobject) {
    if (openGlRenderController) {
        openGlRenderController->stop();
        delete openGlRenderController;
        openGlRenderController = nullptr;
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_rotate
        (JNIEnv *, jobject, jfloat x, jfloat y, jfloat degree) {
    if (openGlRenderController) {
        openGlRenderController->rotate(x, y, degree);
    }

}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_scale
        (JNIEnv *, jobject, jfloat scale) {
    if (openGlRenderController) {
        openGlRenderController->scale(scale);
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_initPbo
        (JNIEnv *env, jobject thiz, jobject assetManager, jobjectArray bitmapArray) {
    if(openGlRenderController) {
        Java_com_example_ty_openglndk_NativeOpenglControler_stop(nullptr, nullptr);
    }
    openGlRenderController = new OpenGlRenderController(env,thiz, assetManager, bitmapArray);
    openGlRenderController->start();

}