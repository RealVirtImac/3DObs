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
    //FOV * PI / 180.0f;
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
}

Camera::~Camera()
{
}

//~ Computations
void Camera::compute_view_matrix()
{
    //std::cout<<this->m_type<<" compute_view_matrix : "<<m_position.x<<", "<<m_position.y<<", "<<m_position.z<<"dioc : "<<m_dioc<<std::endl;
        m_view_matrix = glm::lookAt(m_position,m_position+m_target,m_up);
}

void Camera::compute_projection_matrix()
{
        float dc = 2.0;
        float L = 4;
        float H = L/m_ratio;

        //Frustum (left camera)

        if (m_type == 1)
        {
                /*float top, bottom, left, right;
                top = m_near * tan(m_fov/2);
                bottom = -top;
                float a = m_ratio * top * dc;
                float b = a - m_dioc/2;
                float c = a + m_dioc/2;
                left = -b * m_near/dc;
                right   =  c * m_near/dc;*/

                //float top, bottom, left, right;
                //top = m_near * tan(m_fov/2);
                //bottom = -top;
                //right = m_ratio * top;
                //double frustumshift = (m_dioc/2)/**m_near/m_fov*/;
                //left = -right + frustumshift;
                //right = right + frustumshift;

            //left = m_near/(2*dc) * (dc + m_dioc);
            //right = m_near/(2*dc) * (dc - m_dioc);

            float top, bottom, left, right;
            top = (m_near*H)/(2*dc);
            bottom = -top;
            left = (m_near*(m_dioc-L))/(2*dc);
            right = (m_near*(m_dioc+L))/(2*dc);
            //left = (m_near * (2-m_dioc)) /(2*(dc));
            //right = (m_near * (2+m_dioc)) /(2*(dc));

                std::cout<<m_type<<" frustum : "<<left<<" & "<<right<<" & "<<top<<std::endl;
                std::cout<<m_type<<" pos : "<<m_position.x<<", "<<m_position.y<<", "<<m_position.z<<std::endl;
                std::cout<<abs(left) + abs(right)<<std::endl;

                m_projection_matrix = glm::frustum(left, right, bottom, top, m_near, m_far);
                //m_projection_matrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
                //m_projection_matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(m_dioc/2, 0.0f, 0.0f));
	}

	//Frustum (right camera)
	else if (m_type == 0)
	{
                /*float top, bottom, left, right;
		top = m_near * tan(m_fov/2);
		bottom = -top;
		float a = m_ratio * tan(m_fov/2) * dc;
		float b = a - m_dioc/2;
		float c = a + m_dioc/2;
		left = -c * m_near/dc;
                right   =  b * m_near/dc;*/

            //float top, bottom, left, right;
            //top = m_near * tan(m_fov/2);
            //bottom = -top;
            //right = m_ratio * top;
            //double frustumshift = (m_dioc/2)/**m_near/m_fov*/;
            //left = -right - frustumshift;
            //right = right - frustumshift;

            //left = m_near/(2*dc) * (dc - m_dioc);
            //right = m_near/(2*dc) * (dc + m_dioc);

            float top, bottom, left, right;
            top = (m_near*H)/(2*dc);
            bottom = -top;
            left = (m_near*(m_dioc+L))/(2*dc);
            right = (m_near*(m_dioc-L))/(2*dc);
            //right = (m_near * (2-m_dioc)) /(2*(dc));
            //left = (m_near * (2+m_dioc)) /(2*(dc));

                std::cout<<m_type<<" frustum : "<<left<<" & "<<right<<" & "<<top<<std::endl;
                std::cout<<m_type<<" pos : "<<m_position.x<<", "<<m_position.y<<", "<<m_position.z<<std::endl;

                std::cout<<abs(left) + abs(right)<<std::endl;

                m_projection_matrix = glm::frustum(left, right, bottom, top, m_near, m_far);
               // m_projection_matrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
                //m_projection_matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(m_dioc/2, 0.0f, 0.0f));
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

//~ Setters
void Camera::set_position(const glm::vec3 position)
{
    std::cout<<position.x<<", "<<position.y<<", "<<position.z<<std::endl;
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
