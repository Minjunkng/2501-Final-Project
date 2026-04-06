#include "player_game_object.h"
#include "game.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint destroy_texture, int hit_points, GLuint invunerable_texture, GLuint furry_texture, GLuint alpha_furry_texture)
    : GameObject(position, geom, shader, texture, destroy_texture, hit_points),
      items_collected_(0),
      keys_collected_(0),
      isFurryEnabled_(0),
      isFurry_(0),
      invincible_(false),
      invunerable_texture_(invunerable_texture),
      dog_texture_(furry_texture),
      dog_invunerable_texture_(alpha_furry_texture),
      velocity_(0.0f, 0.0f, 0.0f),
      in_air_(true),
      gravity_(-9.8f),
      restitution_(0.5f),
      ground_height_(-2.1f),
      stop_threshold_(0.3f) {}
      
      

//Collide function is overridden to make sure that there is no invunerability window so that if you hit 3 in x time, it will always register 3.
void PlayerGameObject::collide(GameObject* collided_with) {
    if (collided_with->GetType() == GameObjectType::Enemy && !invincible_) {
        hit_points_ -= 1;
        if (hit_points_ == 0) {
            texture_ = destroy_texture_;
            interactable_ = false;
        }

        timer_.Start(5);
    }
    else if (collided_with->GetType() == GameObjectType::CollectibleKey) {
        keys_collected_ += 1;
        if (keys_collected_ == 1) {
            ghost_ = true;
        }
    }
    else if (collided_with->GetType() == GameObjectType::CollectibleGun) {
        isFurryEnabled_ = true;
        isFurry_ = true;

        GLuint temp = texture_;
        texture_ = dog_texture_;
        dog_texture_ = temp;
    }
    else if (collided_with->GetType() == GameObjectType::Collectible) {
        items_collected_ += 1;
		if (items_collected_ == 5) {
			invincible_ = true;
			items_collected_ = 0;
			timer_.Start(10);

            if (isFurryEnabled_) {
                if (isFurry_) {
                    GLuint temp = texture_;
                    texture_ = dog_invunerable_texture_;
                    invunerable_texture_ = temp;
                }
                else {
                    GLuint temp = texture_;
                    texture_ = invunerable_texture_;
                    invunerable_texture_ = temp;
                }
            } else {
                GLuint temp = texture_;
                texture_ = invunerable_texture_;
                invunerable_texture_ = temp;
			}
		}
	}
}

bool PlayerGameObject::isFurry() {
    if (isFurryEnabled_) {
        return true;
    }
    return false;
}

bool PlayerGameObject::CurrentFurry() {
	return isFurry_;
}

void PlayerGameObject::FurryToggle() {
    if (isFurry_) {
        isFurry_ = false;
    } else {
		isFurry_ = true;
    }
    GLuint temp = texture_;
    texture_ = dog_texture_;
    dog_texture_ = temp;
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

    // ground collsions
    if (pos.y <= ground_height_) {

        pos.y = ground_height_;

        if (in_air_) {

            velocity_.y = -velocity_.y * restitution_;

            // stop bouncing if velocity is small
            if (fabs(velocity_.y) < stop_threshold_) {
                velocity_.y = 0;
                in_air_ = false;
            }
        }
    }

    SetPosition(pos);

    if (timer_.Finished() && invincible_) {
        invincible_ = false;

        GLuint temp = invunerable_texture_;
        invunerable_texture_ = texture_;
        texture_ = temp;
    }
}

GameObjectType  PlayerGameObject::GetType() {
	return GameObjectType::Player;
}

void PlayerGameObject::AddVelocity(const glm::vec3& dv)
{
	velocity_ += dv;
}

void PlayerGameObject::Jump(const glm::vec3& impulse) {

    if (!in_air_) {
        velocity_ += impulse;
        in_air_ = true;
    }
}

} // namespace game
