/// Main configuration header file
/// This file includes all the necessary headers and sets up configurations for the project.
/// It is recommended to include this file in your main header files to ensure consistency.

#pragma once

/// Core includes
/// These includes are always needed to correctly use the framework.
/// They provide essential functionalities for other modules, such as logging and singleton pattern.
# include "core/Logger.hpp"
# include "core/Singleton.hpp"


/// Objects includes
/// These includes provide access to various drawable game objects and entities.
# include "classes/Objects/ParticleSystem.hpp"
# include "classes/Objects/SkyBox.hpp"
# include "classes/Objects/Camera.hpp"


/// OpenGL includes
/// These includes provide access to OpenGL functionalities, such as shaders and window management.
# include "classes/OpenGL/Shader.hpp"
# include "classes/OpenGL/Window.hpp"
# include "classes/OpenGL/BufferGL.hpp"
# include "classes/OpenGL/PMapBufferGL.hpp"


/// Utils includes
/// These includes provide access to various utility functions and classes.
# include "Utils/PriorityMutex.hpp"