/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 14:11:27 by thibautpier       #+#    #+#             */
/*   Updated: 2017/02/22 10:10:58 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.class.hpp"

Shader::Shader() {
    return;
}

Shader::Shader(std::string vertexSrc, std::string fragmentSrc)
                 : vertexSrc(vertexSrc),  fragmentSrc(fragmentSrc) {
    this->vertexID = this->load(GL_VERTEX_SHADER, this->vertexSrc);
    this->fragmentID = this->load(GL_FRAGMENT_SHADER, this->fragmentSrc);
    this->compile();
    return;
}

Shader::~Shader() {
    glDeleteShader(this->vertexID);
    glDeleteShader(this->fragmentID);
    glDeleteProgram(this->programID);
    return;
}

GLuint    Shader::load(GLenum type, std::string const src) {
    GLuint shaderID;
    std::ifstream stream(src);
    std::string buff;
    std::string shader;

    if(!stream) {
        std::cout << "Shader File: " << src << " not found."<< std::endl;
        return -1;
    }

    while(getline(stream, buff))
        shader += buff + '\n';
    stream.close();
    const GLchar *shaderSrc = shader.c_str();

    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shaderSrc, 0);
    glCompileShader(shaderID);

    if(!this->checkShaderCompilation(shaderID, type))
        exit(0);

    return shaderID;
}

void    Shader::compile() {
    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->vertexID);
    glAttachShader(this->programID, this->fragmentID);

    glBindAttribLocation(this->programID, 0, "position");
    glBindAttribLocation(this->programID, 1, "color");

    glLinkProgram(this->programID);

    if(!this->checkProgramCompilation(this->programID))
        exit(0);

    return;
}

bool        Shader::checkShaderCompilation(GLuint shaderID, GLenum type) {
    GLint status;
    GLint errorSize;
    char *error;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
        return true;

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errorSize);    
    error = new char[errorSize + 1];
    glGetShaderInfoLog(shaderID, errorSize, NULL, error);
    std::cout << "shader compilation error:" << std::endl;
    if (type == GL_VERTEX_SHADER)
        std::cout << "Compile failure in vertex shader" << std::endl;
    if (type == GL_FRAGMENT_SHADER)
        std::cout << "Compile failure in fragment shader" << std::endl;
    std::cout << error << std::endl;
    
    return false;
}

bool        Shader::checkProgramCompilation(GLuint program) {
    GLint status;
    GLint errorSize;
    char *error;

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_TRUE)
        return true;

    std::cout << "linker failure:" << std::endl;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorSize);
    std::cout << errorSize << std::endl;
    error = new char[errorSize + 1];
    glGetProgramInfoLog(program, errorSize, NULL, error);
    std::cout << error << std::endl;
    
    return false;
}

GLuint  Shader::getProgram() const {
    return  this->programID;
}