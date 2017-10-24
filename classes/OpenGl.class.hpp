/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGl.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 17:08:27 by thibautpier       #+#    #+#             */
/*   Updated: 2017/07/20 14:15:24 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_CLASS_HPP
# define OPENGL_CLASS_HPP

# include <iostream>
# include <OpenGL/gl3.h>
# include "../math3d.hpp"

struct Vec4 {
    float x, y, z, w;
};

class OpenGl {

    public:
        OpenGl(GLuint shaderProgram);
        ~OpenGl();

        void			loadVBO(unsigned int arrayLength);
        GLuint			getVBO();
        void			render(Vec4, Vec4);
        
    private:
        OpenGl();
        void			loadVAO();
        void			setShaderProgram(GLuint);
        void			setUniforms(Vec4, Vec4);
        void			checkError(std::string);

        unsigned int	vertexLen;
        
        GLuint			vao;
        GLuint			vbo;
        GLuint			program;

        GLint			projectionMatrixUniformLocation;
        GLint			modelMatrixUniformLocation;
        GLint			gravityPositionUniformLocation;
};

#endif