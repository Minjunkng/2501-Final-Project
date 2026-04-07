#include "enemy_game_object_stationary.h"
#include <glm/gtx/norm.hpp> 

namespace game {

    static constexpr float kMaxSpeed = 2.0f;

    EnemyGameObjectStationary::EnemyGameObjectStationary(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points)
        : GameObject(position, geom, shader, texture, destroy_texture, hit_points),
        state_(State::Idle),
        player_(nullptr),
        intercept_trigger_radius_(5.0f)
    {
    }

    void EnemyGameObjectStationary::collide(GameObject* collided_with) {
        if (timer_.Finished() && (collided_with->GetType() == GameObjectType::Player || collided_with->GetType() == GameObjectType::Projectile)) {
            hit_points_ -= 1;
            if (hit_points_ <= 0) {
                texture_ = destroy_texture_;
                scale_ = glm::vec2(1.5f, 1.5f);
                interactable_ = false;
            }
            timer_.Start(5);
        }
    }

    void EnemyGameObjectStationary::Update(double delta_time) {
        if (player_ == nullptr) return;

        glm::vec3 ppos = player_->GetPosition();
        glm::vec3 epos = GetPosition();

        // 1) Detection Logic
        if (state_ == State::Idle) {
            if (glm::length2(ppos - epos) <= intercept_trigger_radius_ * intercept_trigger_radius_) {
                state_ = State::Moving;
            }
        }

        // 2) Movement Logic (Only if detected)
        if (state_ == State::Moving) {
            glm::vec3 diff = ppos - epos;
            float dist2 = glm::length2(diff);

            if (dist2 > 0.01f) { // Prevent division by zero
                glm::vec3 direction = glm::normalize(diff);
                // Fixed: used delta_time instead of undefined dt
                glm::vec3 new_pos = epos + (direction * kMaxSpeed * static_cast<float>(delta_time));
                SetPosition(new_pos);
            }
        }
    }

    void EnemyGameObjectStationary::SetPlayer(GameObject* player) {
        player_ = player;
    }

    void EnemyGameObjectStationary::SetInterceptTriggerRadius(float r) {
        intercept_trigger_radius_ = r;
    }

    GameObjectType EnemyGameObjectStationary::GetType() {
        return GameObjectType::EnemyStationary;
    }

} // namespace game