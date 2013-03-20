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
#include <SDL/SDL.h>
#include <dirent.h>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.hpp"
#include "Rig.hpp"
#include "Framebuffer.hpp"
#include "imgui/imgui.h"
#include "imgui/imguiRenderGL.h"

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
		
		//! Renders the scene thanks to the two cameras and the framebuffers
		void render();
		//! Finds all the available files in a directory
		/*!
		 * Finds all the available files in a directory using a specific pattern
		 * \param directory The directory where the search will be made
		 * \param container The container where the names of the files will be stored
		 */ 
		void find_available_files(const char* directory,std::vector<std::string> &container);
		//! Loads an object in the renderer
		/*!
		 * Loads an object in the renderer
		 * \param model The model to load
		 * \param texture The texture to load
		 */ 
		void load_object(const std::string model,const std::string texture);
		//! Gets the rig maintaining the two cameras
		/*!
		 * \return The rig
		 */ 
		Rig* get_rig() const;
		//! Gets the view mode
		/*!
		 * \return the view mode
		 */
		int get_view_mode() const;
		//! Gets the display gui mode
		/*!
		 * \return the display gui mode
		 */
		bool get_display_gui() const;
		//! Gets the distance between the camera and the virtual plane
		/*!
		 * \return the distance between the camera and the virtual plane
		 */
		float get_dc() const;
		//! Gets the keyboard's layout
		/*!
		 * \return the layout of the keyboard
		 */
        int get_keyboard_layout() const;
		//! Sets the view mode
		/*!
		 * \param mode the view mode
		 */
		void set_view_mode(const int mode);
		//! Sets the display gui mode
		/*!
		 * \param mode the display mode
		 */
		void set_display_gui(const bool mode);
		//! Sets the keyboard's layout
		/*!
		 * \param layout the identifier of the keyboard
		 */
		void set_keyboard_layout(const int layout);
		
	private:
		int m_width;
		int m_height;
	
		const char* readFile(const char* filePath);
		GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
		
		Rig* m_rig;
		Object* m_object;
		Object* m_quad_left;
		Object* m_quad_right;
		int m_view_mode;
		
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
		GLuint m_lighting_shader_light_intensity;
		GLuint m_lighting_shader_light_radius;
		
		GLuint m_lighting_no_texture_shader_program;
		GLuint m_lighting_no_texture_shader_model_matrix_position;
		GLuint m_lighting_no_texture_shader_view_matrix_position;
		GLuint m_lighting_no_texture_shader_projection_matrix_position;
		GLuint m_lighting_no_texture_shader_camera_position;
		GLuint m_lighting_no_texture_shader_light_intensity;
		GLuint m_lighting_no_texture_shader_light_radius;

		GLuint m_quad_shader;
		GLuint m_quad_shader_texture_1;
		GLuint m_quad_shader_texture_2;

		float m_lightIntensity;
		float m_radiusLight;
		
		Framebuffer* m_left_camera_framebuffer;
		Framebuffer* m_right_camera_framebuffer;
		
		bool m_display_gui;
		
		std::vector<std::string> m_list_of_models;
		std::vector<std::string> m_list_of_textures;
		
		bool m_gui_models_toggle;
		bool m_gui_textures_toggle;
		bool m_gui_keyboard_layout;
		int m_toggle;
		
		//~ Distance between camera and virtual screen
		float m_dc;
		
		std::string m_selected_model;
		std::string m_selected_texture;
		
		//0 : AZERTY
		//1 : QWERTY
		int m_keyboard_layout;
};
