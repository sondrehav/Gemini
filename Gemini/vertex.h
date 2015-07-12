#pragma once

#include <glm\glm.hpp>

namespace gemini {
	namespace graphics {
		struct Vertex
		{
			Vertex(glm::vec3 in_position, glm::vec3 in_normal, glm::vec3 in_color, glm::vec2 in_texcoord)
			{
				position = in_position;
				normal = in_normal;
				color = in_color;
				texcoord = in_texcoord;
			}
			Vertex()
			{
				position = glm::vec3(0.0f);
				normal = glm::vec3(0.0f);
				color = glm::vec3(0.0f);;
				texcoord = glm::vec2(0.0f);;
			}
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 color;
			glm::vec2 texcoord;
		};
	}
}


