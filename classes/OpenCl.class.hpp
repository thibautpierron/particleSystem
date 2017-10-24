/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenCl.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/19 10:56:04 by thibautpier       #+#    #+#             */
/*   Updated: 2017/07/20 14:14:26 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_CLASS_HPP
# define OPENCL_CLASS_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <OpenCL/opencl.h>
# include <OpenGL/OpenGL.h>
# include <OpenGL/gl3.h>
# include <OpenGl.class.hpp>

class OpenCl {

    public:
        OpenCl(std::string kernelSrc);
        ~OpenCl();
        void        		createSharedVBO(GLuint buf);
        void        		createOnlyClBuffer(unsigned int arrayLength);
        void        		runGravityKernel(Vec4 mousPosition);
		void				runInitKernel(unsigned int);

    private:
        OpenCl();
        void        		loadKernel(std::string);
        void        		extractKernel();
        void        		checkError(cl_int, std::string msg);
        void        		saveResult();
        void        		getInteropCompatibleDevice();
        
        cl_int              error;
        cl_platform_id      platform;
        cl_uint             numPlatform;
        cl_context          context;
        cl_command_queue    queue;
        cl_uint             deviceNbr;
        cl_device_id        *devicesID;
        cl_device_id        device;
        cl_event            event;
        char                *kernelSrc;
        size_t              kernelSrcSize;

        cl_kernel           gravityKernel;
		std::vector<cl_kernel>	initKernels;
        cl_mem              sharedVBO;
        cl_mem              velocityVBO;
        cl_program          program;

        size_t				bufferItemNumber;

};

#endif