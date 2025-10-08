#pragma once

/// Includes
# include "core/Logger.hpp"

/// System includes
# include <iostream>

/// Dependencies
# include "glm/glm.hpp"
# include <glm/gtc/matrix_transform.hpp>

namespace GE::Objects {

	/// @brief Stores the camera information (view matrix)
	struct CameraInfo {
		glm::vec3	position = {0, 0, 0};
		glm::vec3	lookAt   = {0, 1, 0};
		glm::vec3	up       = {0, 0, 1};
		glm::vec3	right    = {1, 0, 0};
	};

	/// @brief Stores the projection information (projection matrix)
	struct ProjectionInfo {
		float		fov		= 45.0f;
		float		near	= 0.1f;
		float		far		= 100.0f;
		glm::vec2	resolution;
		glm::vec2	resolutionOffset;
	};

	/**
	 * @brief The Camera class represents a camera in 3D space, handling view and projection matrices.
	 * 
	 * The Camera class allows you to set the camera's position, orientation, and projection type (perspective or orthographic).
	 * 
	 * It provides methods to retrieve the view and projection matrices, which are essential for rendering 2D and 3D scenes.
	 */
	class Camera {
		public:
			// Projection types
			enum class ProjectionType {
				PERSPECTIVE,
				ORTHOGRAPHIC
			};

			Camera(const CameraInfo &cameraInfo, const ProjectionInfo &projectionInfo, const ProjectionType &type, Core::Logger *logger = nullptr);
			Camera(const Camera &camera);
			Camera &operator=(const Camera &camera);
			~Camera();

			/// Getters
			
			const CameraInfo		&getCameraInfo() const;
			const ProjectionInfo	&getProjectionInfo() const;

			glm::mat4	getViewMatrix();
			glm::mat4	getProjectionMatrix();
			operator	glm::mat4();

			/// Setters

			void	setCameraInfo(const CameraInfo &cameraInfo);
			void	setProjectionType(const ProjectionType &type);

			void	setPosition(const glm::vec3 &position);
			void	setLookAt(const glm::vec3 &lookAt);

			void	addToPosition(const glm::vec3 &position);
			void	addToLookAt(const glm::vec3 &lookAt);

			void	setFOV(const float &fov);

		private:
			CameraInfo		_cameraInfo;
			ProjectionInfo	_projectionInfo;

			glm::mat4		_viewMatrix;
			glm::mat4		_projectionMatrix;

			ProjectionType	_type;

			Core::Logger	*logger = nullptr;

			/// Private functions

			void	_updateViewMatrix();
			void	_updateProjectionMatrix();
	};
} // namespace GE::Objects