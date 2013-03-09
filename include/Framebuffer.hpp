#pragma once

//!  Framebuffers
/*!
  * \author R. Bertozzi & S. Bougeois
  * \brief Contains one or several textures of rendered images
  * \file Framebuffer.hpp
*/

#include <GL/glew.h>
#include <iostream>
#include <vector>

/*!
 * \brief Contains one or several textures of rendered images
 */ 
class Framebuffer
{
	public :
		//! Constructor
		/*!
		 * Initializes several elements such as the number of color textures and the width / height of the framebuffer
		 * \param number_of_color_textures Number of color textures
		 * \param width Width of the window
		 * \param height Height of the window
		 */ 
		Framebuffer(const int number_of_color_textures, const unsigned int width, const unsigned int weight);
		//! Destuctor
		~Framebuffer();
		
		//! Gets the framebuffer id
		/*!
		 * Returns the id of the framebuffer
		 * \return The id of the framebuffer
		 */ 
		GLuint get_framebuffer_id() const;
		
		//! Gets the array containing all the textures ids
		/*!
		 * Returns the array containing all the textures ids
		 * \return The array containing all the textures ids
		 */ 
		GLuint* get_texture_color_id() const;
		
	private :
		GLuint m_framebuffer_id;
		GLuint* m_texture_color_id;
		GLuint m_depth_texture_id;
		GLenum* m_draw_buffers;
		
		unsigned int m_number_of_color_textures;
		unsigned int m_width;
		unsigned int m_height;
};
