#pragma once

/// Defines
# define CL_HPP_TARGET_OPENCL_VERSION 300 // OpenCL 3.0
# define CL_HPP_ENABLE_EXCEPTIONS
# define CL_ERROR_MSG "\033]8;;https://registry.khronos.org/OpenCL/specs/opencl-cplusplus-1.2.pdf\033\\[Click Here]\033]8;;\033\\"
# define COLOR_HEADER_CXX
# define NO_LIMIT std::numeric_limits<long>::max() // No limit for the number of particles

/// System includes
# include <vector>
# include <array>

/// Dependencies
# include "Shader.hpp"
# include "color.h"
# include <json/json_config.hpp>
# include <OpenCL/opencl.hpp>
# include <OpenCL/cl_gl.h>
# include <GL/glx.h>

/// Global variables
extern bool VERBOSE;

// Data structure for a particle
// Stored in 7 floats
typedef struct Particle {
	cl_float	position[3];
	cl_float	velocity[3];
	cl_float	life;
} Particle;

// This class is an interface to store particles datas.
// The behavior of the particles will be defined in OpenCL kernel functions.
// This class set a OpenCL context and a OpenCL queue.
class ParticleSystem {
	friend class ParticleSystemsHandler;

	private:
		// OpenGL variables

		GLuint				VBO;
		GLuint				VAO;

		// OpenCL variables

		cl::Platform		platform;
		cl::Device			device;
		cl::Context			context;
		cl::Program			program;
		cl::Kernel			kernel;
		cl::BufferGL		particles; // VRAM buffer
		cl::CommandQueue	queue;
		std::vector<cl::Memory>	memObjects;

		// Other variables

		size_t				particleCount;

		/// Private functions

		const std::string	CLstrerrno(cl_int error);
		cl::Platform	getPlatform();
		cl::Device		getDevice(const cl::Platform &platform);
		cl::Context		createContext(const cl::Device &device, const cl::Platform &platform);
		cl::Program		buildProgram(const std::vector<std::string> &VkernelProgramPaths);
		void			createOpenGLBuffers(size_t bufferSize);
		void			createOpenCLContext(const std::vector<std::string> &VkernelProgramPaths);

	public:
		ParticleSystem(size_t ParticleCount, const std::vector<std::string> &VkernelProgramPaths);
		~ParticleSystem();

		/// Public functions

		void	draw();

		// Set the kernel arguments
		// kernel format : void update(__global Particle *particles, int particleCount, ...)
		//  - Particle = struct { cl_float position[3], cl_float velocity[3], cl_float life }
		// Use OpenCL types
		template <typename... arguments>
		void	setKernelArgs(arguments... args) {
			try {
				/// Set the kernel arguments
				int index = 0;

				// Mandatory arguments
				kernel.setArg(index++, particles);
				kernel.setArg(index++, (cl_int)particleCount);

				// User arguments
				(void)(int[]){0, (kernel.setArg(index++, args), 0)...};
			}
			catch (const cl::Error &e) {
				throw std::runtime_error("OpenCL error : " + (std::string)e.what() + " (" + CLstrerrno(e.err()) + ")");
			}
		}
};

// Example of JSON configuration file:
// particleSystem: [{
//     "name": "string",
//     "particleCount": uint,
//     "shader": ["vertexPath", "fragmentPath"],
//     "kernel": ["kernelPath", ...]
// }, ...]
typedef struct JSONParticleSystemConfig {
	std::string					name;
	long						particleCount;
	std::array<std::string, 2>	shaderPaths;
	std::vector<std::string>	kernelPaths;

	bool				active;
	ParticleSystem	   *particleSystem = nullptr;
	GLuint				shaderID = 0;
} JSONParticleSystemConfig;
typedef std::vector<JSONParticleSystemConfig> VJSONParticleSystemConfigs;

// Optional class for a easier control of multiple particle systems
// This class accept JSON configuration files:
class ParticleSystemsHandler {
	private:
		VJSONParticleSystemConfigs	particleSystems;
		Shader						shaders;
		long						globalParticleCount;

	public:
		ParticleSystemsHandler(const std::string &JSONConfigPath, long globalParticleCount = NO_LIMIT);
		~ParticleSystemsHandler();

		/// Public functions

		void	activate(const std::string &systemName);
		void	deactivate(const std::string &systemName);
		void	drawActivesParticleSystems();

		// Set the uniform to the particle system shader
		template <typename argument>
		void setShaderUniform(const std::string &systemName, const std::string &uniformName, const argument &args) {
			auto particleSystem = operator[](systemName);

			if (particleSystem == particleSystems.end())
				throw std::runtime_error("Particle System \"" + systemName + "\" not found");

			if (!particleSystem->active)
				throw std::runtime_error("Particle System \"" + systemName + "\" is inactive");

			shaders.setUniform(particleSystem->shaderID, uniformName, args);
		};

		// Set the kernel arguments to the particle system kernel
		// kernel format : void update(__global Particle *particles, int particleCount, ...)
		//  - Particle = struct { cl_float position[3], cl_float velocity[3], cl_float life }
		// Use OpenCL types
		template <typename... arguments>
		void setKernelArgs(const std::string &systemName, arguments... args) {
			auto particleSystem = operator[](systemName);

			if (particleSystem == particleSystems.end())
				throw std::runtime_error("Particle System \"" + systemName + "\" not found");

			if (!particleSystem->active)
				throw std::runtime_error("Particle System \"" + systemName + "\" is inactive");

			particleSystem->particleSystem->setKernelArgs(args...);
		};

		/// Getters

		VJSONParticleSystemConfigs::const_iterator  operator[](const std::string &systemName) const;
		VJSONParticleSystemConfigs::const_iterator  operator[](const uint &index) const;
		VJSONParticleSystemConfigs::const_iterator  begin() const;
		VJSONParticleSystemConfigs::const_iterator  front() const;
		VJSONParticleSystemConfigs::const_iterator  back() const;
		VJSONParticleSystemConfigs::const_iterator  end() const;
		const long					 			   &getGlobalParticleCount() const;
};