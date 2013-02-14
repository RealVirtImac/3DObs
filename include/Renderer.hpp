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

class Renderer
{
	public:
		Renderer(int width, int height);
		~Renderer();
		
		void render();
		
		Camera* get_camera_one() const;
		void switch_drawing_mode();
		
	private:
		bool m_drawing_mode;
	
		int m_width;
		int m_height;
	
		const char* readFile(const char* filePath);
		GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
		
		Object* m_object;
		Object* m_quad;
		Camera* m_camera_one;
		
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
		
		GLuint m_quad_shader;
		GLuint m_quad_shader_texture;
		
		GLuint m_framebuffer_name;
		GLuint m_left_render_texture;
		GLuint m_left_depth_texture;
		GLenum m_draw_buffers[1];
};
