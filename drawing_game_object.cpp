#include "drawing_game_object.h"

namespace game {

DrawingGameObject::DrawingGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
	: GameObject(position, geom, shader, texture, texture, -1) {
}

void DrawingGameObject::Render(glm::mat4 view_matrix, double current_time) {

}

}