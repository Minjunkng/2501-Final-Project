#include "enemy_game_object_boxy.h"

#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> // length2

namespace game {

    static constexpr float kMinInterceptTime = 0.05f;
    static constexpr float kDefaultStopRadius = 0.15f;
    static constexpr float kMaxSpeed = 2.0f;

    float EnemyGameObjectBoxy::ClampFloat(float x, float lo, float hi) {
        if (x < lo) return lo;
        if (x > hi) return hi;
        return x;
    }

    //Setting up the modular abilities of the enemy object(velocities, direction, etc.)
    EnemyGameObjectBoxy::EnemyGameObjectBoxy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points)
        : GameObject(position, geom, shader, texture, destroy_texture, hit_points),
        state_(State::Patrolling),
        player_(nullptr),
        patrol_center_(position),
        ellipse_width_(2.5f),
        ellipse_height_(1.5f),
        theta_(0.0f),
        omega_(1.0f),
        target_(position),
        velocity_(0.0f, 0.0f, 0.0f),
        intercept_trigger_radius_(2.0f),
        desired_intercept_time_(2.0f),
        course_timer_(),
        course_period_(2.0f)
    {
    }

    //Collision override to interact with different elements differently
    void EnemyGameObjectBoxy::collide(GameObject* collided_with) {
        if (timer_.Finished() && (collided_with->GetType() == GameObjectType::Player || collided_with->GetType() == GameObjectType::Projectile)) {
            hit_points_ -= 1;
            //Resets textures
            if (hit_points_ == 0) {
                texture_ = destroy_texture_;
                interactable_ = false;
            }
            //5 second explosion timer, only triggers if hp == 0 as well
            timer_.Start(5);
        }
    }

    void EnemyGameObjectBoxy::SetPlayer(GameObject* player) {
        player_ = player;
    }

    GameObjectType EnemyGameObjectBoxy::GetType() {
        return GameObjectType::EnemyBoxy;
    }

    void EnemyGameObjectBoxy::SetPatrolEllipse(const glm::vec3& center, float w, float h) {
        patrol_center_ = center;
        ellipse_width_ = (w > 0.01f) ? w : 0.01f;
        ellipse_height_ = (h > 0.01f) ? h : 0.01f;
    }

    //Setting for private variable
    void EnemyGameObjectBoxy::SetPatrolAngularSpeed(float omega_rad_per_sec) {
        omega_ = omega_rad_per_sec;
    }

    //Trigger radius setting for outside modification
    void EnemyGameObjectBoxy::SetInterceptTriggerRadius(float r) {
        intercept_trigger_radius_ = (r > 0.0f) ? r : 0.0f;
    }

    //Specific intercept time to avoid "blinking"
    void EnemyGameObjectBoxy::SetDesiredInterceptTime(float seconds) {
        desired_intercept_time_ = (seconds > kMinInterceptTime) ? seconds : kMinInterceptTime;
    }

    //Course correction period
    void EnemyGameObjectBoxy::SetCourseCorrectionPeriod(float seconds) {
        course_period_ = (seconds > 0.05f) ? seconds : 0.05f;
    }

    //Target set(basically always to last seen coordinates of player)
    void EnemyGameObjectBoxy::SetTarget(const glm::vec3& target) {
        target_ = target;
    }

    EnemyGameObjectBoxy::State EnemyGameObjectBoxy::GetState() const {
        return state_;
    }

    void EnemyGameObjectBoxy::Update(double delta_time) {
        // 1) State transition: patrolling -> intercepting if player gets near
        if (state_ == State::Patrolling && player_ != nullptr) {
            glm::vec3 ppos = player_->GetPosition();
            glm::vec3 epos = GetPosition();

            float r = intercept_trigger_radius_;
            if (glm::length2(ppos - epos) <= r * r) {
                state_ = State::Intercepting;

                // set target to player's current position
                target_ = ppos;

                // compute initial velocity so movement is not instant
                RecomputeVelocityTowardTarget();

                // start slow reaction timer
                course_timer_.Start(course_period_);
            }
        }

        // 2) Execute behavior
        if (state_ == State::Patrolling) {
            PatrolStep(delta_time);
        }
        else {
            InterceptStep(delta_time);
        }
    }

    void EnemyGameObjectBoxy::PatrolStep(double dt) {
        // Back-and-forth motion along x-axis

        float amplitude = 0.5f * ellipse_width_; // reuse width as range

        theta_ += omega_ * static_cast<float>(dt);

        // Keep theta bounded
        const float two_pi = 2.0f * glm::pi<float>();
        if (theta_ >= two_pi) theta_ -= two_pi;
        if (theta_ < 0.0f)   theta_ += two_pi;

        glm::vec3 pos = patrol_center_;

        // Oscillate only in x
        pos.x += amplitude * std::sin(theta_);

        // y remains constant
        SetPosition(pos);
    }

    void EnemyGameObjectBoxy::InterceptStep(double dt) {
        // Slow reaction: every course_period_ seconds update target+velocity
        if (player_ != nullptr && course_timer_.Finished()) {
            target_ = player_->GetPosition();
            RecomputeVelocityTowardTarget();
            course_timer_.Start(course_period_);
        }

        // Euler integration: pos += v * dt
        glm::vec3 pos = GetPosition();
        pos += velocity_ * static_cast<float>(dt);
        SetPosition(pos);

        // if very close to target, stop moving (prevents jitter/overshoot)
        if (glm::length2(target_ - pos) <= kDefaultStopRadius * kDefaultStopRadius) {
            velocity_ = glm::vec3(0.0f);
        }
    }

    void EnemyGameObjectBoxy::RecomputeVelocityTowardTarget() {
        glm::vec3 pos = GetPosition();
        glm::vec3 to_target = target_ - pos;

        // v = (target - pos) / T  (so it takes ~T seconds)
        float T = (desired_intercept_time_ > kMinInterceptTime) ? desired_intercept_time_ : kMinInterceptTime;
        glm::vec3 v = to_target / T;

        // Clamp maximum speed for gameplay feel (optional)
        float speed2 = glm::length2(v);
        if (speed2 > kMaxSpeed * kMaxSpeed) {
            float speed = std::sqrt(speed2);
            v = (v / speed) * kMaxSpeed;
        }

        velocity_ = v;
    }

} // namespace game