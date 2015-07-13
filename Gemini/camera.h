#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/constants.hpp>

namespace gemini {
	namespace util {
		const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		const float PI = glm::pi<float>();
		class Camera
		{
		public:
			Camera();
			virtual ~Camera();
			glm::mat4x4 getViewMatrix();
			void update();
			void moveFront(float dist);
			void moveRight(float dist);
			void moveUp(float dist);
			inline glm::vec3 front() { return m_front; }
			inline glm::vec3 right() { return m_right; }
			inline glm::vec3 up() { return m_up; }
		private:
			glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		public:
			glm::vec3 m_position = glm::vec3(0.0f);
			glm::vec3 m_rotation = glm::vec3(0.0f);
			glm::vec3 m_size = glm::vec3(1.0f);
		private:
			glm::mat4x4 m_vw_matrix;
		};
	}
}

