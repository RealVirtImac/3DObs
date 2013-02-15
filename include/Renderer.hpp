#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Rig.hpp"

class Renderer
{
	public:
		Renderer(int width, int height);
		~Renderer();
		
		void render();
		
		Rig* get_rig() const;
		Camera* get_camera_one() const;
		Camera* get_camera_two() const;
		void switch_drawing_mode();
		
	private:
		bool m_drawing_mode;
	
		int m_width;
		int m_height;
	
		const char* readFile(const char* filePath);
		GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
		
		Rig* m_rig;
		Object* m_object;
		Object* m_quad;
		Camera* m_camera_one;
		Camera* m_camera_two;
		
		GLuint m_basic_shader_program;
		GLuint m_basic_shader_model_matrix_position;
		GLuint m_basic_shader_view_matrix_position;
		GLuint m_basic_shader_projection_matrix_position;
		
		GLuint m_lighting_shader_program;
		GLuint m_lighting_shader_model_matrix_position;
		GLuint m_lighting_shader_view_matrix_position;
		GLuint m_lighting_shader_projection_matrix_position;
		GLuint m_lighting_shader_camera_position;
		GLuint m_lighting_shader_diffuse_texture;
		
		GLuint m_lighting_no_texture_shader_program;
		GLuint m_lighting_no_texture_shader_model_matrix_position;
		GLuint m_lighting_no_texture_shader_view_matrix_position;
		GLuint m_lighting_no_texture_shader_projection_matrix_position;
		GLuint m_lighting_no_texture_shader_camera_position;
		
		GLuint m_quad_shader;
		GLuint m_quad_shader_texture_1;
		GLuint m_quad_shader_texture_2;
		
		GLuint m_left_framebuffer;
		GLuint m_left_render_texture;
		GLuint m_left_depth_texture;
		GLenum m_left_draw_buffers[1];
		
		GLuint m_right_framebuffer;
		GLuint m_right_render_texture;
		GLuint m_right_depth_texture;
		GLenum m_right_draw_buffers[1];
};
