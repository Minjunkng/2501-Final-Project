#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint destroy_texture, int hit_points, GLuint invunerable_texture, GLuint furry_texture, GLuint alpha_furry_texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;
            void collide(GameObject*) override;

            void AddVelocity(const glm::vec3& dv);

            void Jump(const glm::vec3& impulse);

            bool isFurry();
            bool CurrentFurry();

            void FurryToggle();

            GameObjectType GetType(void) override;
        private:
            int items_collected_;
            int keys_collected_;
            bool invincible_;
            GLuint invunerable_texture_;
            GLuint dog_texture_;
            GLuint dog_invunerable_texture_;

            glm::vec3 velocity_;

            bool isFurryEnabled_;
            bool isFurry_;

            bool in_air_;
            float gravity_;
            float restitution_;
            float ground_height_;
            float stop_threshold_;

            GLuint base_texture_;
            void UpdateTexture();
    }; // class PlayerGameObject


} // namespace game
#endif // PLAYER_GAME_OBJECT_H_
