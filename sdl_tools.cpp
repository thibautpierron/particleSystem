/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_tools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/20 14:18:32 by tpierron          #+#    #+#             */
/*   Updated: 2017/07/20 14:21:31 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particleSystem.hpp"

SDL_Window *initSDL(SDL_GLContext ctx) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_Window *win = SDL_CreateWindow("Particle System",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WIN_WIDTH, WIN_HEIGHT,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    ctx = SDL_GL_CreateContext(win);
    if(ctx == 0) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
    return win;
}

Vec4    getMousePosition(SDL_Window *win) {
    int mouseX, mouseY, winX, winY;
    Vec4    position;

    SDL_GetGlobalMouseState(&mouseX, &mouseY);
    SDL_GetWindowPosition(win, &winX, &winY);

    position.x = mouseX - winX - WIN_WIDTH / 2;
    position.y = (mouseY - winY - WIN_HEIGHT / 2) * -1;
    position.z = 0;
    position.w = 0;

    if (position.x < (WIN_WIDTH / 2) * -1)
        position.x = (WIN_WIDTH / 2) * -1;
    else if (position.x > (WIN_WIDTH / 2))
        position.x = (WIN_WIDTH / 2);
    if (position.y < (WIN_HEIGHT / 2) * -1)
        position.y = (WIN_HEIGHT / 2) * -1;
    else if (position.y > (WIN_HEIGHT / 2))
        position.y = (WIN_HEIGHT / 2);
    position.x /= 30;
    position.y /= 30;
    return position;
}

void    windowTitleUpdate(SDL_Window *win, int frameDuration) {
    static int timeCounter = 0;
    std::string frameRate;
    std::string newTitle;

    timeCounter += frameDuration;
    if(timeCounter > 1000000) {
        frameRate = std::to_string(1000000 / frameDuration);
        newTitle = "Particle System (" + frameRate + "fps)"; 
        SDL_SetWindowTitle(win, newTitle.c_str());
        timeCounter = 0;
    }
}

void    resetCamera(Vec4 *cameraRotation) {
    cameraRotation->x = 0;
    cameraRotation->y = 0;
    cameraRotation->z = 0;
    cameraRotation->w = 0;
}

void    eventManager(SDL_Window *win, SDL_Event *events, bool *pause, bool *exitLoop, Vec4 *mousePosition, Vec4 *cameraRotation, OpenCl *cl) {
    static unsigned int initShape = 0;
    if (initShape > 2)
        initShape = 0;

    if (events->window.event == SDL_WINDOWEVENT_CLOSE || events->key.keysym.sym == SDLK_ESCAPE) {
        *exitLoop = true;
        return;
    }
    if (events->type == SDL_KEYDOWN) {
        switch(events->key.keysym.sym) {
            case SDLK_SPACE: *pause = !*pause; resetCamera(cameraRotation); break;
            case SDLK_1: cl->runInitKernel(0); *pause = true; resetCamera(cameraRotation); break;
            case SDLK_2: cl->runInitKernel(1); *pause = true; resetCamera(cameraRotation); break;
            case SDLK_3: cl->runInitKernel(2); *pause = true; resetCamera(cameraRotation); break;
            case SDLK_RIGHT: cameraRotation->x -= 1; *pause = true; break;
            case SDLK_LEFT: cameraRotation->x += 1; *pause = true; break;
            case SDLK_UP: cameraRotation->y -= 1; *pause = true; break;
            case SDLK_DOWN: cameraRotation->y += 1; *pause = true; break;
            case SDLK_KP_MINUS: cameraRotation->z -= 0.5; *pause = true; break;
            case SDLK_KP_PLUS: cameraRotation->z += 0.5; *pause = true; break;
        }
    }
    if (events->button.button == SDL_BUTTON_LEFT) {
        *pause = false;
        *mousePosition = getMousePosition(win);
        resetCamera(cameraRotation);
    }
}