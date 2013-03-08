/*!
 * \file Rig.cpp
 * \brief The rig that maintains the cameras in a single entity
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Rig.hpp"

Rig::Rig(Camera* camera_one, Camera* camera_two, glm::vec3 position, float dioc, glm::vec3 up, glm::vec3 target, int width, int height):
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
	m_camera_one->set_dioc(dioc);
	m_camera_two->set_dioc(dioc);

	m_camera_one->set_target(m_target);
	m_camera_two->set_target(m_target);
	
	m_camera_one->set_up(m_up);
	m_camera_two->set_up(m_up);

	m_camera_one->compute_projection_matrix();
	m_camera_two->compute_projection_matrix();

	m_camera_one->set_position(m_position);
	m_camera_two->set_position(m_position);
}


Rig::~Rig()
{
}

void Rig::update_position(const int direction, const float delta)
{
	glm::vec3 orthogonal_vector = glm::normalize(glm::cross(m_up,m_target));
	glm::vec3 displacement = glm::vec3(0.0,0.0,0.0);
	
	switch(direction)
	{
		//~ Backward
		case 0 : 	m_position -= m_target * (m_speed * delta);
					displacement = (m_camera_one->get_dioc()/2)*orthogonal_vector;
					
					m_camera_one->set_position(m_position - displacement);
					m_camera_two->set_position(m_position + displacement);
		break;
		//~ Forward
		case 1 : 	m_position += m_target * (m_speed * delta);
					displacement = (m_camera_one->get_dioc()/2)*orthogonal_vector;
					
					m_camera_one->set_position(m_position - displacement);
					m_camera_two->set_position(m_position + displacement);
		break;
		//~ Left
		case 2 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_right = glm::normalize(right);

						m_position -= right * (m_speed * delta);

						glm::vec3 displacement_camera = (m_camera_one->get_dioc()/2)*orthogonal_vector;
						
						m_camera_one->set_position(m_position - displacement_camera);
						m_camera_two->set_position(m_position + displacement_camera);
					}
		break;
		//~ Right
		case 3 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_right = glm::normalize(right);

						m_position += right * (m_speed * delta);

						glm::vec3 displacement_camera = (m_camera_one->get_dioc()/2)*orthogonal_vector;
						
						m_camera_one->set_position(m_position - displacement_camera);
						m_camera_two->set_position(m_position + displacement_camera);
					}
		break;
		//~ Up
		case 4 :	{
						m_position.y += (m_speed * delta);
						displacement = (m_camera_one->get_dioc()/2)*orthogonal_vector;
						m_camera_one->set_position(m_position - displacement);
						m_camera_two->set_position(m_position + displacement);
					}
		break;
		//~ Down
		case 5 :	{
						m_position.y -= (m_speed * delta);
						displacement = (m_camera_one->get_dioc()/2)*orthogonal_vector;
						m_camera_one->set_position(m_position - displacement);
						m_camera_two->set_position(m_position + displacement);
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

//~ Getters
glm::vec3 Rig::get_position() const
{
	return m_position;
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

glm::vec3 Rig::get_target() const
{
	return m_target;
}
