/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 14:04:43 by thibautpier       #+#    #+#             */
/*   Updated: 2017/07/20 14:20:43 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEM
# define PARTICLESYSTEM

# define WIN_WIDTH 1024
# define WIN_HEIGHT 1024
# define PARTICLE_NBR 10000

# include "SDL.h"
// # include <SDL2/SDL.h>
# include <iostream>
# include "Shader.class.hpp"
# include "OpenGl.class.hpp"
# include "OpenCl.class.hpp"

SDL_Window	*initSDL(SDL_GLContext ctx);
Vec4    	getMousePosition(SDL_Window *win);
void    	windowTitleUpdate(SDL_Window *win, int frameDuration);
void    	resetCamera(Vec4 *cameraRotation);
void    	eventManager(SDL_Window *win, SDL_Event *events, bool *pause, bool *exitLoop, Vec4 *mousePosition, Vec4 *cameraRotation, OpenCl *cl);


#endif