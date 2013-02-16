/*!
 * \file Renderer.cpp
 * \brief Renderer of the context
 * \author R. Bertozzi & S. Bougeois 
 */

#include "../include/Renderer.hpp"

Renderer::Renderer(int width, int height):
	m_width(width),
	m_height(height)
{
	GLenum error;
	if((error = glewInit()) != GLEW_OK) {
		throw std::runtime_error((const char*)glewGetErrorString(error));
	}
	//~ Loading object
	m_object = new Object("models/mothership.obj","textures/stargate.bmp");
	m_object->set_model_matrix(glm::scale(glm::mat4(1.0),glm::vec3(0.25f,0.25f,0.25f)));
	//~ Loading quad
	m_quad = new Object("models/quad.obj",NULL);
	//~ Compiling shaders
	m_basic_shader_program = loadProgram("shaders/basic.vertex.glsl","shaders/basic.fragment.glsl");
	m_lighting_shader_program = loadProgram("shaders/lighting.vertex.glsl","shaders/lighting.fragment.glsl");
	m_lighting_no_texture_shader_program = loadProgram("shaders/lighting.vertex.glsl","shaders/lighting_no_tex.fragment.glsl");
	m_quad_shader = loadProgram("shaders/quad.vertex.glsl","shaders/quad.fragment.glsl");
	//~ Locating uniforms
	m_basic_shader_model_matrix_position = glGetUniformLocation(m_basic_shader_program,"model_matrix");
	m_basic_shader_view_matrix_position = glGetUniformLocation(m_basic_shader_program,"view_matrix");
	m_basic_shader_projection_matrix_position = glGetUniformLocation(m_basic_shader_program,"projection_matrix");
	
	m_lighting_shader_model_matrix_position = glGetUniformLocation(m_lighting_shader_program,"model_matrix");
	m_lighting_shader_view_matrix_position = glGetUniformLocation(m_lighting_shader_program,"view_matrix");
	m_lighting_shader_projection_matrix_position = glGetUniformLocation(m_lighting_shader_program,"projection_matrix");
	m_lighting_shader_camera_position = glGetUniformLocation(m_lighting_shader_program,"camera_position");
	m_lighting_shader_diffuse_texture = glGetUniformLocation(m_lighting_shader_program, "diffuse_texture");
	
	m_lighting_no_texture_shader_model_matrix_position = glGetUniformLocation(m_lighting_no_texture_shader_program,"model_matrix");
	m_lighting_no_texture_shader_view_matrix_position = glGetUniformLocation(m_lighting_no_texture_shader_program,"view_matrix");
	m_lighting_no_texture_shader_projection_matrix_position = glGetUniformLocation(m_lighting_no_texture_shader_program,"projection_matrix");
	m_lighting_no_texture_shader_camera_position = glGetUniformLocation(m_lighting_no_texture_shader_program,"camera_position");
	
	m_quad_shader_texture_1 = glGetUniformLocation(m_quad_shader, "renderedTexture1");
	m_quad_shader_texture_2 = glGetUniformLocation(m_quad_shader, "renderedTexture2");
	
	//~ Creating camera
	m_camera_one = new Camera(m_width,m_height, 0);
	m_camera_two = new Camera(m_width,m_height, 1);
	
	glm::vec3 rig_position = glm::vec3(0.0f,0.0f,5.0f);
	glm::vec3 rig_up = glm::vec3(0.0f,1.0f,0.0f);
	glm::vec3 rig_target = glm::vec3(0.0f,0.0f,1.0f);
	m_rig = new Rig(m_camera_one,m_camera_two,rig_position,rig_up,rig_target,m_width,m_height);
	
	//Generating textures
	glGenTextures(1, &m_left_render_texture);
	glGenTextures(1, &m_left_depth_texture);
	//Binding textures
	glBindTexture(GL_TEXTURE_2D, m_left_render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, m_left_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenTextures(1, &m_right_render_texture);
	glGenTextures(1, &m_right_depth_texture);
	//Binding textures
	glBindTexture(GL_TEXTURE_2D, m_right_render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, m_right_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//~ //Generating framebuffers
	glGenFramebuffers(1, &m_left_framebuffer);
	//~ //Binding framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_left_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_left_render_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_left_depth_texture, 0);
	
	m_left_draw_buffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, m_left_draw_buffers);
	
	//~ //Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//~ //Generating framebuffers
	glGenFramebuffers(1, &m_right_framebuffer);
	//~ //Binding framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_right_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_right_render_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_right_depth_texture, 0);
	
	m_right_draw_buffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, m_right_draw_buffers);
	
	//~ //Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer Error" << std::endl;
}



Renderer::~Renderer()
{
	glDeleteTextures(1, &m_left_render_texture);
	glDeleteTextures(1, &m_left_depth_texture);
	glDeleteFramebuffers(1, &m_left_framebuffer);
	
	glDeleteTextures(1, &m_right_render_texture);
	glDeleteTextures(1, &m_right_depth_texture);
	glDeleteFramebuffers(1, &m_right_framebuffer);
	
	delete m_object;
	delete m_quad;
	delete m_camera_one;
	delete m_camera_two;
	delete m_rig;
}

