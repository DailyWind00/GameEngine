# include "Shader.hpp"

namespace GE::OpenGL {

	# pragma region Constructors & Destructors

	Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath, Core::Logger *logger) : logger(logger) {
		if (logger) logger->trace("Creating shader");

		this->vertexPath = vertexPath;
		this->fragmentPath = fragmentPath;
		this->geometryPath = geometryPath;

		shaderID = make_shader();

		if (logger) logger->info("Shader created");
	}

	Shader::~Shader() {
		glUseProgram(0);
		glDeleteProgram(shaderID);

		if (logger) logger->info("Shader deleted");
	}
	
	# pragma endregion

	# pragma region Private functions

	// Create a shader module from a file
	GLuint Shader::make_module(const std::string &filepath, GLuint module_type) {
		std::ifstream file(filepath);
		std::stringstream buffer;
		std::string line;

		if (logger) logger->trace("> Compiling shader : " + filepath);

		if (!file.is_open())
			throw std::runtime_error("Failed to open file " + filepath);

		while (getline(file, line))
			buffer << line << '\n';
		file.close();

		std::string shaderSourceStr = buffer.str();			// Cause corruption if
		const char *shaderSource = shaderSourceStr.c_str(); // in a single line

		GLuint shaderModule = glCreateShader(module_type);
		glShaderSource(shaderModule, 1, &shaderSource, nullptr);
		glCompileShader(shaderModule);

		int success;
		glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
		if (!success) {
			std::string infoLog;
			infoLog.resize(1024);
			glGetShaderInfoLog(shaderModule, 1024, nullptr, (GLchar *)infoLog.data());
			throw std::runtime_error("Failed to compile shader " + filepath + ":\n" + infoLog);
		}

		if (logger) logger->trace("Shader compiled");

		return shaderModule;
	}

	// Create a shader from the modules
	GLuint Shader::make_shader() {
		std::vector<GLuint> shadersIDs;

		shadersIDs.push_back(make_module(vertexPath, GL_VERTEX_SHADER));
		shadersIDs.push_back(make_module(fragmentPath, GL_FRAGMENT_SHADER));
		if (!geometryPath.empty())
			shadersIDs.push_back(make_module(geometryPath, GL_GEOMETRY_SHADER));

		if (logger) logger->trace("> Linking shader program");

		GLuint shader = glCreateProgram();
		for (GLuint module = 0; module < shadersIDs.size(); module++) {
			glAttachShader(shader, shadersIDs[module]);
			glDeleteShader(shadersIDs[module]);
		}

		glLinkProgram(shader);

		int success;
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			std::string infoLog;
			infoLog.resize(1024);
			glGetProgramInfoLog(shader, 1024, nullptr, (GLchar *)infoLog.data());
			throw std::runtime_error("Failed to link shader:\n" + infoLog);
		}

		if (logger) logger->trace("Shader program linked");

		return shader;
	}

	# pragma endregion

	# pragma region Public functions

	// Set this shader as the current shader
	void Shader::use() {
		glUseProgram(shaderID);
	}

	// Recompile the shader
	void Shader::recompile() {
		if (logger) logger->info("Recompiling shader");

		glUseProgram(0);
		glDeleteProgram(shaderID);

		try {
			shaderID = make_shader();
		}
		catch (const std::exception &e) {
			if (logger) logger->error("Failed to recompile shader: " + std::string(e.what()));
			return;
		}

		if (logger) logger->info("Shader recompiled");
	}

	# pragma endregion

	# pragma region Uniforms setters

	// Set a boolean uniform
	void Shader::setUniform(const std::string &name, bool value) {
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
	}

	// Set an integer uniform
	void Shader::setUniform(const std::string &name, int value) {
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	// Set a float uniform
	void Shader::setUniform(const std::string &name, float value) {
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	// Set a vec2 uniform
	void Shader::setUniform(const std::string &name, glm::vec2 value) {
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1]);
	}

	// Set a vec3 uniform
	void Shader::setUniform(const std::string &name, glm::vec3 value) {
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1], value[2]);
	}

	// Set a vec4 uniform
	void Shader::setUniform(const std::string &name, glm::vec4 value) {
		glUniform4f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1], value[2], value[3]);
	}

	// Set a mat4 uniform
	void Shader::setUniform(const std::string &name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	
	# pragma endregion

	# pragma region Getters

	// Return the shader ID
	const GLuint &Shader::getID() const {
		return shaderID;
	}

	# pragma endregion
	
} // namespace GE::OpenGL