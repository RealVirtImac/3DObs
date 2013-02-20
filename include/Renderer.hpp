#pragma once

//!  Renderer of the context
/*!
  * \author R. Bertozzi & S. Bougeois
  * \brief Renderer of the context
  * \file Renderer.hpp
*/

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

/*!
 * \brief Renderer of the context
 */ 
class Renderer
{
	public:
		//! Constructor
		/*!
		 * Initializes several elements such as GLEW, the object, the quad, the shaders, the framebuffers...
		 * \param width Width of the window
		 * \param height Height of the window
		 */ 
		Renderer(int width, int height);
		//! Destructor
		~Renderer();
		
		//! Render the scene thanks to the two cameras and the framebuffers
		void render();
		
		//! Gets the rig maintaining the two cameras
		/*!
		 * \return The rig
		 */ 
		Rig* get_rig() const;
		//! Gets the first camera
		/*!
		 * \return The first camera
		 */ 
		Camera* get_camera_one() const;
		//! Gets the second camera
		/*!
		 * \return The second camera
		 */ 
		Camera* get_camera_two() const;
		
		//! Resets the cameras DIOC
		void reset_cameras_dioc();
		//! Set the cameras DIOC
		/*!
		 * \param delta The value of the iteration. Can be positive or negative
		 */ 
		void set_cameras_dioc(const float delta);
		
	private:
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
