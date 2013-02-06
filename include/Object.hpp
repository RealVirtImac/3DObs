#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "ObjLoader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Object
{
	public:
		explicit Object(const char* filename);
		~Object();
		
		void create_buffers();
		
		unsigned int get_size() const;
		GLuint get_vao() const;
		glm::mat4 get_model_matrix() const;
		
	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_uvs;
		
		glm::mat4 m_model_matrix;
		
		GLuint m_object_vao;
		GLuint m_object_vertices_vbo;
		GLuint m_object_normals_vbo;
		GLuint m_object_uvs_vbo;
};
