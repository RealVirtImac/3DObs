/*!
 * \file Object.cpp
 * \brief Object that can be instanced in the scene
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Object.hpp"

Object::Object(const char* filename, const char* texture_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,aiProcess_FlipUVs | aiProcess_Triangulate);
	
	for(unsigned int n_mesh = 0; n_mesh < scene->mNumMeshes; ++n_mesh)
	{
		aiMesh* mesh = scene->mMeshes[n_mesh];
		
		for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];
			for(int j = 0; j < 3; ++j)
			{
				if(mesh->HasTextureCoords(0))
				{
					aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
					glm::vec2 uvs = glm::vec2(uv[0],uv[1]);
					m_uvs.push_back(uvs);
				}
				
				aiVector3D normal = mesh->mNormals[face.mIndices[j]];
				glm::vec3 normals = glm::vec3(normal[0],normal[1],normal[2]);
				m_normals.push_back(normals);
				
				aiVector3D pos = mesh->mVertices[face.mIndices[j]];
				glm::vec3 poss = glm::vec3(pos[0],pos[1],pos[2]);
				m_vertices.push_back(poss);
			}
		}
	}
	
	//~ Initializing model matrix
	m_model_matrix = glm::mat4(1.0);
	//~ Creating buffers
	create_buffers();
	//~ Load texture
	m_texture_path = texture_path;
	load_textures();
}

Object::~Object()
{
	if(m_texture_path != NULL)
	{
		glDeleteTextures(1, &m_diffuse_texture);
		SDL_FreeSurface(m_diffuse_texture_surface);
	}
	
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
	if((m_diffuse_texture_surface = SDL_LoadBMP(m_texture_path)))
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
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

glm::vec3 Object::computeBarycentre()
{
    glm::vec3 sum = glm::vec3(0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        sum += m_vertices.at(i);
    }

    return sum /= m_vertices.size();
}

float Object::computeAvgDistToBarycentre()
{
    glm::vec3 bar = computeBarycentre();
    std::cout<<bar.x<<", "<<bar.y<<", "<<bar.z<<std::endl;
    float sumDist = 0;
    float avgDist = 0;

    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        sumDist += sqrt((m_vertices.at(i).x - bar.x)*(m_vertices.at(i).x - bar.x) +
                        (m_vertices.at(i).y - bar.y)*(m_vertices.at(i).y - bar.y) +
                        (m_vertices.at(i).z - bar.z)*(m_vertices.at(i).z - bar.z));
    }
    avgDist = sumDist / m_vertices.size();
    return avgDist;
}

float Object::computeStandardDeviation()
{
    glm::vec3 moyPt;
    moyPt = computeBarycentre();
    float sumDist = 0;
    float moyDist = 0;

    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        sumDist += sqrt((m_vertices.at(i).x - moyPt.x)*(m_vertices.at(i).x - moyPt.x) +
                        (m_vertices.at(i).y - moyPt.y)*(m_vertices.at(i).y - moyPt.y) +
                        (m_vertices.at(i).z - moyPt.z)*(m_vertices.at(i).z - moyPt.z));
    }
    moyDist = sumDist / m_vertices.size();

    float sumValMoinsMoy = 0;
    float var = 0;
    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        sumValMoinsMoy += pow(sqrt((m_vertices.at(i).x - moyPt.x)*(m_vertices.at(i).x - moyPt.x) +
                               (m_vertices.at(i).y - moyPt.y)*(m_vertices.at(i).y - moyPt.y) +
                               (m_vertices.at(i).z - moyPt.z)*(m_vertices.at(i).z - moyPt.z))
                          - moyDist, 2);
    }
    var = sumValMoinsMoy / m_vertices.size();

    return sqrt(var);
}

//~ Setters
void Object::set_model_matrix(const glm::mat4 input_matrix)
{
	m_model_matrix = input_matrix;
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

GLuint Object::get_diffuse_texture() const
{
	return m_diffuse_texture;
}

const char* Object::get_texture_path() const
{
	return m_texture_path;
}
