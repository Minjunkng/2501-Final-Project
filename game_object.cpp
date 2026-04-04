#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game_object.h"
#include "timer.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint destroy_texture, int hit_points)
{

    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(1.0f, 1.0f);
    angle_ = 0.0;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    destroy_texture_ = destroy_texture;
    hit_points_ = hit_points;
    Timer* timer_ = new Timer();
    interactable_ = true;
}

bool GameObject::return_interactable(void) {
    return interactable_;
}

//True if you exist, false if you don't
bool GameObject::return_exist(void) {
    if (hit_points_ == 0 && timer_.Finished()) {
        return false;
    }
    else {
        return true;
    }
}

float GameObject::return_x(void) {
    return position_.x;
}

float GameObject::return_y(void) {
    return position_.y;
}

float GameObject::return_radius(void) {
    return 0.5f * std::max(scale_.x, scale_.y);
}

//Has a different gameobject to check the typing matchup
void GameObject::collide(GameObject* collided_with) {
    //Leftover code from before, since enemies currently only have 1 hp, this does not actually negatively impact anything yet
    if (timer_.Finished()) {
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

GameObjectType GameObject::GetType() {
    return GameObjectType::Generic;
}


glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}


void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}


void GameObject::Update(double delta_time) {
}

glm::vec3 GameObject::GetVelocity() const {
    return glm::vec3(-2.5f, 0.0f, 0.0f);
}

void GameObject::SetScale(float s) {
    scale_ = glm::vec2(s, s);
}

void GameObject::SetScale(float sx, float sy) {
    scale_ = glm::vec2(sx, sy);
}


void GameObject::Render(glm::mat4 view_matrix, double current_time) {

    shader_->Enable();

    shader_->SetUniformMat4("view_matrix", view_matrix);

    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    geometry_->SetGeometry(shader_->GetShaderProgram());

    glBindTexture(GL_TEXTURE_2D, texture_);

    // **Set ghost uniform**
    shader_->SetUniform1i("ghost", ghost_ ? 1 : 0);

    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
