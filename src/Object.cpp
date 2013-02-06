#include "../include/Object.hpp"

Object::Object(const char* filename)
{
	//~ Loading OBJ
	load_OBJ(filename, m_vertices, m_uvs, m_normals);
	//~ Initializing model matrix
	m_model_matrix = glm::mat4(1.0);
	//~ Creating buffers
	create_buffers();
}

Object::~Object()
{
	glDeleteBuffers(1,&m_object_vertices_vbo);
	glDeleteBuffers(1,&m_object_normals_vbo);
	glDeleteBuffers(1,&m_object_uvs_vbo);
	glDeleteVertexArrays(1, &m_object_vao);
}

void Object::create_buffers()
{
	// Generating Vertex Array
	glGenVertexArrays(1, &m_object_vao);
	// Generating buffers
	glGenBuffers(1, &m_object_vertices_vbo);
	glGenBuffers(1, &m_object_normals_vbo);
	glGenBuffers(1, &m_object_uvs_vbo);
	// Binding vao
	glBindVertexArray(m_object_vao);
	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_object_vertices_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, m_object_normals_vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
    // Uvs
    glBindBuffer(GL_ARRAY_BUFFER, m_object_uvs_vbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) , (void*)0);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
	// Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//~ Getters
GLuint Object::get_vao() const
{
	return m_object_vao;
}

unsigned int Object::get_size() const
{
	return m_vertices.size();
}

glm::mat4 Object::get_model_matrix() const
{
	return m_model_matrix;
}
