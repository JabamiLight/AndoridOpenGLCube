//
// Created by 唐宇 on 2018/5/20.
//

#include "shape_render.h"

#define LOG_TAG "ShapeRener"

ShapeRener::~ShapeRener() {
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    delete[]VAO;
    delete[]VBO;
}


void ShapeRener::render() {
    glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);
    //设置一个颜色状态
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //使能颜色状态的值来清屏
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void ShapeRener::initRenderObj() {
    float vertices[] = {
            -0.5f, -0.0f, 0.0f, // left
            0.5f, -0.0f, 0.0f, // right
            0.0f, 1.0f, 0.0f  // top
    };
    if (VAO && VBO) {
        glDeleteVertexArrays(2, VAO);
        glDeleteBuffers(2, VBO);
        glDeleteBuffers(1, &EBO);
    } else {
        VAO = new GLuint[2];
        VBO = new GLuint[2];
    }
    //生成两个VAO和两个VBO
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    //绑定第一个VAO
    glBindVertexArray(VAO[0]);
    //绑定第一个VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //GL_STATIC_DRAW表示顶点不会变化，绑定数组
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //指定解析顶点规则
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vertices1[] = {
            0.5f, -0.25f, 0.0f,   // 右上角
            0.5f, -0.75f, 0.0f,  // 右下角
            -0.5f, -0.75f, 0.0f, // 左下角
            -0.5f, -0.25f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    //绑定索引EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //绑定索引数组
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //指定解析顶点规则
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

ShapeRener::ShapeRener(const char *vertex, const char *frag, _jobject *assetManager,
                       JavaVM *g_jvm) : BaseRender(vertex, frag, assetManager, g_jvm) {}


