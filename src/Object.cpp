#include "../include/Object.hpp"

Object::Object(const char* filename)
{
	//~ Loading OBJ
	load_OBJ(filename, m_vertices, m_uvs, m_normals);
	//~ Initializing model matrix
	m_model_matrix = glm::scale(glm::mat4(1.0),glm::vec3(0.025f,0.025f,0.025f));
	//~ Creating buffers
	create_buffers();
	//~ Load texture
	load_textures();
}

Object::~Object()
{
	glDeleteTextures(1, &m_diffuse_texture);
	SDL_FreeSurface(m_diffuse_texture_surface);
	
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

void Object::load_textures()
{
	if((m_diffuse_texture_surface = SDL_LoadBMP("textures/cube_map_tex.bmp")))
	{
		//~ Properties
		GLenum texture_format;
		GLint nb_of_colors = m_diffuse_texture_surface->format->BytesPerPixel;
        if (nb_of_colors == 4)
        {
                if (m_diffuse_texture_surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nb_of_colors == 3)
        {
                if (m_diffuse_texture_surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        }
		//~
		glGenTextures(1, &m_diffuse_texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, nb_of_colors, m_diffuse_texture_surface->w, m_diffuse_texture_surface->h, 0, texture_format, GL_UNSIGNED_BYTE, m_diffuse_texture_surface->pixels );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
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
