//
// Created by TY on 2018/5/25.
//

#ifndef OPENGLNDK_ASSETREADER_H
#define OPENGLNDK_ASSETREADER_H

#include <jni.h>
#include <android/asset_manager_jni.h>
#include "../libcommon/CommonTools.h"
#define LOG_TAG "AssetReader"
class AssetReader {
    JavaVM* g_jvm;
    jobject  assetManager;
public:
    AssetReader(JavaVM *g_jvm, jobject assetManager);
    void readSource( const char* vertexFilePath,
                    const char* fragFilePath,
                    char*& vertexContent,
                    char*& fragContent);
    virtual ~AssetReader();

    void mallocContent(AAsset *pAsset, char* &content) {
        off_t  bufferSize=AAsset_getLength(pAsset);
        LOGI("file size: %d\n",bufferSize);
        content=new char[bufferSize+1];
        content[bufferSize]=0;
        int numBytesRead=AAsset_read(pAsset,content,bufferSize);
        AAsset_close(pAsset);
    }

};


#endif //OPENGLNDK_ASSETREADER_H
