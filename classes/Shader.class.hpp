/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 14:11:32 by thibautpier       #+#    #+#             */
/*   Updated: 2017/02/21 14:08:32 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_CLASS_HPP
# define SHADER_CLASS_HPP

# include <iostream>
# include <fstream>
# include <OpenGL/gl3.h>

class Shader {

    public:
        Shader(std::string vertexSrc, std::string fragmentSrc);
        ~Shader();
        GLuint getProgram() const;

    private:
        Shader();
        GLuint      load(GLenum , std::string);
        void        compile();

        bool        checkShaderCompilation(GLuint, GLenum);
        bool        checkProgramCompilation(GLuint);

        GLuint          vertexID;
        GLuint          fragmentID;
        GLuint          programID;
        std::string     vertexSrc;
        std::string     fragmentSrc;
};

#endif