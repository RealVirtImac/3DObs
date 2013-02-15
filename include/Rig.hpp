#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.hpp"

#include <iostream>
#include <cmath>

class Rig
{
	public:
		Rig(Camera* camera_one, Camera* camera_two, glm::vec3 position, glm::vec3 up, glm::vec3 target, int width, int height);
		~Rig();
		
		void set_position(const glm::vec3 position);
		void set_up(const glm::vec3 up);
		void set_target(const glm::vec3 target);
		
		void update_position(const int up_down);
		void update_horizontal_angle(const int mouse_x);
		void update_vertical_angle(const int mouse_y);
		void update_target();
		
		glm::vec3 get_position() const;
		
	private:
		Camera* m_camera_one;
		Camera* m_camera_two;
		
		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target;
		glm::vec3 m_right;
		
		float m_speed;
		float m_mouse_speed;
		float m_horizontal_angle;
		float m_vertical_angle;
		
		int m_width;
		int m_height;
};
