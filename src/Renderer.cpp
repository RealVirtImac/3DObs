#include "../include/Renderer.hpp"

Renderer::Renderer():m_drawing_mode(false)
{
	GLenum error;
	if((error = glewInit()) != GLEW_OK) {
		throw std::runtime_error((const char*)glewGetErrorString(error));
	}
	//~ Loading object
	m_object = new Object("models/cube_map.obj");
	//~ Compiling shaders
	m_basic_shader_program = loadProgram("shaders/basic.vertex.glsl","shaders/basic.fragment.glsl");
	m_lighting_shader_program = loadProgram("shaders/lighting.vertex.glsl","shaders/lighting.fragment.glsl");
	//~ Locating uniforms
	m_basic_shader_model_matrix_position = glGetUniformLocation(m_basic_shader_program,"model_matrix");
	m_basic_shader_view_matrix_position = glGetUniformLocation(m_basic_shader_program,"view_matrix");
	m_basic_shader_projection_matrix_position = glGetUniformLocation(m_basic_shader_program,"projection_matrix");
	
	m_lighting_shader_model_matrix_position = glGetUniformLocation(m_lighting_shader_program,"model_matrix");
	m_lighting_shader_view_matrix_position = glGetUniformLocation(m_lighting_shader_program,"view_matrix");
	m_lighting_shader_projection_matrix_position = glGetUniformLocation(m_lighting_shader_program,"projection_matrix");
	m_lighting_shader_camera_position = glGetUniformLocation(m_lighting_shader_program,"camera_position");
	m_lighting_shader_diffuse_texture = glGetUniformLocation(m_lighting_shader_program, "diffuse_texture");
	//~ Creating camera
	glm::vec3 cam_one_position = glm::vec3(0.0f,0.0f,5.0f);
	glm::vec3 cam_one_up = glm::vec3(0.0f,1.0f,0.0f);
	glm::vec3 cam_one_target = glm::vec3(0.0f,0.0f,0.0f);
	m_camera_one = new Camera(cam_one_position,cam_one_up,cam_one_target);
}

Renderer::~Renderer()
{
	delete m_object;
	delete m_camera_one;
}

void Renderer::render()
{
	//~ Choosing shader
	glUseProgram(m_lighting_shader_program);
	//~ Sending uniforms
	glUniformMatrix4fv(m_lighting_shader_model_matrix_position, 1, GL_FALSE, glm::value_ptr(m_object->get_model_matrix()));
	glUniformMatrix4fv(m_lighting_shader_view_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_view_matrix()));
	glUniformMatrix4fv(m_lighting_shader_projection_matrix_position, 1, GL_FALSE, glm::value_ptr(m_camera_one->get_projection_matrix()));
	glUniform3fv(m_lighting_shader_camera_position, GL_FALSE, glm::value_ptr(m_camera_one->get_position()));
	glUniform1i(m_lighting_shader_diffuse_texture, 0);
	//~ Binding vao
	glBindVertexArray(m_object->get_vao());
	//~ Drawing
	switch(m_drawing_mode)
	{
		case false : glDrawArrays(GL_TRIANGLES, 0, m_object->get_size());
		break;
		case true : glDrawArrays(GL_LINE_LOOP, 0, m_object->get_size());
		break;
	}
	//~ Unbind
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

Camera* Renderer::get_camera_one() const
{
	return m_camera_one;
}

void Renderer::switch_drawing_mode()
{
	m_drawing_mode = !m_drawing_mode;
}
