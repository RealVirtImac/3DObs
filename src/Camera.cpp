/*!
 * \file Camera.cpp
 * \brief Camera that is viewing the scene
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Camera.hpp"
#include <iostream>

Camera::Camera(int width, int height, int type):
	m_type(type),
	m_fov(45.0f),
	m_ratio((float)width/(float)height),
        m_near(2.0f*(2.0f/3.0f)),
        //m_far(2.0f + 2.0f*(1.0f/3.0f)), //theorically
        m_far(100.0f),
	m_horizontal_angle(M_PI),
	m_vertical_angle(0.0f),
	m_width(width),
	m_height(height)
{
}

Camera::~Camera()
{
}

//~ Computations
void Camera::compute_view_matrix()
{
	m_view_matrix = glm::lookAt(m_position,m_position+m_target,m_up);
}

void Camera::compute_projection_matrix()
{
	float dc = 2.0;
	float L = 4.0;
	float H = L/m_ratio;

	//Frustum (left camera)
	if (m_type == 1)
	{
		float top, bottom, left, right;
		top = (m_near*H)/(2*dc);
		bottom = -top;
		left = (m_near*(m_dioc-L))/(2*dc);
		right = (m_near*(m_dioc+L))/(2*dc);

		m_projection_matrix = glm::frustum(left, right, bottom, top, m_near, m_far);
	}

	//Frustum (right camera)
	else if (m_type == 0)
	{
		float top, bottom, left, right;
		top = (m_near*H)/(2*dc);
		bottom = -top;
		left = (m_near*(m_dioc+L))/(2*dc);
		right = (m_near*(m_dioc-L))/(2*dc);

		m_projection_matrix = glm::frustum(-left, -right, bottom, top, m_near, m_far);
	}
}

//~ Getters
glm::mat4 Camera::get_view_matrix() const
{
	return m_view_matrix;
}

glm::mat4 Camera::get_projection_matrix() const
{
	return m_projection_matrix;
}

glm::vec3 Camera::get_position() const
{
	return m_position;
}

float Camera::get_fov() const
{
	return m_fov;
}

float Camera::get_ratio() const
{
	return m_ratio;
}

float Camera::get_near() const
{
	return m_near;
}

float Camera::get_far() const
{
	return m_far;
}

float Camera::get_dioc() const
{
	return m_dioc;
}

float Camera::get_horizontal_angle() const
{
	return m_horizontal_angle;
}

float Camera::get_vertical_angle() const
{
	return m_vertical_angle;
}


glm::vec3 Camera::get_target() const
{
	return m_target;
}

//~ Setters
void Camera::set_position(const glm::vec3 position)
{
	m_position = position;
}

void Camera::set_target(const glm::vec3 target)
{
	m_target = target;
}

void Camera::set_up(const glm::vec3 up)
{
	m_up = up;
}

void Camera::set_horizontal_angle(const float angle)
{
	m_horizontal_angle = angle;
}

void Camera::set_vertical_angle(const float angle)
{
	m_vertical_angle = angle;
}

void Camera::set_dioc(const float dioc)
{
	m_dioc = dioc;
}

void Camera::set_right(const glm::vec3 right)
{
	m_right = right;
}
