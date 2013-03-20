 /*!
 * \file Application.cpp
 * \brief Creates the context of the program
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Application.hpp"

Application::Application():
	m_running(true),
	m_display(NULL),
	m_joystick(NULL),
	m_display_gui(true),
	m_has_focus_changed(false)
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

	for(int i = 0; i < 6; ++i)
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
	
	if((m_display = SDL_SetVideoMode(m_width,m_height,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL|SDL_FULLSCREEN)) == NULL)
	{
		return false;
	}
	
	m_renderer = new Renderer(m_width,m_height);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_EnableUNICODE(SDL_ENABLE);
	
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
	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);

	for(unsigned int i = 0; i < m_input_keys.size(); ++i)
	{
		if(m_input_keys.at(i))
		{
			m_renderer->get_rig()->update_position(i,1.0f);
		}
	}
	
	if(m_joystick != NULL)
	{
		int epsilon = 3000;
		//~ //Forward / Backward
		if(SDL_JoystickGetAxis(m_joystick, 1) > epsilon) m_renderer->get_rig()->update_position(0,abs(SDL_JoystickGetAxis(m_joystick, 1)/10000.0));
		if(SDL_JoystickGetAxis(m_joystick, 1) < -epsilon) m_renderer->get_rig()->update_position(1,abs(SDL_JoystickGetAxis(m_joystick, 1)/10000.0));
		//~ //Left / Right
		if(SDL_JoystickGetAxis(m_joystick, 0) < -epsilon) m_renderer->get_rig()->update_position(2,abs(SDL_JoystickGetAxis(m_joystick, 0)/10000.0));
		if(SDL_JoystickGetAxis(m_joystick, 0) > epsilon) m_renderer->get_rig()->update_position(3,abs(SDL_JoystickGetAxis(m_joystick, 0)/10000.0));
		
		//~ //Eyes to the left / Right	
		if(SDL_JoystickGetAxis(m_joystick, 2) < -epsilon) m_mouse_x -= (abs(SDL_JoystickGetAxis(m_joystick, 2)/1000.0));
		if(SDL_JoystickGetAxis(m_joystick, 2) > epsilon) m_mouse_x += (abs(SDL_JoystickGetAxis(m_joystick, 2)/1000.0));
		//~ //Eyes to the ground / sky
		if(SDL_JoystickGetAxis(m_joystick, 3) < -epsilon) m_mouse_y -= (abs(SDL_JoystickGetAxis(m_joystick, 3)/1000.0));
		if(SDL_JoystickGetAxis(m_joystick, 3) > epsilon) m_mouse_y += (abs(SDL_JoystickGetAxis(m_joystick, 3)/1000.0));
	}
	
	if(m_display_gui)
	{
		m_renderer->get_rig()->update_horizontal_angle(m_display->w/2);
		m_renderer->get_rig()->update_vertical_angle(m_display->h/2);
		
		m_renderer->set_display_gui(true);
		
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		SDL_WarpMouse(m_display->w/2, m_display->h/2);
		if(m_has_focus_changed)
		{
			#ifdef _WIN32
				m_mouse_x = m_display->w/2;
				m_mouse_y = m_display->h/2;
			#elif __linux
				m_mouse_x = 0.0f;
				m_mouse_y = 0.0f;
			#endif
			m_has_focus_changed = !m_has_focus_changed;
		}
		m_renderer->get_rig()->update_horizontal_angle(m_mouse_x);
		m_renderer->get_rig()->update_vertical_angle(m_mouse_y);
		
		m_renderer->set_display_gui(false);
		
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	m_renderer->get_rig()->update_target();
	m_renderer->get_rig()->update_up();
	
	m_renderer->get_rig()->get_camera_one()->compute_view_matrix();
	m_renderer->get_rig()->get_camera_two()->compute_view_matrix();
}

void Application::on_event(SDL_Event* Event)
{
	if(Event->type == SDL_QUIT)
	{
		m_running = false;
	}
	if(Event->type == SDL_MOUSEBUTTONDOWN)
	{
		if(Event->button.button == SDL_BUTTON_RIGHT)
		{
			m_display_gui = !m_display_gui;
			if(!m_display_gui)
			{
				m_has_focus_changed = !m_has_focus_changed;
			}
		}
	}
	if(Event->type == SDL_KEYDOWN)
	{
	    if(m_renderer->get_keyboard_layout() == 1)
        {
            //~ Forward
            if(Event->key.keysym.sym == 273) m_input_keys.at(1) = true;
            //~ Backward
            if(Event->key.keysym.sym == 274) m_input_keys.at(0) = true;
            //~ Left
            if(Event->key.keysym.sym == 276) m_input_keys.at(2) = true;
            //~ Right
            if(Event->key.keysym.sym == 275) m_input_keys.at(3) = true;
        }
         if(m_renderer->get_keyboard_layout() == 0)
        {
			//~ Forward
            if(Event->key.keysym.sym == SDLK_z) m_input_keys.at(1) = true;
            //~ Backward
            if(Event->key.keysym.sym == SDLK_s) m_input_keys.at(0) = true;
            //~ Left
            if(Event->key.keysym.sym == SDLK_q) m_input_keys.at(2) = true;
            //~ Right
            if(Event->key.keysym.sym == SDLK_d) m_input_keys.at(3) = true;
        }
		switch(Event->key.keysym.sym)
		{
			case SDLK_ESCAPE:
			{
				m_running = false;
			}
			break;
			//~ Up
			case SDLK_t : 	m_input_keys.at(4) = true;
			break;
			//~ Down
			case SDLK_g : 	m_input_keys.at(5) = true;
			break;
			default : ;
			break;
		}
	}
	if(Event->type == SDL_KEYUP)
	{
	    if(m_renderer->get_keyboard_layout() == 1)
        {
            //~ Forward
            if(Event->key.keysym.sym == 273) m_input_keys.at(1) = false;
            //~ Backward
            if(Event->key.keysym.sym == 274) m_input_keys.at(0) = false;
            //~ Left
            if(Event->key.keysym.sym == 276) m_input_keys.at(2) = false;
            //~ Right
            if(Event->key.keysym.sym == 275) m_input_keys.at(3) = false;
        }
         if(m_renderer->get_keyboard_layout() == 0)
        {
			//~ Forward
            if(Event->key.keysym.sym == SDLK_z) m_input_keys.at(1) = false;
            //~ Backward
            if(Event->key.keysym.sym == SDLK_s) m_input_keys.at(0) = false;
            //~ Left
            if(Event->key.keysym.sym == SDLK_q) m_input_keys.at(2) = false;
            //~ Right
            if(Event->key.keysym.sym == SDLK_d) m_input_keys.at(3) = false;
        }
		switch(Event->key.keysym.sym)
		{
			case SDLK_ESCAPE: m_running = false;
			break;
			//~ Up
			case SDLK_t : 	m_input_keys.at(4) = false;
			break;
			//~ Down
			case SDLK_g : 	m_input_keys.at(5) = false;
			break;
			//~ Switch view mode
			case SDLK_v :
				if (m_renderer->get_view_mode() == 0)
					m_renderer->set_view_mode(1);
				else  if (m_renderer->get_view_mode() == 1)
					m_renderer->set_view_mode(0);
			break;
			//~ Arrow up
                        case SDLK_p : m_renderer->get_rig()->change_dioc(0.005,m_renderer->get_dc(), m_renderer->get_l());
			break;
			//~ Arrow down
                        case SDLK_l : m_renderer->get_rig()->change_dioc(-0.005,m_renderer->get_dc(), m_renderer->get_l());
			break;
                        case SDLK_o : m_renderer->get_rig()->reset_dioc(m_renderer->get_dc(), m_renderer->get_l());
			break;
			default : ;
			break;
		}
	}
	if(Event->type == SDL_JOYBUTTONDOWN)
	{
		if((int)Event->jbutton.button == 7)
		{
			m_running = false;
		}
		if((int)Event->jbutton.button == 0)
		{
                        m_renderer->get_rig()->reset_dioc(m_renderer->get_dc(), m_renderer->get_l());
		}
	}
	if(Event->type == SDL_JOYHATMOTION)
	{
		if(Event->jhat.value & SDL_HAT_UP)
		{
                        m_renderer->get_rig()->change_dioc(0.005,m_renderer->get_dc(), m_renderer->get_l());
		}
		if(Event->jhat.value & SDL_HAT_DOWN)
		{
                        m_renderer->get_rig()->change_dioc(-0.005,m_renderer->get_dc(), m_renderer->get_l());
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
