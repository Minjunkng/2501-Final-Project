#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

    public:
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points);

        // Update function for moving the Collectible object around
        void Update(double delta_time) override;
        void collide(GameObject*) override;
        bool return_exist(void) override;

        GameObjectType GetType(void) override;

    }; // class CollectibleGameObject


} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
