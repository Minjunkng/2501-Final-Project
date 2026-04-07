#include "player_game_object.h"
#include "game.h"

namespace game {

    /*
        PlayerGameObject inherits from GameObject
    */

    PlayerGameObject::PlayerGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points, GLuint invunerable_texture, GLuint furry_texture, GLuint alpha_furry_texture)
        : GameObject(position, geom, shader, texture, destroy_texture, hit_points),
        items_collected_(0),
        keys_collected_(0),
        isFurryEnabled_(0),
        isFurry_(0),
        invincible_(false),
        base_texture_(texture), //  store original
        invunerable_texture_(invunerable_texture),
        dog_texture_(furry_texture),
        dog_invunerable_texture_(alpha_furry_texture),
        velocity_(0.0f, 0.0f, 0.0f),
        in_air_(true),
        gravity_(-9.8f),
        restitution_(0.5f),
        ground_height_(-2.1f),
        stop_threshold_(0.3f) {
    }

    //  central texture logic
    void PlayerGameObject::UpdateTexture() {
        if (isFurry_) {
            texture_ = invincible_ ? dog_invunerable_texture_ : dog_texture_;
        }
        else {
            texture_ = invincible_ ? invunerable_texture_ : base_texture_;
        }
    }


    //Collide function
    void PlayerGameObject::collide(GameObject* collided_with) {
        if ((collided_with->GetType() == GameObjectType::Enemy
            || collided_with->GetType() == GameObjectType::EnemyBoxy
            || collided_with->GetType() == GameObjectType::EnemyStationary)
            && !invincible_) {
            hit_points_ -= 1;
            if (hit_points_ == 0) {
                texture_ = destroy_texture_;
                interactable_ = false;
                exploded_ = true;
            }
            timer_.Start(5);
        }
        else if (collided_with->GetType() == GameObjectType::CollectibleKey) {
            keys_collected_ += 1;
            if (keys_collected_ == 3) {
                ghost_ = true;
            }
        }
        else if (collided_with->GetType() == GameObjectType::CollectibleGun) {
            isFurryEnabled_ = true;
            isFurry_ = true;

            UpdateTexture();
        }
        else if (collided_with->GetType() == GameObjectType::Collectible) {
            items_collected_ += 1;

            if (items_collected_ == 5) {
                invincible_ = true;
                items_collected_ = 0;
                timer_.Start(10);

                UpdateTexture();
            }
        }
    }

    int PlayerGameObject::GetNumKeys() const {
        return keys_collected_;
    }

    bool PlayerGameObject::isFurry() {
        return isFurryEnabled_;
    }

    bool PlayerGameObject::CurrentFurry() {
        return isFurry_;
    }

    void PlayerGameObject::FurryToggle() {
        if (!isFurryEnabled_) return;

        isFurry_ = !isFurry_;

        UpdateTexture(); //  switch correctly
    }

    void PlayerGameObject::Update(double delta_time) {
        float dt = (float)delta_time;

        glm::vec3 pos = GetPosition();

        // apply gravity
        if (in_air_) {
            velocity_.y += gravity_ * dt;
        }

        // integrate pos
        pos += velocity_ * dt;

        // ground collisions
        if (pos.y <= ground_height_) {
            pos.y = ground_height_;

            if (in_air_) {
                velocity_.y = -velocity_.y * restitution_;

                if (fabs(velocity_.y) < stop_threshold_) {
                    velocity_.y = 0;
                    in_air_ = false;
                }
            }
        }

        SetPosition(pos);

        // invincibility expired
        if (timer_.Finished() && invincible_) {
            invincible_ = false;

            UpdateTexture(); //  revert properly
        }
    }

    GameObjectType PlayerGameObject::GetType() {
        return GameObjectType::Player;
    }

    void PlayerGameObject::AddVelocity(const glm::vec3& dv) {
        velocity_ += dv;
    }

    void PlayerGameObject::Jump(const glm::vec3& impulse) {
        if (!in_air_) {
            velocity_ += impulse;
            in_air_ = true;
        }
    }

} // namespace game