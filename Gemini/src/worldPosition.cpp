#include "worldPosition.h"

namespace gemini {
	namespace util {


		WorldPosition::WorldPosition()
		{

		}


		WorldPosition::~WorldPosition()
		{

		}

		void WorldPosition::moveFront(float dist)
		{
			m_position += m_front * dist;
		}

		void WorldPosition::moveRight(float dist)
		{
			m_position += m_right * dist;
		}

		void WorldPosition::moveUp(float dist)
		{
			m_position += m_up * dist;
		}

		void WorldPosition::update(){

			m_front.x = -cos(m_rotation.y - PI / 2.0f) * cos(-m_rotation.x);
			m_front.y = -sin(-m_rotation.x);
			m_front.z = sin(m_rotation.y + PI / 2.0f) * cos(m_rotation.x);

			m_right = glm::cross(m_front, UP);
			m_up = glm::cross(m_right, m_front);

			m_vw_matrix = glm::mat4x4(1.0f);
			m_vw_matrix = glm::scale(m_vw_matrix, m_size);
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_vw_matrix = glm::rotate(m_vw_matrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_vw_matrix = glm::translate(m_vw_matrix, m_position);

		}

		glm::mat4x4 WorldPosition::getViewMatrix(){
			return m_vw_matrix;
		}

	}
}