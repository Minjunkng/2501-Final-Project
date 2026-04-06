#ifndef PROJECTILE_OBJECT_H_
#define PROJECTILE_OBJECT_H_

#include "game_object.h"

namespace game {

    class ProjectileObject : public GameObject {
    public:

        ProjectileObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, const glm::vec3& direction);

        void Update(double delta_time) override;
        void collide(GameObject*) override;

        bool ProjectileObject::return_exist() override;

        GameObjectType GetType(void) override;

        void SetSpeed(float new_speed);

        glm::vec3 GetVelocity() const override;

    private:

        glm::vec3 velocity_;
        float lifetime_ = 2.0f;
        float age_ = 0.0f;
        bool interactable_;
    };

} // namespace game

#endif