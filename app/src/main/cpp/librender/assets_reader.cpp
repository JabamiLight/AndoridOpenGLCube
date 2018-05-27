//
// Created by TY on 2018/5/25.
//

#include "assets_reader.h"

AssetReader::AssetReader(JavaVM *g_jvm, jobject assetManager) : g_jvm(g_jvm),
                                                                assetManager(assetManager) {}
void
AssetReader::readSource(const char *vertexFilePath, const char *fragFilePath, char *&vertexContent,
                        char *&fragContent) {
    int status;
    JNIEnv *env;
    bool isAttached = false;
    status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status < 0) {
        g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
        isAttached = true;
    }
    LOGI("ReadAssets");
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    if (mgr == NULL) {
        LOGI(" %s", "AAssetManager==NULL");
        return;
    }
    AAsset *assetVertex = AAssetManager_open(mgr, vertexFilePath, AASSET_MODE_BUFFER);
    if (assetVertex == NULL) {
        LOGI(" %s", "asset==NULL");
        return;
    }
    mallocContent(assetVertex, vertexContent);
    AAsset *assetFrag = AAssetManager_open(mgr, fragFilePath, AASSET_MODE_BUFFER);
    if (assetFrag == NULL) {
        LOGI(" %s", "asset==NULL");
        return;
    }
    mallocContent(assetFrag, fragContent);
    if (isAttached)
        g_jvm->DetachCurrentThread();

}

AssetReader::~AssetReader() {
    if (assetManager) {
        int status;
        JNIEnv *env;
        bool isAttached = false;
        status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (status < 0) {
            g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
            isAttached = true;
        }
        env->DeleteGlobalRef(assetManager);
        if (isAttached)
            g_jvm->DetachCurrentThread();
    }
}
