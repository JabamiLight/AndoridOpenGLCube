#include <jni.h>
#include <string>
#include "jni_opengl_controler.h"
#include "librender/opengl_render_controller.h"
#include <android/native_window_jni.h>

OpenGlRenderController *openGlRenderController;

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_init
        (JNIEnv *env, jobject thiz, jobject assetManager, jobject surface) {
    if (!openGlRenderController) {
        openGlRenderController = new OpenGlRenderController(env, assetManager,
                                                            ANativeWindow_fromSurface(env, surface));
        openGlRenderController->start();
    } else{
        openGlRenderController->setWindow(ANativeWindow_fromSurface(env, surface));

    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_resetSize
        (JNIEnv *, jobject, jint width, jint height) {
    if (openGlRenderController) {
        openGlRenderController->resetSize(width, height);
    }
}

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_stop
        (JNIEnv *, jobject) {
    if (openGlRenderController) {
        openGlRenderController->stop();
    }
}
