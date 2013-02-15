#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <SDL/SDL.h>

#include "ObjLoader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Object
{
	public:
		Object(const char* filename);
		Object(const char* filename, const char* texture_path);
		~Object();
		
		void create_buffers();
		void load_textures();
		
		GLuint get_diffuse_texture() const;
		const char* get_texture_path() const;
		unsigned int get_size() const;
		GLuint get_vao() const;
		glm::mat4 get_model_matrix() const;
		
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
