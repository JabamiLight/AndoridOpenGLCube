#include <jni.h>
#include <string>
#include "jni_opengl_controler.h"
#include "librender/opengl_render_controller.h"
#include <android/native_window_jni.h>

OpenGlRenderController *openGlRenderController;

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_init
        (JNIEnv *env, jobject thiz, jobject assetManager) {
    if (!openGlRenderController) {
        openGlRenderController = new OpenGlRenderController(env, assetManager);
        openGlRenderController->start();
    }
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
    }
}
