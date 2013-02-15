#include "../include/Camera.hpp"

Camera::Camera(int width, int height, int type):
	m_type(type),
	m_fov(45.0f),
	m_ratio((float)width/(float)height),
	m_near(0.5f),
	m_far(100.0f),
	m_horizontal_angle(M_PI),
	m_vertical_angle(0.0f),
	m_speed(0.15f),
	m_mouse_speed(0.005f),
	m_width(width),
	m_height(height)
{
	compute_view_matrix();
	compute_projection_matrix();
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
	//CONST TPS
	float dc = 2.0;
	float dioc = 0.065;
	//!CONST TPS

	//Frustum (left camera)
	if (m_type == 1)
	{
		float top, bottom, left, right;
		top = m_near * tan(m_fov/2);
		bottom = -top;
		float a = m_ratio * tan(m_fov/2) * dc;
		float b = a - dioc/2;
		float c = a + dioc/2;
		left = -b * m_near/dc;
		right   =  c * m_near/dc;

		m_projection_matrix = glm::frustum(left, right, bottom, top, m_near, m_far);
		m_projection_matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(dioc/2, 0.0f, 0.0f));
	}

	//Frustum (right camera)
	else if (m_type == 0)
	{
		float top, bottom, left, right;
		top = m_near * tan(m_fov/2);
		bottom = -top;
		float a = m_ratio * tan(m_fov/2) * dc;
		float b = a - dioc/2;
		float c = a + dioc/2;
		left = -c * m_near/dc;
		right   =  b * m_near/dc;

		m_projection_matrix = glm::frustum(left, right, bottom, top, m_near, m_far);
		m_projection_matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(dioc/2, 0.0f, 0.0f));
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

void Camera::set_horizontal_angle(float angle)
{
	m_horizontal_angle = angle;
}

void Camera::set_vertical_angle(float angle)
{
	m_vertical_angle = angle;
}

