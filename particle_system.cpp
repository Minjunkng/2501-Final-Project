#include "particle_system.h"
#include "geometry.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace game {

    //  Base ParticleSystem class
    ParticleSystem::ParticleSystem(const glm::vec3& position, Geometry* geom, Shader* shader,
        int max_particles, GLuint texture)
        : GameObject(position, geom, shader, 0, 0, -1),
        max_particles_(max_particles),
        texture_(texture)
    {
        particles_.resize(max_particles_);
    }

    bool ParticleSystem::IsFinished() const {
        return std::all_of(particles_.begin(), particles_.end(),
            [](const Particle& p) { return p.lifetime <= 0.0f; });
    }

    void ParticleSystem::Update(double delta_time) {
        for (auto& p : particles_) {
            if (p.lifetime > 0.0f) {
                p.position += p.velocity * static_cast<float>(delta_time);
                p.lifetime -= static_cast<float>(delta_time);

                // Only fade alpha here; RGB darkening done in Render()
                p.color.a = std::max(0.0f, p.color.a - static_cast<float>(delta_time) * 0.5f);
            }
        }
    }

    void ParticleSystem::Render(glm::mat4 view_matrix, double /*current_time*/) {
        shader_->Enable();
        shader_->SetUniformMat4("view_matrix", view_matrix);

        if (texture_ != 0)
            glBindTexture(GL_TEXTURE_2D, texture_);

        for (const auto& p : particles_) {
            if (p.lifetime <= 0.0f) continue;

            glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(p.scale, 1.0f));
            glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), p.angle, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), p.position);
            glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

            shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
            shader_->SetUniform4f("override_color", p.color);
            shader_->SetUniform1i("ghost", 0);

            geometry_->SetGeometry(shader_->GetShaderProgram());
            glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, nullptr);
        }

        if (texture_ != 0)
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    // explosion particles child class
    ExplosionParticles::ExplosionParticles(const glm::vec3& position, Geometry* geom, Shader* shader,
        int num_particles, GLuint texture)
        : ParticleSystem(position, geom, shader, num_particles, texture)
    {
        for (auto& p : particles_) {
            float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159265f;
            float speed = 2.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f;
            p.velocity = glm::vec3(std::cos(angle) * speed, std::sin(angle) * speed, 0.0f);
            p.position = position;
            p.lifetime = 2.0f + static_cast<float>(rand()) / RAND_MAX;
            p.initial_color = glm::vec4(1.0f, 0.5f + static_cast<float>(rand()) / 2.0f, 0.0f, 1.0f);
            p.color = p.initial_color;
            p.angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159265f;
            p.scale = glm::vec2(0.1f, 0.1f);
        }

        hit_points_ = 0;
        timer_.Start(2.0);
    }

    void ExplosionParticles::Render(glm::mat4 view_matrix, double /*current_time*/) {
        shader_->Enable();
        shader_->SetUniformMat4("view_matrix", view_matrix);

        if (texture_ != 0)
            glBindTexture(GL_TEXTURE_2D, texture_);

        for (auto& p : particles_) {
            if (p.lifetime <= 0.0f) continue;

            // Determine life ratio and whether to apply "ghost" effect
            float life_ratio = std::max(0.0f, p.lifetime / 2.0f); // 2.0f = max lifetime
            glm::vec4 darkened_color = glm::vec4(p.initial_color.r * life_ratio,
                p.initial_color.g * life_ratio,
                p.initial_color.b * life_ratio,
                p.color.a);

            // Apply ghost after 1 second
            int ghost_flag = (2.0f - p.lifetime >= 1.0f) ? 1 : 0;
            shader_->SetUniform1i("ghost", ghost_flag);

            glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(p.scale, 1.0f));
            glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), p.angle, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), p.position);
            glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

            shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
            shader_->SetUniform4f("override_color", darkened_color);

            geometry_->SetGeometry(shader_->GetShaderProgram());
            glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, nullptr);
        }

        if (texture_ != 0)
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Snow particles child class
    SnowParticles::SnowParticles(float screen_width, float screen_height, Geometry* geom, Shader* shader,
        int num_particles, GLuint texture)
        : ParticleSystem(glm::vec3(0.0f), geom, shader, num_particles, texture)
    {
        for (auto& p : particles_) {
            p.position = glm::vec3(
                static_cast<float>(rand()) / RAND_MAX * screen_width - screen_width / 2.0f,
                static_cast<float>(rand()) / RAND_MAX * screen_height / 2.0f,
                0.0f
            );
            p.velocity = glm::vec3(0.0f, -0.01f - static_cast<float>(rand()) / 500.0f, 0.0f);
            p.lifetime = 1000.0f; // effectively infinite
            p.color = glm::vec4(0.9f, 0.95f, 1.0f, 1.0f);
            p.initial_color = p.color;
            p.angle = 0.0f;
            p.scale = glm::vec2(0.1f, 0.1f);
        }
    }

    void SnowParticles::Update(double delta_time) {
        for (auto& p : particles_) {
            p.position += p.velocity * static_cast<float>(delta_time);

            if (p.position.y < -50.0f) {
                p.position.y = 25.0f;
                p.position.x = static_cast<float>(rand()) / RAND_MAX * 50.0f - 25.0f;
            }
        }
    }

} // namespace game