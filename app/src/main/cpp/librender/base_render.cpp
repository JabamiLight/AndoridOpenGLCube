//
// Created by 唐宇 on 2018/5/27.
//

#include "base_render.h"

int BaseRender::useProgram() {
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    //绑定位置的几种方式
    bindLocation();
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char message[infoLen];
            glGetProgramInfoLog(program, infoLen, NULL, message);
            LOGE("Error linking program : %s\n", message);
        }

        glDeleteProgram(program);
        return 0;
    }
    glUseProgram(program);
    return 1;
}

int BaseRender::initShader() {
    assetReader->readSource(vertexPath, fragPath, PIC_PREVIEW_VERTEX_SHADER_2,
                            PIC_PREVIEW_FRAG_SHADER_2);
    vertShader = compileShader(GL_VERTEX_SHADER, PIC_PREVIEW_VERTEX_SHADER_2);
    if (!vertShader) {
        return -1;
    }
    fragShader = compileShader(GL_FRAGMENT_SHADER, PIC_PREVIEW_FRAG_SHADER_2);
    if (!fragShader) {
        return -1;
    }
    return 1;
}

GLuint BaseRender::compileShader(GLenum type, const char *source) {
    GLint status;
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        LOGE("Failed to create shader %d", type);
        return 0;
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char message[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, message);
            LOGE("Failed to compile shader : %s\n", message);
        }

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool BaseRender::checkGlError(const char *op) {
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("error::after %s() glError (0x%x)\n", op, error);
        return true;
    }
    return false;
}

BaseRender::BaseRender(const char *vertex, const char *frag, jobject assetManager, JavaVM *g_jvm) {
    vertexPath = vertex;
    fragPath = frag;
    assetReader = new AssetReader(g_jvm, assetManager);

}

BaseRender::~BaseRender() {

}

bool BaseRender::init(int width, int height) {
    this->_backingLeft = 0;
    this->_backingTop = 0;
    this->_backingWidth = width;
    this->_backingHeight = height;

    vertShader = 0;
    fragShader = 0;
    program = 0;
    int ret = initShader();
    if (ret < 0) {
        LOGI("init shader failed...");
        this->dealloc();
        return false;
    }
    ret = useProgram();
    if (ret < 0) {
        LOGI("use program failed...");
        this->dealloc();
        return false;
    }
    return true;
}

void BaseRender::dealloc() {
    if (vertShader) {
        glDeleteShader(vertShader);
        vertShader = 0;
    }
    if (fragShader) {
        glDeleteShader(fragShader);
        fragShader = 0;
    }
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    if (assetReader) {
        delete assetReader;
        assetReader = NULL;
    }

}

void BaseRender::resetRenderSize(int left, int top, int width, int height) {
    this->_backingLeft = left;
    this->_backingTop = top;
    this->_backingWidth = width;
    this->_backingHeight = height;
}

int BaseRender::bindLocation() {
    return 0;
}
