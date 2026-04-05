#include "collectible_game_object_key.h"

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	CollectibleGameObjectKey::CollectibleGameObjectKey(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint destroy_texture, int hit_points)
		: GameObject(position, geom, shader, texture, destroy_texture, hit_points) {
	}

	// Update function for moving the player object around
	void CollectibleGameObjectKey::Update(double delta_time) {

		// Special player updates go here

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

	//Collide function is overridden to make sure that there is no invunerability window so that if you hit 3 in x time, it will always register 3.
	void CollectibleGameObjectKey::collide(GameObject* collided_with) {
		if (collided_with->GetType() == GameObjectType::Player) {
			hit_points_ = 0;
			interactable_ = false;
		}
	}

	bool CollectibleGameObjectKey::return_exist(void){
		return hit_points_ > 0;
	}

	GameObjectType  CollectibleGameObjectKey::GetType() {
		return GameObjectType::CollectibleKey;
	}

} // namespace game
