#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint destroy_texture, int hit_points, GLuint invunerable_texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;
            void collide(GameObject*) override;

            void AddVelocity(const glm::vec3& dv);

            void Jump(const glm::vec3& impulse);

            GameObjectType GetType(void) override;
        private:
            int items_collected_;
            bool invincible_;
            GLuint invunerable_texture_;

            glm::vec3 velocity_;

            bool in_air_;
            float gravity_;
            float restitution_;
            float ground_height_;
            float stop_threshold_;
    }; // class PlayerGameObject


} // namespace game
#endif // PLAYER_GAME_OBJECT_H_
