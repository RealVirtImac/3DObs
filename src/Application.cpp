#include "../include/Application.hpp"

Application::Application():m_running(true), m_display(NULL)
{
}

Application::~Application()
{
}

bool Application::on_init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}
	
	SDL_WM_SetCaption("3DObs",0);
	
	m_width = 1280;
	m_height = 720;
	
	if((m_display = SDL_SetVideoMode(m_width,m_height,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL)) == NULL)
	{
		return false;
	}
	
	m_renderer = new Renderer(m_width,m_height);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_WarpMouse(m_width/2, m_height/2);
	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	SDL_ShowCursor(SDL_DISABLE); 
	
	SDL_EnableKeyRepeat(40, 10);
	return true;
}

int Application::on_execute()
{
	if(this->on_init() == false)
	{
		return -1;
	}
	
	SDL_Event Event;
	
	while(m_running)
	{
		while(SDL_PollEvent(&Event))
		{
			on_event(&Event);
		}
		this->on_loop();
		
		this->on_render();
	}
	
	this->on_cleanup();
	
	return 0;
}

void Application::on_loop()
{
	SDL_WarpMouse(m_width/2, m_height/2);
	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	
	m_renderer->get_camera_one()->update_horizontal_angle(m_mouse_x);
	m_renderer->get_camera_one()->update_vertical_angle(m_mouse_y);
	m_renderer->get_camera_one()->update_target();
	m_renderer->get_camera_one()->compute_view_matrix();
	
	m_renderer->get_camera_two()->update_horizontal_angle(m_mouse_x);
	m_renderer->get_camera_two()->update_vertical_angle(m_mouse_y);
	m_renderer->get_camera_two()->update_target();
	m_renderer->get_camera_two()->compute_view_matrix();
}

void Application::on_event(SDL_Event* Event)
{
	if(Event->type == SDL_QUIT)
	{
		m_running = false;
	}
	if(Event->type == SDL_KEYDOWN)
	{
		switch(Event->key.keysym.sym)
		{
			case SDLK_ESCAPE: m_running = false;
			break;
			//~ Forward
			case SDLK_z : 	
							
			break;
			//~ Backward
			case SDLK_s : 	
							
			break;
			//~ Left
			case SDLK_q : 	
							
			break;
			//~ Right
			case SDLK_d : 	
							
			break;
			//~ Wireframe
			case SDLK_w : m_renderer->switch_drawing_mode();
			break;
			default : ;
			break;
		}
	}
}

void Application::on_render()
{
	m_renderer->render();
	
	SDL_GL_SwapBuffers();
}

void Application::on_cleanup()
{
	delete m_renderer;
	SDL_FreeSurface(m_display);
	SDL_Quit();
}
