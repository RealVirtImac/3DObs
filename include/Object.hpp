#pragma once

//!  Object that can be instanced in the scene
/*!
  * \author R. Bertozzi & S. Bougeois
  * \brief Object that can be instanced in the scene
  * \file Object.hpp
*/

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <SDL/SDL.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

#include "ObjLoader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

/*!
 * \brief Object that can be instanced in the scene
 */ 
class Object
{
	public:
		//! Constructor with texture
		/*!
		 *	\param filename Path of the obj model to load
		 *	\param texture_path Path of the texture to load
		 */
		Object(const char* filename, const char* texture_path);
		//! Destuctor
		~Object();
		
		//! Creates all the required buffers for the objects (vertices, normals, uvs) and the associated VAO
		void create_buffers();
		//! Loads the textures thanks to SDL_LoadBMP
		void load_textures();

                //! Compute and return the barycentre of the object
                glm::vec3 computeBarycentre();

                //! Compute and return the standard deviation
                float computeStandardDeviation();

                //! Compute and return the average dist to the barycentre
                float computeAvgDistToBarycentre();
		
		//! Gets the identifier of the diffuse texture
		/*!
		 * \return The identifier of the diffuse texture
		 */ 
		GLuint get_diffuse_texture() const;
		//! Gets the path of the diffuse texture
		/*!
		 * \return The path of the diffuse texture
		 */ 
		const char* get_texture_path() const;
		//! Gets the number of vertices in the model
		/*!
		 * \return The number of vertices in the model
		 */ 
		unsigned int get_size() const;
		//! Gets the identifier of the VAO
		/*!
		 * \return The identifier of the VAO
		 */ 
		GLuint get_vao() const;
		//! Gets the Model matrix
		/*!
		 * \return Model matrix
		 */ 
		glm::mat4 get_model_matrix() const;
		
		//! Sets the model matrix of the object
		void set_model_matrix(const glm::mat4 input_matrix);
		
	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_uvs;
		
		glm::mat4 m_model_matrix;
		
		GLuint m_object_vao;
		GLuint m_object_vertices_vbo;
		GLuint m_object_normals_vbo;
		GLuint m_object_uvs_vbo;
		
		const char* m_texture_path;
		SDL_Surface* m_diffuse_texture_surface;
		GLuint m_diffuse_texture;
};
