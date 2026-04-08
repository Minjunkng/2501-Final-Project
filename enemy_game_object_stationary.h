#ifndef ENEMY_GAME_OBJECT_STATIONARY_H_
#define ENEMY_GAME_OBJECT_STATIONARY_H_

#include <glm/vec3.hpp>
#include "game_object.h"
#include "timer.h"

namespace game {

    class EnemyGameObjectStationary : public GameObject {
    public:
        //For the enemy state
        enum class State {
            Patrolling,
            Intercepting
        };

        void collide(GameObject*) override;

        EnemyGameObjectStationary(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points);

        // --- Configuration ---
        void SetPlayer(GameObject* player);
        void SetPatrolEllipse(const glm::vec3& center, float w, float h);
        void SetPatrolAngularSpeed(float omega_rad_per_sec);
        void SetInterceptTriggerRadius(float r);
        void SetDesiredInterceptTime(float seconds);
        void SetCourseCorrectionPeriod(float seconds);
        void SetTarget(const glm::vec3& target);

        State GetState() const;

        void Update(double delta_time) override;

        GameObjectType GetType(void) override;

    private:
        State state_;

        // Player pointer
        GameObject* player_;

        // Patrol ellipse params
        glm::vec3 patrol_center_;
        float ellipse_width_;   // full width
        float ellipse_height_;  // full height
        float theta_;           // param angle
        float omega_;           // radians/second

        glm::vec3 target_;
        glm::vec3 velocity_;
        float intercept_trigger_radius_;
        float desired_intercept_time_;

        // Slow reaction timer 
        Timer course_timer_;
        float course_period_;

    private:
        void PatrolStep(double dt);
        void InterceptStep(double dt);
        void RecomputeVelocityTowardTarget();

        static float ClampFloat(float x, float lo, float hi);
    };

} // namespace game

#endif // ENEMY_GAME_OBJECT_STATIONARY_H_
