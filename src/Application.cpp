/*!
 * \file Application.cpp
 * \brief Creates the context of the program
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Application.hpp"

Application::Application():
	m_running(true),
	m_display(NULL),
	m_joystick(NULL)
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

	for(int i = 0; i < 4; ++i)
	{
		m_input_keys.push_back(false);
	}

	int numJoystick = SDL_NumJoysticks();
	if (numJoystick >= 1)
	{
		m_joystick = SDL_JoystickOpen(0);
		std::cout << SDL_JoystickName(0) << std::endl;
		SDL_JoystickEventState(SDL_ENABLE);
	}
	else
	{
		std::cout << "There is no joystick" << std::endl;
	}
	
	if((m_display = SDL_SetVideoMode(m_width,m_height,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL)) == NULL)
	{
		return false;
	}
	
	m_renderer = new Renderer(m_width,m_height);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_WarpMouse(m_width/2, m_height/2);
	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	SDL_ShowCursor(SDL_DISABLE); 
	
	SDL_EnableKeyRepeat(10, 10);
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

	for(unsigned int i = 0; i < m_input_keys.size(); ++i)
	{
		if(m_input_keys.at(i))
		{
			m_renderer->get_rig()->update_position(i);
		}
	}
	
	if(m_joystick != NULL)
	{
		int epsilon = 4000;
		//~ Forward / Backward
		if(SDL_JoystickGetAxis(m_joystick, 1) > epsilon) m_renderer->get_rig()->update_position(0);
		if(SDL_JoystickGetAxis(m_joystick, 1) < -epsilon) m_renderer->get_rig()->update_position(1);
		//~ Left / Right
		if(SDL_JoystickGetAxis(m_joystick, 0) < -epsilon) m_renderer->get_rig()->update_position(2);
		if(SDL_JoystickGetAxis(m_joystick, 0) > epsilon) m_renderer->get_rig()->update_position(3);
		
		int delta = 10;
		//~ Eyes to the left / Right
		if(SDL_JoystickGetAxis(m_joystick, 2) < -epsilon) m_mouse_x -= delta;
		if(SDL_JoystickGetAxis(m_joystick, 2) > epsilon) m_mouse_x += delta;
		//~ Eyes to the ground / sky
		if(SDL_JoystickGetAxis(m_joystick, 3) < -epsilon) m_mouse_y -= delta;
		if(SDL_JoystickGetAxis(m_joystick, 3) > epsilon) m_mouse_y += delta;
	}

	m_renderer->get_rig()->update_horizontal_angle(m_mouse_x);
	m_renderer->get_rig()->update_vertical_angle(m_mouse_y);
	m_renderer->get_rig()->update_target();
	
	m_renderer->get_camera_one()->compute_view_matrix();
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
			case SDLK_z :	m_input_keys.at(1) = true;
			break;
			//~ Backward
			case SDLK_s : 	m_input_keys.at(0) = true;
			break;
			//~ Left
			case SDLK_q : 	m_input_keys.at(2) = true;
			break;
			//~ Right
			case SDLK_d : 	m_input_keys.at(3) = true;
			break;
			case SDLK_o : m_renderer->reset_cameras_dioc();
			break;
			default : ;
			break;
		}
	}
	if(Event->type == SDL_KEYUP)
	{
		switch(Event->key.keysym.sym)
		{
			case SDLK_ESCAPE: m_running = false;
			break;
			//~ Forward
			case SDLK_z :	m_input_keys.at(1) = false;
			break;
			//~ Backward
			case SDLK_s : 	m_input_keys.at(0) = false;
			break;
			//~ Left
			case SDLK_q : 	m_input_keys.at(2) = false;
			break;
			//~ Right
			case SDLK_d : 	m_input_keys.at(3) = false;
			break;
			default : ;
			break;
		}
	}
	if(Event->type == SDL_MOUSEBUTTONDOWN)
	{
		float delta = 0.01;
		if (Event->button.button == SDL_BUTTON_WHEELUP)
		{
			m_renderer->set_cameras_dioc(delta);
		}
		if (Event->button.button == SDL_BUTTON_WHEELDOWN)
		{
			m_renderer->set_cameras_dioc(-delta);
		}
	}
	if(Event->type == SDL_JOYBUTTONDOWN)
	{
		if((int)Event->jbutton.button == 0)
		{
			m_renderer->reset_cameras_dioc();
		}
		if((int)Event->jbutton.button == 7)
		{
			m_running = false;
		}
	}
	if(Event->type == SDL_JOYHATMOTION)
	{
		float delta = 0.01;
		if(Event->jhat.value == SDL_HAT_UP)
		{
			m_renderer->set_cameras_dioc(delta);
			
		}
		if(Event->jhat.value == SDL_HAT_DOWN)
		{
			m_renderer->set_cameras_dioc(-delta);
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
	if(m_joystick != NULL) SDL_JoystickClose(m_joystick);
	SDL_FreeSurface(m_display);
	SDL_Quit();
}
