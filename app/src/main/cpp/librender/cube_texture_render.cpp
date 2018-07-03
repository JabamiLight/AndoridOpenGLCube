//
// Created by TY on 2018/5/28.
//

#include <android/bitmap.h>
#include "cube_texture_render.h"
#include "../glm/gtx/vector_angle.inl"
#include <time.h>

CubeTextureRender::CubeTextureRender(const char *vertex1, const char *frag1,
                                     jobject assetManager, JavaVM *g_jvm1, jobject saber)
        : BaseRender(vertex1, frag1, assetManager, g_jvm1) {
    this->saber = saber;
    this->g_jvm = g_jvm1;
}

void CubeTextureRender::initRenderObj() {
    initTexture();
    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };
    if (VAO && VBO) {
        glDeleteVertexArrays(1, VAO);
        glDeleteBuffers(1, VBO);
    }
    VAO = new GLuint;
    VBO = new GLuint;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    initMatrix();


}

float y = 0;

void CubeTextureRender::render() {
    glViewport(0, 0, _backingWidth, _backingHeight);
    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLocation, 0);
    glm::mat4 view = glm::mat4(1.0f);
//    long cur = getCurrentTime();
//    float timeLost = (cur - currentTime) / 400.0f;
    //opengl右手 矩阵左手

//    model = glm::rotate(model, degree, glm::vec3(x, y, 0.0f));
    if (x > 0 && y > 0) {
        y = -y;
    } else if (x <= 0 && y > 0) {
        x = -x;
        degree = -degree;
    } else if (x > 0 && y < 0) {
        y = -y;
    } else if (x <= 0 && y < 0) {
        y = -y;
    }
   LOGE("tedu %f %f %f" ,x,y,degree);

    if (degree != 0) {
        glm::vec3 cross = glm::cross(glm::vec3(0.0f, 0.0f, 1.0), glm::vec3(x, y, 0.0f));
        glm::mat4 tmpMat = glm::mat4(1.0f);
        tmpMat = glm::rotate(tmpMat, degree, cross);
        tmpMat *= model;
        model = tmpMat;
    }
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 projection = glm::mat4(1.0f);
    float ratio = (float) _backingWidth / (float) _backingHeight;
    //    projection = glm::ortho(-1.0f, 1.0f, -ratio, ratio,0.1f, 10.0f);

    projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);

    glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, &(model[0][0]));
    glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, &projection[0][0]);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);


}

void CubeTextureRender::dealloc() {
    BaseRender::dealloc();
    if (saber) {
        int status;
        JNIEnv *env;
        bool isAttached = false;
        status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (status < 0) {
            g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
            isAttached = true;
        }
        AndroidBitmap_unlockPixels(env, saber);
        env->DeleteGlobalRef(saber);
        if (isAttached)
            g_jvm->DetachCurrentThread();
    }
    if (texture) {
        glDeleteTextures(1, &texture);
    }

}

void CubeTextureRender::initTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int status;
    JNIEnv *env;
    bool isAttached = false;
    status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status < 0) {
        g_jvm->AttachCurrentThread(&env, NULL);//将当前线程注册到虚拟机中．
        isAttached = true;
    }

    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, saber, &info) < 0) {
        LOGI("获取bitmat 信息失败");
        return;
    }
    unsigned char *dataFromBmp = NULL;
    AndroidBitmap_lockPixels(env, saber, (void **) &dataFromBmp);
    if (!dataFromBmp) {
        LOGI("获取pixel 失败");
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 dataFromBmp);
    AndroidBitmap_unlockPixels(env, saber);
    if (isAttached)
        g_jvm->DetachCurrentThread();
    glBindTexture(GL_TEXTURE_2D, 0);

}

CubeTextureRender::~CubeTextureRender() {
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    delete VAO;
    delete VBO;
}

void CubeTextureRender::initMatrix() {
    glUseProgram(program);
    textureLocation = glGetUniformLocation(program, "texture1");
    viewMatLocation = glGetUniformLocation(program, "view");
    modelMatLocation = glGetUniformLocation(program, "model");
    projectionMatLocation = glGetUniformLocation(program, "projection");
    currentTime = getCurrentTime();

}

void CubeTextureRender::rotate(jfloat x, jfloat y, jfloat degree) {
    this->x = x;
    this->y = y;
    this->degree = degree / 80.0f;
}

void CubeTextureRender::setScale(jfloat scale) {
    LOGE("scale %f",scale);
    if (scale > 1) {
        fov -= scale;
    } else if(scale<1) {
        fov +=  (1.0f/scale);
    }
    if (fov <= 20.0) {
        fov = 20.0;
    } else if (fov > 140.0) {
        fov = 140.0;
    }
}


