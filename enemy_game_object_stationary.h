#ifndef ENEMY_GAME_OBJECT_STATIONARY_H_
#define ENEMY_GAME_OBJECT_STATIONARY_H_

#include <glm/vec3.hpp>
#include "game_object.h"
#include "timer.h"

namespace game {

    class EnemyGameObjectStationary : public GameObject {
    public:
        // Simplified states: either sitting still or moving toward player
        enum class State {
            Idle,
            Moving
        };

        EnemyGameObjectStationary(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points);

        // --- Overrides ---
        void Update(double delta_time) override;
        void collide(GameObject* collided_with) override;
        GameObjectType GetType(void) override;

        // --- Configuration ---
        void SetPlayer(GameObject* player);
        void SetInterceptTriggerRadius(float r);

    private:
        State state_;
        GameObject* player_;
        float intercept_trigger_radius_;
    };

} // namespace game

#endif // ENEMY_GAME_OBJECT_STATIONARY_H_