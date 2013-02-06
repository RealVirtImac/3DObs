#include "../include/Camera.hpp"

Camera::Camera(glm::vec3 cam_one_position, glm::vec3 cam_one_up, glm::vec3 cam_one_target):
	m_fov(90.0f),
	m_ratio(1280.0/720.0),
	m_near(0.01f),
	m_far(1000.0f),
	m_position(cam_one_position),
	m_up(cam_one_up),
	m_target(cam_one_target)
{
	compute_view_matrix();
	compute_projection_matrix();
}

Camera::~Camera()
{
}

void Camera::compute_view_matrix()
{
	m_view_matrix = glm::lookAt(m_position,m_target,m_up);
}

void Camera::compute_projection_matrix()
{
	m_projection_matrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
}

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

