#include "camera.h"
#include <glm/gtx/transform.hpp>

#define YAW m_rotation.y
#define PITCH m_rotation.x

namespace gemini {
	namespace util {

		Camera::Camera()
		{

		}


		Camera::~Camera()
		{
		
		}

		void Camera::moveFront(float dist)
		{
			m_position += m_front * dist;
		}

		void Camera::moveRight(float dist)
		{
			m_position += m_right * dist;
		}

		void Camera::moveUp(float dist)
		{
			m_position += m_up * dist;
		}

		void Camera::update(){

			m_front.x = -cos(YAW - PI / 2.0f) * cos(-PITCH);
			m_front.y = -sin(-PITCH);
			m_front.z = sin(YAW + PI / 2.0f) * cos(PITCH);

			m_right = glm::cross(m_front, UP);
			m_up = glm::cross(m_right, m_front);

			m_vw_matrix = glm::mat4x4(1.0f);
			m_vw_matrix = glm::scale(m_vw_matrix, m_size);
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_vw_matrix = glm::translate(m_vw_matrix, m_position);

		}

		glm::mat4x4 Camera::getViewMatrix(){
			return m_vw_matrix;
		}

	}
}

