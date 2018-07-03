

// Created by TY on 2018/6/26.
//

#include <android/bitmap.h>
#include "pbo_render.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void PboRender::initRenderObj() {

    if (VAO && VBO) {
        glDeleteVertexArrays(1, VAO);
        glDeleteBuffers(1, VBO);
    } else {
        VAO = new GLuint;
        VBO = new GLuint;
    }
    //生成两个VAO和两个VBO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    float vertices1[] = {
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // 右下角
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // 右上角
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 左下角
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上角
    };
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &frame);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glGenTextures(1, &textureFrame);
    glBindTexture(GL_TEXTURE_2D, textureFrame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_FRAMEBUFFER, 0);
    initTexture();

}

void PboRender::initTexture() {
    resetTexture();
    glUseProgram(program);
    textureLocation = glGetUniformLocation(program, "texture1");

}

void PboRender::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DITHER);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, curPicWidth, curPicHeight);
    glUseProgram(program);
//    resetTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLocation, 0);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    readPixels();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _backingWidth, _backingHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, textureFrame);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}


PboRender::PboRender(const char *vertex1, const char *frag1, jobject assetManager1,
                     JavaVM *g_jvm, jobject *bitmapArray, jsize length) : BaseRender(vertex1, frag1,
                                                                                     assetManager1,
                                                                                     g_jvm) {

    this->g_jvm = g_jvm;
    this->pics = bitmapArray;
    isRenderContinus = true;
    caculateFps = true;
    this->length = length;
}

void PboRender::resetTexture() {
    jobject convertPic;
    convertPic = *(pics + textureIndex);
    int status;
    JNIEnv *env;
    bool isAttached = false;
    status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status < 0) {
        g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
        isAttached = true;
    }

    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, convertPic, &info) < 0) {
        LOGI("获取bitmat 信息失败");
        return;
    }
    unsigned char *dataFromBmp = NULL;
    AndroidBitmap_lockPixels(env, convertPic, (void **) &dataFromBmp);
    if (!dataFromBmp) {
        LOGI("获取pixel 失败");
        return;
    }
    curPicWidth = info.width;
    curPicHeight = info.height;
    long long curTime = getCurrentTime();

    long long consumTime = 0;


    initPob();

    if (initFrame) {
        glBindFramebuffer(GL_FRAMEBUFFER, frame);
        glBindTexture(GL_TEXTURE_2D, textureFrame);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, curPicWidth, curPicHeight, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFrame,
                               0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        initFrame = false;
    }


    if (uoloadPboType == NONE) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, curPicWidth, curPicHeight, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     dataFromBmp);

//        LOGE("复制数据时间%lld", getCurrentTime() - curTime);
    } else if (uoloadPboType == ONE) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[0]);
        curTime = getCurrentTime();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     0);
        LOGE("上传纹理时间%lld", getCurrentTime() - curTime);


        curTime = getCurrentTime();
        glBufferData(GL_PIXEL_UNPACK_BUFFER, info.width * info.height * 4, 0, GL_DYNAMIC_DRAW);

        GLubyte *ptr = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, mPboSize,
                                                    GL_MAP_WRITE_BIT |
                                                    GL_MAP_INVALIDATE_BUFFER_BIT);
        consumTime = getCurrentTime() - curTime;
        LOGE("获取映射时间时间%lld", consumTime);
        curTime = getCurrentTime();
        if (ptr) {
            memcpy(ptr, dataFromBmp, info.width * info.height * 4);
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
        }
        consumTime = getCurrentTime() - curTime;
        LOGE("++复制数据时间%lld", consumTime);


        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    } else if (uoloadPboType == TWO) {
        index = (index + 1) % 2;
        nextIndex = (index + 1) % 2;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[index]);
//        glPixelStorei(GL_UNPACK_ALIGNMENT,128);
        curTime = getCurrentTime();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     0);


        LOGE("图片处理时间%lld", getCurrentTime() - curTime);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[nextIndex]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, info.width*info.height*4, dataFromBmp, GL_STREAM_DRAW);

        curTime = getCurrentTime();
        GLubyte *ptr = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
                                                    info.width * info.height * 4,
                                                    GL_MAP_WRITE_BIT |
                                                    GL_MAP_INVALIDATE_BUFFER_BIT);
        LOGE("获取映射时间时间%lld", getCurrentTime() - curTime);
        if (ptr) {
            curTime = getCurrentTime();
            memcpy(ptr, dataFromBmp, info.width * info.height * 4);
            LOGE("++复制数据时间%lld", getCurrentTime() - curTime);
            curTime = getCurrentTime();
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
            LOGE("释放时间%lld", getCurrentTime() - curTime);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    } else if (uoloadPboType == THREE) {
        index = (index + 1) % 3;
        nextIndex = (index + 1) % 3;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[index]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     0);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[nextIndex]);
        curTime = getCurrentTime();
        glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_DYNAMIC_DRAW);
        GLubyte *ptr = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, mPboSize,
                                                    GL_MAP_WRITE_BIT |
                                                    GL_MAP_INVALIDATE_BUFFER_BIT);
        LOGE("获取映射时间时间%lld", getCurrentTime() - curTime);
        if (ptr) {
            curTime = getCurrentTime();
            memcpy(ptr, dataFromBmp, info.width * info.height * 4);
            LOGE("++复制数据时间%lld", getCurrentTime() - curTime);
            curTime = getCurrentTime();
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
            LOGE("释放时间%lld", getCurrentTime() - curTime);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    AndroidBitmap_unlockPixels(env, convertPic);
    if (isAttached)
        g_jvm->DetachCurrentThread();
    glBindTexture(GL_TEXTURE_2D, 0);
//    textureIndex = static_cast<char>((textureIndex + 1) % length);
}

