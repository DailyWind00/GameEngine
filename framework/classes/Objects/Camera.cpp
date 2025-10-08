#include "Camera.hpp"

namespace GE::Objects {

	# pragma region Constructors & Destructors

	Camera::Camera(const CameraInfo &cameraInfo, const ProjectionInfo &projectionInfo, const ProjectionType &type, Core::Logger *logger) : logger(logger) {
		_type = type;
		_cameraInfo = cameraInfo;
		_projectionInfo = projectionInfo;
		_updateViewMatrix();
		_updateProjectionMatrix();

		if (logger) logger->info("Creating camera");
	}

	Camera::Camera(const Camera &camera) {
		_type = camera._type;
		_cameraInfo = camera._cameraInfo;
		_projectionInfo = camera._projectionInfo;
		_updateViewMatrix();
		_updateProjectionMatrix();
	}

	Camera &Camera::operator=(const Camera &camera) {
		_type = camera._type;
		_cameraInfo = camera._cameraInfo;
		_projectionInfo = camera._projectionInfo;
		_updateViewMatrix();
		_updateProjectionMatrix();
		return *this;
	}

	Camera::~Camera() {
		if (logger) logger->info("Camera destroyed");
	}
	
	# pragma endregion

	# pragma region Private functions

	/// @brief Update the view matrix based on the camera information
	void	Camera::_updateViewMatrix() {
		_viewMatrix = glm::lookAt(_cameraInfo.position, _cameraInfo.lookAt, _cameraInfo.up);
		_cameraInfo.right = glm::normalize(glm::cross(glm::normalize(_cameraInfo.lookAt - _cameraInfo.position), _cameraInfo.up));
	}

	/// @brief Update the projection matrix based on the projection information and type
	void	Camera::_updateProjectionMatrix() {
		switch (_type) {
			case ProjectionType::PERSPECTIVE:
				_projectionMatrix = glm::perspective(
					glm::radians(_projectionInfo.fov),
					_projectionInfo.resolution.x / _projectionInfo.resolution.y,
					_projectionInfo.near,
					_projectionInfo.far
				);
				break;
			case ProjectionType::ORTHOGRAPHIC:
				_projectionMatrix = glm::ortho(
					_projectionInfo.resolutionOffset.x,
					_projectionInfo.resolution.x,
					_projectionInfo.resolutionOffset.y,
					_projectionInfo.resolution.y,
					_projectionInfo.near, _projectionInfo.far
				);
				break;
		}
	}
	
	# pragma endregion

	# pragma region Getters

	/// @brief Return the camera information
	/// @return The camera information
	const CameraInfo	&Camera::getCameraInfo() const {
		return _cameraInfo;
	}

	/// @brief Return the projection information
	/// @return The projection information
	const ProjectionInfo	&Camera::getProjectionInfo() const {
		return _projectionInfo;
	}

	/// @brief Return the view matrix
	/// @return The view matrix
	glm::mat4	Camera::getViewMatrix() {
		return _viewMatrix;
	}

	/// @brief Return the projection matrix
	/// @return The projection matrix
	glm::mat4	Camera::getProjectionMatrix() {
		return _projectionMatrix;
	}

	/// @brief Return the combined projection * view matrix
	/// @return The combined projection * view matrix
	Camera::operator glm::mat4() {
		return _projectionMatrix * _viewMatrix;
	}
	
	# pragma endregion

	# pragma region Setters

	/// @brief Set the camera information
	/// @param cameraInfo The new camera information
	void	Camera::setCameraInfo(const CameraInfo &cameraInfo) {
		_cameraInfo = cameraInfo;
		_updateViewMatrix();
	}

	/// @brief Set the projection type and update the projection matrix
	/// @param type The new projection type
	void	Camera::setProjectionType(const ProjectionType &type) {
		_type = type;
		_updateProjectionMatrix();
	}

	/// @brief Set the camera position
	/// @param position The new camera position
	void	Camera::setPosition(const glm::vec3 &position) {
		_cameraInfo.position = position;
		_updateViewMatrix();
	}

	/// @brief Set the camera lookAt
	/// @param lookAt The new camera lookAt
	void	Camera::setLookAt(const glm::vec3 &lookAt) {
		_cameraInfo.lookAt = lookAt;
		_updateViewMatrix();
	}

	/// @brief Set the camera field of view (only for perspective projection)
	/// @param fov The new field of view in degrees
	void	Camera::setFOV(const float &fov) {
		_projectionInfo.fov = fov;
		_updateProjectionMatrix();
	}

	/// @brief Add to the camera position
	/// @param position The position to add
	void	Camera::addToPosition(const glm::vec3 &position) {
		_cameraInfo.position += position;
		_updateViewMatrix();
	}

	/// @brief Add to the camera lookAt
	/// @param lookAt The lookAt to add
	void	Camera::addToLookAt(const glm::vec3 &lookAt) {
		_cameraInfo.lookAt += lookAt;
		_updateViewMatrix();
	}
	
	# pragma endregion

} // namespace GE::Objects