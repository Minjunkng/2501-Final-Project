Minjun Kang
101306107

1.1 Continuous scrolling is enabled, character is rooted at center of screen

2.1 Background is now tiled, and is not too distorted. Note that while the background covers a large space, 
if a player goes too far out of bounds, it will result in a blue background. however, it would take a while.

3.1 Game object main class now can be toggled into a "ghost mode" by using a ghost attribute, which modifies the pixel shader to render in grayscale.

3.2 If the player object consumes 3 consumables(bullets), this will transition the player sprite into ghost mode(greyscale). Note that
since the collectibles spawn at random places, the player may spawn on top of a collectible, which will cause the player to immediately consume
the colectible, contributing to the count.

4.1 Created a particle system class, with a inherited class ExplosionParticles. after a certain time(3 seconds) the particles object which holds
all of the projectiles will be set to dissapear.

4.2 after a certain time(1 second), the explosion particles will be converted from the original texture to "ghost mode", ie greyscale. This 
results in a darker texture over time, and as the original explosion texture is a intense color at start, this fulfills the requirement of 
"The particles start with an intense color and become darker over time".

4.3 explosion particles are spawned when player hits a enemy, and also did the bonus where shooting a enemy also causes the explosion particles to spawn.

5.1 SnowParticles inherits particle system, and spawns snow particles at random locations on the screen. The particles will fall downwards, 
and will be recycled after a period of time. Note that the snow particles will be spawned at random locations, and variable speeds(some are way slower than others).

5.2 Particles are small white-ish blue-ish circles, see the snow.png in textures. Although the particles are a bit squareish, hopefully it 
stil looks like snow(At least hail right?).

6.1 Blade rotates over time at a certain fixed position adjacent to the player helicopter sprite.

6.2 The blade sprite transforms relative to the helicopter using a hierarchical transformation, so that the blade appears to be attached to the helicopter
when the helicopter moves.

7.1 Got this Readme lol

7.2 Commented througout the project

Note: Sorry for the really ugly game lmao, and since past requirements aren't being marked hopefully the jank rotations and projectile connections don't count.