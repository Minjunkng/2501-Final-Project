#ifndef TEXT_GAME_OBJECT_H_
#define TEXT_GAME_OBJECT_H_

#include <string>

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class TextGameObject : public GameObject {

        public:
            TextGameObject(GameObject* player, const glm::vec3 &offset, Geometry *geom, Shader *shader, GLuint texture);

            // Text to be displayed
            std::string GetText(void) const;
            void SetText(std::string text);

            // Render function for the text
            void Render(glm::mat4 view_matrix, double current_time) override;

            void Update(double delta_time) override;

        private:
            std::string text_;
            GameObject* parent_;
            glm::vec3 offset_;

    }; // class TextGameObject

} // namespace game

#endif // TEXT_GAME_OBJECT_H_