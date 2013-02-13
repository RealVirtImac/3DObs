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
		Renderer();
		~Renderer();
		
		void render();
		
		Camera* get_camera_one() const;
                Camera* get_camera_two() const;
		
	private:
		const char* readFile(const char* filePath);
		GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
		
		Object* m_object;
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
};
