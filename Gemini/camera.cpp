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

			m_front.x = -cos(-m_rotation.x) * sin(m_rotation.y);
			m_front.y = sin(m_rotation.x);
			m_front.z = -cos(-m_rotation.x) * cos(m_rotation.y);

			m_front = glm::normalize(m_front);

			m_right = glm::normalize(glm::cross(m_front, UP));
			m_up = glm::cross(m_right, m_front);

			m_vw_matrix = glm::mat4x4(1.0f);
			m_vw_matrix = glm::scale(m_vw_matrix, 1.0f / m_size);
			m_vw_matrix = glm::rotate(m_vw_matrix, -m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, -m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, -m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_vw_matrix = glm::translate(m_vw_matrix, -m_position);

		}

		glm::mat4x4 Camera::getViewMatrix(){
			return m_vw_matrix;
		}

	}
}

