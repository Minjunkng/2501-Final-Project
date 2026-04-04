#pragma once
#include "game_object.h"

namespace game {

class BladeGameObject : public GameObject {
public:
    BladeGameObject(GameObject* parent,
                    const glm::vec3& offset, // relative to parent
                    Geometry* geom,
                    Shader* shader,
                    GLuint texture,
                    GLuint destroy_texture,
                    float rotation_speed = glm::radians(180.0f))
        : GameObject(parent->GetPosition(), geom, shader, texture, destroy_texture, -1),
          parent_(parent), offset_(offset), rotation_speed_(rotation_speed)
    {}

    void Update(double delta_time) override;

private:
    GameObject* parent_;
    glm::vec3 offset_;        // local offset from parent
    float rotation_speed_;
};
} // namespace game