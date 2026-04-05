#ifndef USER_INTERFACE_OBJECT_H_
#define USER_INTERFACE_OBJECT_H_

#include "game_object.h"

namespace game {

class UserInterfaceObject : public GameObject {

	public:
		UserInterfaceObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
	
};

}


#endif
