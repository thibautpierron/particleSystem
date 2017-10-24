/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 09:44:30 by thibautpier       #+#    #+#             */
/*   Updated: 2017/07/20 14:25:42 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particleSystem.hpp"

int main(int ac, char** av) {
    srand(time(NULL));

    int particleNumber;
    if (ac == 1)
        particleNumber = 1000000;
    else if (ac == 2)
        particleNumber = strtol(av[1], NULL, 0);
    else
        exit(0);

    SDL_Event                                       events;
    SDL_GLContext                                   ctx;
    SDL_Window                                      *win;
    Vec4                                            mousePosition, cameraRotation;
    std::chrono::high_resolution_clock::time_point  start, end;
    GLint                                           sync = 0;
    CGLContextObj                                   glctx;
    bool                                            pause = true;
    bool                                            exitLoop = false;

    win = initSDL(&ctx);

    Shader shaders("shader/color2d.vert", "shader/color2d.frag");
    OpenCl cl("kernel/kernel.cl");
    OpenGl gl(shaders.getProgram());
    gl.loadVBO(particleNumber);
    cl.createSharedVBO(gl.getVBO());
    cl.createOnlyClBuffer(particleNumber);

    cl.runInitKernel(0);
    resetCamera(&cameraRotation);
    gl.render(cameraRotation, mousePosition);
    SDL_GL_SwapWindow(win);
    glctx = CGLGetCurrentContext();

    while (!exitLoop) {
        start = std::chrono::high_resolution_clock::now();
        CGLSetParameter(glctx, kCGLCPSwapInterval, &sync);
            
        gl.render(cameraRotation, mousePosition);
        SDL_GL_SwapWindow(win);

        if (!pause)
            cl.runGravityKernel(mousePosition);
               
        SDL_PollEvent(&events);
        eventManager(win, &events, &pause, &exitLoop, &mousePosition, &cameraRotation, &cl);
        end = std::chrono::high_resolution_clock::now();
        windowTitleUpdate(win, std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count());
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}