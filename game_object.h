#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"
#include "timer.h"

namespace game {

    enum class GameObjectType {
        Generic,
        Player,
        Enemy,
		EnemyStationary,
        EnemyBoxy,
        Collectible,
		CollectibleGun,
		CollectibleKey,
        Projectile,
        Wall,
        Text
    };

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint destroy_texture, int hit_points);

            virtual float GameObject::return_radius(void);///////////////////////////
            float GameObject::return_x(void);
            float GameObject::return_y(void);

            virtual bool GameObject::return_exist(void);

            virtual void GameObject::collide(GameObject*);

            virtual GameObjectType GameObject::GetType();

            bool GameObject::return_interactable(void);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject. Can also be overriden in children
            virtual void Render(glm::mat4 view_matrix, double current_time);
            void RenderWithModel(glm::mat4 view, glm::mat4 model, double time);

            // Getters
            //
            // Get position in OpenGL coordinates
            inline glm::vec3 GetPosition(void) const { return position_; }
            // Get scale (size) of the object
            inline glm::vec2 GetScale(void) const { return glm::vec2(scale_.x, scale_.y); }
            // Get current orientation
            inline float GetRotation(void) const { return angle_; }

            // Get bearing direction (vector pointing to the direction in which
            // the game object is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Get hit pts
            int getHitPoints() const { return hit_points_; }
            
            bool HasExploded() const { return exploded_; }

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            void SetScale(float s);
            void SetScale(float sx, float sy);
            void SetRotation(float angle);

            virtual glm::vec3 GetVelocity() const;

            void SetGhost(bool g) { ghost_ = g; }
            bool IsGhost() const { return ghost_; }


        protected:
            // Object's Transform Variables
            bool ghost_ = false;
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;
            int hit_points_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;
            GLuint destroy_texture_;

            Timer timer_;

            bool interactable_;
            bool exploded_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
