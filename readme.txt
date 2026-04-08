Minjun Kang + Amanda Lee
101306107 + 101349677

How to Play:

Use WASD to move, and space to shoot. Collect 3 keys to win with in the provided time limit in the UI. Collect the dog collectible 
to switch between the default weapon and the dog weapon, and Collect power up flasks to become super charged and invincible for a short duration. Be wary of the
Dog enemy, the cloud enemy and robot enemy, as they can chase and kill you! You have 3hp.

1.0 Game Mechanics:
Has a meaningful gameplay(enemies, powerups, life total) and meaningful finishing condition. Fulfills minimum 0.25 pts, 
and in addtion has 3 keys(specific item to be collected, item count to be achieved, and target locations to attack(enemies))

2.0 Enemies:
- 2.1 Enemy Textures:
A Wolf sprite(Same as invincible texture for the player), a fog cloud, and a robot gunner for 3 unique enemy types. 
Each enemy type has a unique texture, and the player can distinguish between them visually.
- 2.2 Enemy Behaviors:
Wolf: A melee enemy that chases the player when within a certain range. Uses chase behavior with very 
little time between readjusting, so will follow the player similar to pursuit. Unique attack is a suicide bite.

Cloud: A evil fog that follows a fixed back and forth pattern. Uses no special abilities, and follows a basic movement pattern. 
Unique attack is a dash attack.

Robot: A robot enemy that wanders in an elliptical motion, and can switch into a chase mode when player approaches 
a set distance from robot. Unique attack is a slow charge. 

3.0 Weapons:
Weapon 1: Shoots a red diamond at a slow fire rate. The default weapon.
Weapon 2: After obtaining the brown dog upgrade, can toggle P to switch into dog mode, showing a 
higher energy blast gun which has a faster travel rate. Has a small chance to insta blow up an enemy.

4.9 Collectible Items:
4.1: A dog collectible represents a upgrade that allows the player to switch between dog form and airplane form for different 
weapons. a set of 3 keys is also a type of collectible as the main objective of the game.
4.2: A power up flask collectible also exists in the game. If 5 are collected, the player becomes super charged, becoming 
invincible for a short duration. the texture changes to reflect that.

5.0 Movement:
5.1: Movement is handled through transformations.
5.2 The Player uses physics based movement(i.e with gravity) and other objects may use parametric based equations.

6.0 Collisions:
Collision detection for everything. Player collides with enemies and collectibles. Enemies collide with the player.

7.0 Game World:
7.1: Can scroll to the side(by moving) to a larger game world.
7.2: Texture is crated using texture tiling.

8.0 Particles:
Explosion particles when enemies die.
Energy Particles when Player gets dog weapon upgrade.

9.0 UI:
9.1: A health bar that shows the player's current health. A timer that shows how long the player 
has left to win. And a counter for how many keys the player has collected.

10 Advanced Method: 
Enemy Pursuit mechanics as an advanced steering method

11 Documentation:
Code is commented and Readme is provided.

Bonus:
Second Advanced method for a bonus 0.5 pts: Collision handling for the Player uses rebound mechanics(There is an air wall)

