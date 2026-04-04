#ifndef WALL_GAME_OBJECT_H_
#define WALL_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    class WallGameObject : public GameObject {
    public:

        WallGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int hitpoints);

        void Update(double delta_time) override;
        void collide(GameObject*) override;

        GameObjectType GetType(void) override;


    private:
        bool interactable_;
    };

} // namespace game

#endif