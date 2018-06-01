//
// Created by TY on 2018/5/25.
//

#ifndef OPENGLNDK_NATIVE_OPENGL_CONTROLER_H
#define OPENGLNDK_NATIVE_OPENGL_CONTROLER_H

#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_init
(JNIEnv *, jobject, jobject);
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_initCube
(JNIEnv *, jobject, jobject,jobject);

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_resetSize
(JNIEnv *, jobject, jint,jint,jobject);

JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_stop
(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_rotate
(JNIEnv *, jobject,jfloat,jfloat,jfloat);
JNIEXPORT void JNICALL Java_com_example_ty_openglndk_NativeOpenglControler_scale
(JNIEnv *, jobject,jfloat);


#ifdef __cplusplus
}
#endif

#endif //OPENGLNDK_NATIVE_OPENGL_CONTROLER_H
