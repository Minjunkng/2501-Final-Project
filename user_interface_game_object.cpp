#include "user_interface_game_object.h"

namespace game {

	UserInterfaceGameObject::UserInterfaceGameObject(GameObject* parent, const glm::vec3& offset, Geometry* geom, Shader* shader, GLuint texture) :
		GameObject(parent->GetPosition(), geom, shader, texture, texture, -1) {
		parent_ = parent;
		offset_ = offset;
	}

	void UserInterfaceGameObject::Update(double delta_time) {
		// move UI with player
		if (parent_) {
			glm::vec3 parent_pos = parent_->GetPosition();
			position_ = parent_pos + offset_; // move relative to parent
		}
	}
}