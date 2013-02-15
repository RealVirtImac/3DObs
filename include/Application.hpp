#pragma once

//!  Creates the context of the program
/*!
  * This class contains the main skeleton of the program, from the initialization of the SDL context to its deletion.
  * \author R. Bertozzi & S. Bougeois
  * \brief Creates the context of the program
  * \file Application.hpp
*/


#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Renderer.hpp"

class Application
{
	public:
		//! Constructor
		Application();
		//! Destructor
		~Application();
		//! Main skeleton of the program.
		/*!
		 * \return 0
		*/
		int on_execute();
		//! Initializes the SDL context
		/*!
		 * 	\return True if everything was all right, False otherwise
		 */
		bool on_init();
		//! Manages all the user's events
		/*!
		 * 	\param Event the input event
		 */
		void on_event(SDL_Event* Event);
		//! Makes all the required calculations just before the render method
		void on_loop();
		//! Renders the context
		void on_render();
		//! Cleans the context
		void on_cleanup();
	
	private :
		//! Is false, the program closes
		bool m_running;
		
		SDL_Surface* m_display;
		
		Renderer* m_renderer;
		
		int m_mouse_x;
		int m_mouse_y;
		
		int m_width;
		int m_height;
};
