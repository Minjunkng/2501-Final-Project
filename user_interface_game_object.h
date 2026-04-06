#pragma once
#include "game_object.h"

namespace game {

	class UserInterfaceGameObject : public GameObject {

	public:
		UserInterfaceGameObject(GameObject* parent, const glm::vec3& offset, Geometry* geom, Shader* shader, GLuint texture);

		void Update(double delta_time) override;

	private:
		GameObject* parent_;
		glm::vec3 offset_;

	};

}