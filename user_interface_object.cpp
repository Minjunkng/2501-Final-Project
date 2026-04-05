#include "user_interface_object.h"

namespace game {

	UserInterfaceObject::UserInterfaceObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture, texture, -1) {
	}

}