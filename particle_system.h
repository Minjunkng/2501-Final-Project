#pragma once
#include "game_object.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace game {

    class Geometry;
    class Shader;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;          // current color (alpha fades)
        glm::vec4 initial_color;  // original color (used for darkening)
        float lifetime;
        float angle;
        glm::vec2 scale;
    };

    //  Base ParticleSystem 
    class ParticleSystem : public GameObject {
    public:
        ParticleSystem(const glm::vec3& position, Geometry* geom, Shader* shader,
            int max_particles, GLuint texture = 0);

        virtual void Update(double delta_time);
        virtual void Render(glm::mat4 view_matrix, double current_time);
        bool IsFinished() const;

    protected:
        std::vector<Particle> particles_;
        int max_particles_;
        GLuint texture_;
    };

    // explosion particles child class
    class ExplosionParticles : public ParticleSystem {
    public:
        ExplosionParticles(const glm::vec3& position, Geometry* geom, Shader* shader,
            int num_particles, GLuint texture);

        void Render(glm::mat4 view_matrix, double current_time) override;
    };

    // snow particles child class
    class SnowParticles : public ParticleSystem {
    public:
        SnowParticles(float screen_width, float screen_height, Geometry* geom, Shader* shader,
            int num_particles, GLuint texture);

        void Update(double delta_time) override;
    };

} // namespace game