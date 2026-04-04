#include "projectile_object.h"

namespace game {

    //Setting up the modular abilities of the enemy object(velocities, direction, etc.)
    ProjectileObject::ProjectileObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, const glm::vec3& direction)
        : GameObject(position, geom, shader, texture, texture, 1) {

        velocity_ = direction * 3.0f; // fast speed
        interactable_ = true;
    }

    void ProjectileObject::Update(double delta_time) {
        float dt = (float)delta_time;

        glm::vec3 pos = GetPosition();
        pos += velocity_ * dt;
        SetPosition(pos);

        age_ += dt;
        if (age_ >= lifetime_) {
            interactable_ = false;
        }
    }

    bool ProjectileObject::return_exist() {
        if (interactable_ == false) {
            return false;
        }
        return true;
    }

    GameObjectType ProjectileObject::GetType() {
        return GameObjectType::Projectile;
    }

    glm::vec3 ProjectileObject::GetVelocity() const {
        return velocity_;
    }

    void ProjectileObject::collide(GameObject* collided_with) {
        if (collided_with->GetType() == GameObjectType::Enemy) {
            interactable_ = false;
        }
    }

} // namespace game
