#include "blade_game_object.h"
#include <glm/gtc/constants.hpp>

namespace game {

    void BladeGameObject::Update(double delta_time) {
        // Rotate blade locally
        angle_ += rotation_speed_ * static_cast<float>(delta_time);

        float two_pi = 2.0f * glm::pi<float>();
        if (angle_ >= two_pi) angle_ -= two_pi;

        // Hierarchical transformation: position is relative to parent
        if (parent_) {
            glm::vec3 parent_pos = parent_->GetPosition();
            position_ = parent_pos + offset_; // move relative to parent
        }
    }

} // namespace game