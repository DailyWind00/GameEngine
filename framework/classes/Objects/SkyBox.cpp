#include "SkyBox.hpp"

namespace GE::Objects {

	# pragma region Constructors & Destructors

	SkyBox::SkyBox(const std::vector<std::string> &path, Core::Logger *logger) : logger(logger) {
		if (logger) logger->info("Creating SkyBox");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Load the skybox textures
		if (path.size() == 6) {
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			// Load the textures
			int width, height, nrChannels;
			for (size_t i = 0; i < 6; i++) {
				unsigned char *data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
				if (data) {
					if (nrChannels == 3)
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					else if (nrChannels == 4)
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					if (logger) logger->trace("Loaded texture: " + path[i]);
				}
				else
					if (logger) logger->error("Failed to load texture: " + path[i]);
					
				stbi_image_free(data);
			}

			// Set the texture parameters
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		else if (path.size() != 0) // Allow for no textures to be loaded
			throw std::invalid_argument("SkyBox: Invalid path size");

		// Set the vertex data for the cube
		float skyboxVertices[] = {        
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if (logger) logger->info("SkyBox created");
	}

	SkyBox::~SkyBox() {
		if (textureID)
			glDeleteTextures(1, &textureID);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);

		if (logger) logger->info("SkyBox deleted");
	}
	
	# pragma endregion

	# pragma region Public functions

	/// @brief Draw the skybox
	/// @note Temporarily disable depth writing and change depth function to GL_LEQUAL
	void SkyBox::draw() {
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		if (textureID) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
	
	# pragma endregion

} // namespace GE::Objects