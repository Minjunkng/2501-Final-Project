#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "game_object.h"
#include "particle_system.h"

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

            static Game& GetInstance() {
                static Game instance;
                return instance;
            }

            Geometry* GetParticleGeometry() { return particle_geometry_; }
            Shader* GetParticleShader() { return particle_shader_; }
            std::vector<GameObject*>& GetGameObjects() { return game_objects_; }

        private:

            Timer entity_spawn_timer_;
            GLuint stationary_entity_tex_;
            GLuint collectible_entity_tex_;
            GLuint enemy_entity_tex_;
            GLuint entity_explosion_tex_;
			GLuint collectible_key_tex_;
            GLuint projectile_tex_;
			GLuint tex_diamond_red_;
            void SpawnEntity(char);

            Geometry* particle_geometry_;
            Shader* particle_shader_;

            GameObject* blade_;
            float blade_rotation_ = 0.0f;

            SnowParticles* snow_particles_ = nullptr;
            Timer snow_spawn_timer_;
            float world_width_ = 50.0f;   // width of snow spawn area
            float world_height_ = 25.0f;

            Timer toggle_timer_;

            Timer game_timer_;

            void HandleCollision(GameObject* a, GameObject* b);
            bool Game::RayCircleIntersect(
                float ox, float oy,     // ray origin
                float dx, float dy,     // ray direction (normalized)
                float cx, float cy,     // circle center
                float r,                // circle radius
                float maxDist           // maximum ray length (projectile travel this frame)
            );

            double shoot_cooldown_ = 0.0;
            const double shoot_interval_ = 0.35f;

            GLuint explosion_tex_;

            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // Shader for health bar
            Shader health_bar_shader_;

            // Shader for text
            Shader text_shader;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // List of game objects
            std::vector<GameObject*> game_objects_;

            // Keep track of game time
            double current_time_;

            // Camera zoom factor
            float camera_zoom_;

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Get mouse position in OpenGL coordinates
            // Returns "true" if position could be gathered, "false" otherwise
            // Position is saved to mouse.x and mouse.y
            bool GetMousePosition(glm::vec2 &mouse);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);
    }; // class Game

} // namespace game

#endif // GAME_H_
