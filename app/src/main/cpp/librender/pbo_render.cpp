

// Created by TY on 2018/6/26.
//

#include <android/bitmap.h>
#include "pbo_render.h"


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
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // 右下角
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右上角
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 左下角
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f   // 左上角
    };
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
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

    glGenFramebuffers(1,&frame);
    glBindFramebuffer(GL_FRAMEBUFFER,frame);
    glGenTextures(1,&textureFrame);
    glBindTexture(GL_TEXTURE_2D,textureFrame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    initPob();
    initTexture();

}

void PboRender::initTexture() {
    resetTexture();
    glUseProgram(program);
    textureLocation = glGetUniformLocation(program, "texture1");

}

void PboRender::render() {
    glViewport(0, 0, curPicWidth, curPicHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    long long cur=getCurrentTime();
    resetTexture();
    LOGE("重置纹理时间%lld",getCurrentTime()-cur);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLocation, 0);
    glBindFramebuffer(GL_FRAMEBUFFER,frame);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    readPixels();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
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
    curPicWidth=info.width;
    curPicHeight=info.height;
    long long curTime = getCurrentTime();

    long long consumTime = 0;

    if (uoloadPboType == NONE) {
        glBindTexture(GL_TEXTURE_2D,textureFrame);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, curPicWidth, curPicHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureFrame,0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA,
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
                                                    GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
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
    uploadPobs = new GLuint[3];
    downloadPbos = new GLuint[2];
    glGenBuffers(3, uploadPobs);
    glGenBuffers(2, downloadPbos);
    int align=128;
    int width=6000;
    int height=6000;
    mPboSize = ((width * 4 + (align - 1)) & ~(align - 1)) * height;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPobs[2]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, mPboSize, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);


    glBindBuffer(GL_PIXEL_PACK_BUFFER, uploadPobs[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, mPboSize, 0, GL_STATIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, uploadPobs[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, mPboSize, 0, GL_STATIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);




}

void PboRender::unBindPbo() {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    index = (index + 1) % 2;
    nextIndex = (index + 1) % 2;
}

void PboRender::readPixels() {

    int size=_backingHeight*_backingWidth*4;
    FILE *file = fopen("/mnt/sdcard/readPixel.rgba", "wb");
    long long curTime=getCurrentTime();
    if(downloadPboType==NONE){
        byte* pixel=new byte[size];
        glReadPixels(0,0,_backingWidth,_backingHeight,GL_RGBA,GL_UNSIGNED_BYTE,pixel);
        fwrite(pixel,size,1,file);
        delete [] pixel;
    } else if(downloadPboType==ONE){
        
    } else if(downloadPboType==TWO){
        index = (index + 1) % 2;
        nextIndex = (index + 1) % 2;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, uploadPobs[index]);
        glReadPixels(0,0,_backingWidth,_backingHeight,GL_RGBA,GL_UNSIGNED_BYTE,0);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, uploadPobs[nextIndex]);
//        glBufferData(GL_PIXEL_PACK_BUFFER, info.width*info.height*4, dataFromBmp, GL_STREAM_DRAW);

        GLubyte *ptr = (GLubyte *) glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0,
                                                    size,
                                                    GL_MAP_READ_BIT);
        if (ptr) {
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // release pointer to mapping buffer
            fwrite(ptr,size,1,file);
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    fclose(file);

    LOGE("读取完成耗时%lld",getCurrentTime()-curTime);
}


