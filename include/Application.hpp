#pragma once

#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Renderer.hpp"

class Application
{
	public:
		Application();
		~Application();
		
		int on_execute();
		
		bool on_init();
		void on_event(SDL_Event* Event);
		void on_loop();
		void on_render();
		void on_cleanup();
	
	private :
		bool m_running;
		SDL_Surface* m_display;
		Renderer* m_renderer;
		int m_mouse_x;
		int m_mouse_y;
		
		int m_width;
		int m_height;
};