void Renderer::render()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_left_framebuffer);
	glDrawBuffers(1, m_left_draw_buffers);
	glClearColor(0.0,0.0,0.0,1.0);
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//~ Choosing shader
	if(m_object->get_texture_path() != NULL)
	{
		glUseProgram(m_lighting_shader_program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_object->get_diffuse_texture());
		glUniform1i(m_lighting_shader_diffuse_texture, 0);
		glUniformMatrix4fv(m_lighting_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_object->get_model_matrix()));
		glUniformMatrix4fv(m_lighting_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_view_matrix()));
		glUniformMatrix4fv(m_lighting_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_projection_matrix()));
		glUniform3fv(m_lighting_shader_camera_position, GL_FALSE, glm::value_ptr(m_camera_one->get_position()));
	}
	else 
	{
		glUseProgram(m_lighting_no_texture_shader_program);
		glUniformMatrix4fv(m_lighting_no_texture_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_object->get_model_matrix()));
		glUniformMatrix4fv(m_lighting_no_texture_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_view_matrix()));
		glUniformMatrix4fv(m_lighting_no_texture_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_projection_matrix()));
		glUniform3fv(m_lighting_no_texture_shader_camera_position, GL_FALSE, glm::value_ptr(m_camera_one->get_position()));
	}
	
	//~ Binding vao
	glBindVertexArray(m_object->get_vao());
	//~ Drawing
	glDrawArrays(GL_TRIANGLES, 0, m_object->get_size());

	//~ Unbind
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_right_framebuffer);
	glDrawBuffers(1, m_right_draw_buffers);
	glClearColor(0.0,0.0,0.0,1.0);
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//~ Choosing shader
	if(m_object->get_texture_path() != NULL)
	{
		glUseProgram(m_lighting_shader_program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_object->get_diffuse_texture());
		glUniform1i(m_lighting_shader_diffuse_texture, 0);
		glUniformMatrix4fv(m_lighting_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_object->get_model_matrix()));
		glUniformMatrix4fv(m_lighting_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_two->get_view_matrix()));
		glUniformMatrix4fv(m_lighting_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_two->get_projection_matrix()));
		glUniform3fv(m_lighting_shader_camera_position, GL_FALSE, glm::value_ptr(m_camera_two->get_position()));
	}
	else 
	{
		glUseProgram(m_lighting_no_texture_shader_program);
		glUniformMatrix4fv(m_lighting_no_texture_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_object->get_model_matrix()));
		glUniformMatrix4fv(m_lighting_no_texture_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_view_matrix()));
		glUniformMatrix4fv(m_lighting_no_texture_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_projection_matrix()));
		glUniform3fv(m_lighting_no_texture_shader_camera_position, GL_FALSE, glm::value_ptr(m_camera_one->get_position()));
	}
	
	//~ Binding vao
	glBindVertexArray(m_object->get_vao());
	//~ Drawing
	glDrawArrays(GL_TRIANGLES, 0, m_object->get_size());

	//~ Unbind
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClearColor(0.0,0.0,0.0,1.0);
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//~ //Choosing shader
	glUseProgram(m_quad_shader);
	//~ //Sending uniforms
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_left_render_texture);
	glUniform1i(m_quad_shader_texture_1, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_right_render_texture);
	glUniform1i(m_quad_shader_texture_2, 1);
	glUniformMatrix4fv(m_lighting_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_quad->get_model_matrix()));
	glUniformMatrix4fv(m_lighting_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_two->get_view_matrix()));
	glUniformMatrix4fv(m_lighting_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(glm::perspective(
		m_camera_two->get_fov(),
		m_camera_two->get_ratio(),
		m_camera_two->get_near(),
		m_camera_two->get_far())));
	glUniform3fv(m_lighting_shader_camera_position, GL_FALSE, glm::value_ptr(m_rig->get_position()));
	//~ //Binding vao
	glBindVertexArray(m_quad->get_vao());
	//~ //Drawing
	glDrawArrays(GL_TRIANGLES, 0, m_quad->get_size());
	//~ //Unbind
	glBindVertexArray(0);
}

const char* Renderer::readFile(const char* filePath) {
    std::ifstream file(filePath);

    if (!file) {
        return 0;
    }

    file.seekg(0, std::ios_base::end);
    size_t length = file.tellg();
    file.seekg(0);

    char* src = new char[length];
    file.read(src, length);
    src[length - 1] = '\0';

    return src;
}

GLuint Renderer::loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    const char* vertexShaderSource = this->readFile(vertexShaderFile);
    if(!vertexShaderSource) {
        std::cerr << "Unable to load " << vertexShaderFile << std::endl;
        return 0;
    }

    const char* fragmentShaderSource = this->readFile(fragmentShaderFile);
    if(!fragmentShaderSource) {
        std::cerr << "Unable to load " << fragmentShaderFile << std::endl;
        return 0;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

        char* log = new char[logLength];

        glGetShaderInfoLog(vertexShader, logLength, 0, log);
        std::cerr << "Vertex Shader error:" << log << std::endl;
        std::cerr << vertexShaderSource << std::endl;

        delete [] log;
        return 0;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

        char* log = new char[logLength];

        glGetShaderInfoLog(fragmentShader, logLength, 0, log);
        std::cerr << "Fragment Shader error:" << log << std::endl;
        std::cerr << fragmentShaderSource << std::endl;

        delete [] log;
        return 0;
    }

    GLuint program;

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        char* log = new char[logLength];

        glGetProgramInfoLog(program, logLength, 0, log);
        std::cerr << "Program link error:" << log << std::endl;

        delete [] log;
        return 0;
    }

    delete [] vertexShaderSource;
    delete [] fragmentShaderSource;

    return program;
}

Rig* Renderer::get_rig() const
{
	return m_rig;
}

Camera* Renderer::get_camera_one() const
{
	return m_camera_one;
}

Camera* Renderer::get_camera_two() const
{
	return m_camera_two;
}
