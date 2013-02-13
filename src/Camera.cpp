#include "../include/Camera.hpp"

Camera::Camera(glm::vec3 cam_one_position, glm::vec3 cam_one_up, glm::vec3 cam_one_target, int type):
        m_type(type),
        m_fov(45.0f),
	m_ratio(1280.0/720.0),
	m_near(1.0f),
	m_far(100.0f),
	m_position(cam_one_position),
	m_up(cam_one_up),
	m_target(cam_one_target),
	m_horizontal_angle(M_PI),
	m_vertical_angle(0.0f),
	m_speed(0.15f),
	m_mouse_speed(0.005f)
{    
	compute_view_matrix();
	compute_projection_matrix();
}

Camera::~Camera()
{
}

void Camera::compute_view_matrix()
{
	m_view_matrix = glm::lookAt(m_position,m_position+m_target,m_up);
}

void Camera::compute_projection_matrix()
{
        //CONST TPS
        float dc = 2.0;
        float dioc = 0.65;
        //!CONST TPS

        //Frustum (left camera)
        if (m_type ==0)
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
        else if (m_type == 1)
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
        //m_projection_matrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
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

void Camera::update_horizontal_angle(const int mouse_x)
{
	m_horizontal_angle += (m_mouse_speed * float(1280.0/2 - mouse_x));
}

void Camera::update_vertical_angle(const int mouse_y)
{
	m_vertical_angle += (m_mouse_speed * float(720.0/2 - mouse_y));
}

void Camera::update_target()
{
	m_target = glm::vec3(cos(m_vertical_angle) * sin(m_horizontal_angle), sin(m_vertical_angle), cos(m_vertical_angle) * cos(m_horizontal_angle));
}

void Camera::compute_right()
{
	glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
	m_up = glm::cross(right, m_target);
}

void Camera::update_position(const int up_down)
{
	switch(up_down)
	{
		//~ Forward
		case 0 : m_position -= m_target * m_speed;
		break;
		//~ Backward
		case 1 : m_position += m_target * m_speed;
		break;
		//~ Left
		case 2 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_position -= right * m_speed;
					}
		break;
		//~ Right
		case 3 :	{
						glm::vec3 right = glm::vec3(sin(m_horizontal_angle - M_PI/2.0f), 0, cos(m_horizontal_angle - M_PI/2.0f));
						m_position += right * m_speed;
					}
		break;
	}
}
