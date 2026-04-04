#include "wall_game_object.h"

namespace game {

    //Setting up the modular abilities of the enemy object(velocities, direction, etc.)
    WallGameObject::WallGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int hitpoints)
        : GameObject(position, geom, shader, texture, texture, -1) {

    }

    void WallGameObject::Update(double delta_time) {
    }

    GameObjectType WallGameObject::GetType() {
        return GameObjectType::Wall;
    }

    void WallGameObject::collide(GameObject* collided_with) {
        if (collided_with->GetType() == GameObjectType::Enemy) {
            interactable_ = false;
        }
    }

} // namespace game
