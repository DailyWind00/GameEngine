#pragma once

/// Defines
# define CL_HPP_TARGET_OPENCL_VERSION 300 // OpenCL 3.0
# define CL_HPP_ENABLE_EXCEPTIONS
# define CL_ERROR_MSG "\033]8;;https://registry.khronos.org/OpenCL/specs/opencl-cplusplus-1.2.pdf\033\\[Click Here]\033]8;;\033\\"
# define NO_LIMIT std::numeric_limits<long>::max() // No limit for the number of particles

/// Includes
# include "core/Logger.hpp"

/// System includes
# include <vector>
# include <array>

/// Dependencies
# include <json/json_config.hpp>
# include <OpenCL/opencl.hpp>
# include <OpenCL/cl_gl.h>
# include <glad/glad.h>
# include <GL/glx.h>

namespace GE {
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
		public:
			ParticleSystem(size_t ParticleCount, const std::vector<std::string> &VkernelProgramPaths, Core::Logger* logger = nullptr);
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
			Core::Logger*		logger = nullptr;

			/// Private functions

			const std::string	CLstrerrno(cl_int error);
			cl::Platform	getPlatform();
			cl::Device		getDevice(const cl::Platform &platform);
			cl::Context		createContext(const cl::Device &device, const cl::Platform &platform);
			cl::Program		buildProgram(const std::vector<std::string> &VkernelProgramPaths);
			void			createOpenGLBuffers(size_t bufferSize);
			void			createOpenCLContext(const std::vector<std::string> &VkernelProgramPaths);
	};
} // namespace GE