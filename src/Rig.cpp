#include "../include/Rig.hpp"

Rig::Rig(Camera* camera_one, Camera* camera_two, glm::vec3 position, glm::vec3 up, glm::vec3 target, int width, int height):
	m_camera_one(camera_one),
	m_camera_two(camera_two),
	m_position(position),
	m_up(up),
	m_target(target),
	m_speed(0.15f),
	m_mouse_speed(0.005f),
	m_horizontal_angle(M_PI),
	m_vertical_angle(0.0f),
	m_width(width),
	m_height(height)
{
	m_camera_one->set_position(m_position);
	m_camera_two->set_position(m_position);
	
	m_camera_one->set_target(m_target);
	m_camera_two->set_target(m_target);
	
	m_camera_one->set_up(m_up);
	m_camera_two->set_up(m_up);
	
	m_camera_one->compute_view_matrix();
	m_camera_two->compute_view_matrix();
}

Rig::~Rig()
{
}

void Rig::update_position(const int up_down)
{
	switch(up_down)
	{
		//~ Forward
		case 0 : 	m_position -= m_target * m_speed;
					m_camera_one->set_position(m_position);
					m_camera_two->set_position(m_position);
		break;
		//~ Backward
		case 1 : 	m_position += m_target * m_speed;
					m_camera_one->set_position(m_position);
					m_camera_two->set_position(m_position);
		break;
		//~ Left
		case 2 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_right = right;
						m_position -= right * m_speed;
						m_camera_one->set_position(m_position);
						m_camera_two->set_position(m_position);
					}
		break;
		//~ Right
		case 3 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_right = right;
						m_position += right * m_speed;
						m_camera_one->set_position(m_position);
						m_camera_two->set_position(m_position);
					}
		break;
	}
}

void Rig::update_horizontal_angle(const int mouse_x)
{
	m_horizontal_angle += (m_mouse_speed * float(m_width/2.0 - mouse_x));
	m_camera_one->set_horizontal_angle(m_horizontal_angle);
	m_camera_two->set_horizontal_angle(m_horizontal_angle);
}

void Rig::update_vertical_angle(const int mouse_y)
{
	m_vertical_angle += (m_mouse_speed * float(m_height/2.0 - mouse_y));
	m_camera_one->set_vertical_angle(m_vertical_angle);
	m_camera_two->set_vertical_angle(m_vertical_angle);
}

void Rig::update_target()
{
	m_target = glm::vec3(cos(m_vertical_angle) * sin(m_horizontal_angle), sin(m_vertical_angle), cos(m_vertical_angle) * cos(m_horizontal_angle));
	m_camera_one->set_target(m_target);
	m_camera_two->set_target(m_target);
}

//~ Setters
void Rig::set_position(const glm::vec3 position)
{
	m_position = position;
}

void Rig::set_up(const glm::vec3 up)
{
	m_up = up;
}

void Rig::set_target(const glm::vec3 target)
{
	m_target = target;
}
