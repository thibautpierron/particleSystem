/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGl.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 17:07:54 by thibautpier       #+#    #+#             */
/*   Updated: 2017/07/20 13:39:45 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGl.class.hpp"

OpenGl::OpenGl(GLuint shaderProgram) {

    glClearColor(0.5, 0.5, 0.5, 0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

    glGenBuffers(1, &this->vbo);
    glGenVertexArrays(1, &this->vao);

    this->setShaderProgram(shaderProgram);
    return;
}

OpenGl::~OpenGl() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);

    return;
}

void OpenGl::loadVBO(unsigned int arrayLength) {
    this->vertexLen = arrayLength;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertexLen * sizeof(Vec4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    loadVAO();
    return;
}

void OpenGl::loadVAO() {
	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    return;
}

void OpenGl::setShaderProgram(GLuint program) {
    this->program = program;
    this->projectionMatrixUniformLocation = glGetUniformLocation(this->program, "projectionMatrix");
    this->modelMatrixUniformLocation = glGetUniformLocation(this->program, "modelMatrix");
    this->gravityPositionUniformLocation = glGetUniformLocation(this->program, "gravityPosition");
    
    return;
}

void OpenGl::setUniforms(Vec4 rot, Vec4 gravityCenter) {
    t_matrix matrix = createProjectionMatrix(60.0f, 1.0f, 1.0f, 100.0f);
    t_matrix model = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
    
    rotateMatrixOnY(&model, degToRad(rot.x));
    rotateMatrixOnX(&model, degToRad(rot.y));
    translateMatrix(&model, 0.0f, 0.0f, -30.0f + rot.z);

    glUniformMatrix4fv(this->projectionMatrixUniformLocation, 1, GL_FALSE, &matrix.m[0]);
	glUniformMatrix4fv(this->modelMatrixUniformLocation, 1, GL_FALSE, &model.m[0]);
    glUniform4f(this->gravityPositionUniformLocation, gravityCenter.x, gravityCenter.y, gravityCenter.z, gravityCenter.w);
    return;
}

void OpenGl::render(Vec4 rotation, Vec4 gravityCenter) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(this->program);   
    glBindVertexArray(this->vao);

    this->setUniforms(rotation, gravityCenter);
    glDrawArrays(GL_POINTS, 0, this->vertexLen);

    glBindVertexArray(0);
    glUseProgram(0);

    return;
}

GLuint OpenGl::getVBO() {
    return this->vbo;
}

void OpenGl::checkError(std::string str) {
    GLenum error = glGetError();

    if(error == GL_NO_ERROR) 
        return;
    std::cout << "Error num: " << error << " at :" << str << std::endl;
    exit(0);
    return;
}