void PboRender::initPob() {
    if (initPbo) {
        initPbo = false;
        uploadPobs = new GLuint[3];
        downloadPbos = new GLuint[2];
//        glGenBuffers(3, uploadPobs);
        glGenBuffers(2, downloadPbos);
        int align = 128;
        int width = curPicWidth;
        int height = curPicHeight;
//        mPboSize = ((width * 4 + (align - 1)) & ~(align - 1)) * height;
        mPboSize = curPicWidth * curPicHeight * 4;
//         mRowStride = (width * 4 + (align - 1)) & ~(align - 1);
//        mPboSize = mRowStride * height;
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[0]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[1]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[2]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPbos[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, mPboSize, NULL, GL_DYNAMIC_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPbos[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, mPboSize, NULL, GL_DYNAMIC_READ);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }


}


void my_copy(volatile unsigned char *dst, volatile unsigned char *src, int sz)
{
    if (sz & 63) {
        sz = (sz & -64) + 64;
    }
    asm volatile (
    "NEONCopyPLD: \n"
            " VLDM %[src]!,{d0-d7} \n"
            " VSTM %[dst]!,{d0-d7} \n"
            " SUBS %[sz],%[sz],#0x40 \n"
            " BGT NEONCopyPLD \n"
    : [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory");
}

void PboRender::unBindPbo() {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    index = (index + 1) % 2;
    nextIndex = (index + 1) % 2;
}


void PboRender::readPixels() {

    int size = mPboSize;
    char path[40];
    sprintf(path, "/mnt/sdcard/pixel/readPixel%d.rgba", picCount);
//    picCount--;
    if (picCount <= 0) {
        return;
    }
    if (!cachePixel) {
        cachePixel = new byte[size];
        memset(cachePixel, 0, size);
    }
//    if (access("/mnt/sdcard/pixel", 0)) {
//        mkdir("/mnt/sdcard/pixel", S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
//    }
//    FILE *file = fopen(path, "wb");
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    long long curTime = getCurrentTime();
    if (downloadPboType == NONE) {
        long long cc = getCurrentTime();
        byte *pixel = new byte[size];
        glReadPixels(0, 0, curPicWidth, curPicHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        LOGE("glReadPixels Time %lld", getCurrentTime() - cc);
        cc = getCurrentTime();
//        fwrite(pixel, size, 1, file);
        my_copy(cachePixel, pixel, size);
        LOGE("内存复制耗时 %lld", getCurrentTime() - cc);
        delete[] pixel;
    } else if (downloadPboType == ONE) {


    } else if (downloadPboType == TWO) {
        index = (index + 1) % 2;
        nextIndex = (index + 1) % 2;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPbos[index]);
        long long cc = getCurrentTime();
        glReadPixels(0, 0, curPicWidth, curPicHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        LOGE("glReadPixels Time %lld", getCurrentTime() - cc);

        if (readPixInit) {
            readPixInit = false;
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            return;
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPbos[nextIndex]);
        GLubyte *ptr = static_cast<GLubyte *>(glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0,
                                                               mPboSize,
                                                               GL_MAP_READ_BIT));
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // release pointer to mapping buffer
        cc = getCurrentTime();
        if (ptr) {
//            int status;
//            JNIEnv *env;
//            bool isAttached = false;
//            status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
//            if (status < 0) {
//                g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
//                isAttached = true;
//            }
//            jobject byteBuffer = env->NewDirectByteBuffer(ptr, size);
//
//            jclass clz=env->GetObjectClass(byteBuffer);
//            jmethodID methodId = env->GetMethodID(clz, "get", "([B)Ljava/nio/ByteBuffer;");
//            jbyteArray byteArray = env->NewByteArray(size);
//            env->CallObjectMethod(byteBuffer,methodId,byteArray);
//
//
//            if (isAttached)
//                g_jvm->DetachCurrentThread();

//            memcpy(cachePixel, ptr, size);
            my_copy(cachePixel,ptr,size);


        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

//            fwrite(ptr, size, 1, file);
        LOGE("内存复制耗时 %lld", getCurrentTime() - cc);

    }
//    fclose(file);

    LOGE("完成耗时%lld", getCurrentTime() - curTime);
}


