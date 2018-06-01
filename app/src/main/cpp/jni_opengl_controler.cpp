#include <jni.h>
#include <string>
#include "jni_opengl_controler.h"
#include "librender/opengl_render_controller.h"
#include <android/native_window_jni.h>

OpenGlRenderController *openGlRenderController;
OpenGlRenderController *openGlCubeRenderController;

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_init
        (JNIEnv *env, jobject thiz, jobject assetManager) {
    if (!openGlRenderController) {
        openGlRenderController = new OpenGlRenderController(env, assetManager);
        openGlRenderController->start();
    }
}
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_initCube
        (JNIEnv *env, jobject thiz, jobject assetManager,jobject saber) {
    if (!openGlCubeRenderController) {
        openGlCubeRenderController = new OpenGlRenderController(env, assetManager,saber);
        openGlCubeRenderController->start();
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_resetSize
        (JNIEnv *env, jobject, jint width, jint height, jobject surface) {
    if (openGlRenderController) {
        openGlRenderController->resetSize(width, height, ANativeWindow_fromSurface(env, surface));
    }
    if (openGlCubeRenderController) {
        openGlCubeRenderController->resetSize(width, height, ANativeWindow_fromSurface(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_stop
        (JNIEnv *, jobject) {
    if (openGlRenderController) {
        openGlRenderController->stop();
        delete openGlRenderController;
        openGlRenderController= nullptr;
    }
    if (openGlCubeRenderController) {
        openGlCubeRenderController->stop();
        delete openGlCubeRenderController;
        openGlCubeRenderController= nullptr;
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_rotate
        (JNIEnv *, jobject,jfloat x,jfloat y,jfloat degree){
    if (openGlCubeRenderController) {
        openGlCubeRenderController->rotate(x,y,degree);
    }

}
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_scale
        (JNIEnv *, jobject,jfloat scale){
    if (openGlCubeRenderController) {
        openGlCubeRenderController->scale(scale);
    }
}
