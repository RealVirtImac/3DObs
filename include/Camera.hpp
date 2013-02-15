#pragma once

#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
	public:
		Camera(int width, int height, int type);
		~Camera();
		
		void compute_view_matrix();
		void compute_projection_matrix();
		
		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;
		glm::vec3 get_position() const;
		
		void set_position(const glm::vec3 position);
		void set_target(const glm::vec3 target);
		void set_up(const glm::vec3 up);
		void set_horizontal_angle(float angle);
		void set_vertical_angle(float angle);
		
		void update_target();
		void compute_right();
	
	private:
		int m_type;
		float m_fov;
		float m_ratio;
		float m_near;
		float m_far;
		
		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target;
		glm::vec3 m_right;
		
		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;
		
		float m_horizontal_angle;
		float m_vertical_angle;
		float m_speed;
		float m_mouse_speed;
		
		int m_width;
		int m_height;
};
