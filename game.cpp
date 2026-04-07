#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "collectible_game_object.h"
#include "collectible_game_object_gun.h"
#include "collectible_game_object_key.h"
#include "enemy_game_object.h"
#include "enemy_game_object_stationary.h"
#include "enemy_game_object_boxy.h"
#include "game.h"
#include "projectile_object.h"
#include "wall_game_object.h"
#include "particle_system.h"
#include "blade_game_object.h"
#include "user_interface_game_object.h"
#include "text_game_object.h"

#include "timer.h"

#include <random>

namespace game {
    //Amanda Lee



// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;
    enum {
        tex_chopper = 0,//For Player
        tex_ufo = 1,//Now is little robot guys
        tex_orb = 2,
        tex_landscape = 3,
        tex_explosion = 4,
        tex_collectible = 5,
        tex_enemy = 6,
        tex_invunerable = 7,
        tex_projectile = 8,
		tex_transparent = 9,
		tex_sky = 10,
        tex_blade = 11,
		tex_snow = 12,
        tex_howl = 13,
        tex_key = 14,
		tex_fenrir = 15, 
		tex_diamond_red = 16,
        tex_text_font = 17
         };
    // Add the textures in the same order as the enum above
    textures.push_back("/textures/airplane.png"); 
    textures.push_back("/textures/robotgunner.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/landscape.png");
    textures.push_back("/textures/explosion2.png");
    textures.push_back("/textures/bullet.png");
    textures.push_back("/textures/robotgunner.png");
    textures.push_back("/textures/player_invincible.png");
    textures.push_back("/textures/laserBullet.png");
    textures.push_back("/textures/Transparency.png");
    textures.push_back("/textures/tileable_sky.jpg");
    textures.push_back("/textures/blade.png");
    textures.push_back("/textures/snow.png");
    textures.push_back("/textures/howl.png");
    textures.push_back("/textures/pinkcloud.png");
    textures.push_back("/textures/fenrir_wolf.png");
    textures.push_back("/textures/diamond_red.png");
    textures.push_back("/textures/text_font.png");
    // Load all the textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, geometry, shader, texture)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    //
    // Create the player object
    PlayerGameObject *player = new PlayerGameObject(glm::vec3(-2.5f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_chopper], tex_[tex_explosion],3, tex_[tex_invunerable], tex_[tex_howl], tex_[tex_fenrir]);
    // Add the player to the list of all game objects in the game
    game_objects_.push_back(player);
    // Make the object larger
    player->SetScale(2.0); 

    // Offset above player
    glm::vec3 blade_offset(0.0f, 1.0f, 0.0f); // 1 unit above player

    BladeGameObject* blades = new BladeGameObject(
        player,              // parent object
        blade_offset,        // local offset
        sprite_,
        &sprite_shader_,
        tex_[tex_blade],
        tex_[tex_explosion],
        glm::radians(360.0f) // rotation speed
    );
    blades->SetScale(1.5);
    game_objects_.push_back(blades);

    snow_particles_ = new SnowParticles(
        50.0f,        // screen_width
        10.0f,        // screen_height
        sprite_,       // geometry
        &sprite_shader_, // shader
        1000,            // max particles
		tex_[tex_snow]   // texture
    );

    game_objects_.push_back(snow_particles_);

    GameObject* gun = new CollectibleGameObjectGun(glm::vec3(2.0f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_howl], tex_[tex_explosion], 1);
    game_objects_.push_back(gun);

    GameObject* supa = new EnemyGameObjectStationary(glm::vec3(2.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_fenrir], tex_[tex_explosion], 1);
    game_objects_.push_back(supa);

    GameObject* ssupa = new EnemyGameObjectBoxy(glm::vec3(2.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_fenrir], tex_[tex_explosion], 1);
    game_objects_.push_back(ssupa);

    // Setup UI
    glm::vec3 health_offset = glm::vec3(-4.83f, 3.0f, 0.0f);
    GameObject* health_bar = new BladeGameObject(player, health_offset, sprite_, &health_bar_shader_, tex_[0], tex_[0], glm::radians(0.0f));
    health_bar->SetScale(0.7);
    game_objects_.push_back(health_bar);

    glm::vec3 health_text_offset = glm::vec3(-4.5f, 3.6f, 0.0f);
    TextGameObject* health_text = new TextGameObject(player, health_text_offset, sprite_, &text_shader_, tex_[tex_text_font]);
    health_text->SetText("Health");
    health_text->SetScale(1.4f, 0.35f);
    game_objects_.push_back(health_text);

    glm::vec3 time_text_offset = glm::vec3(-2.5f, 3.6f, 0.0f);
    TextGameObject* time_text = new TextGameObject(player, time_text_offset, sprite_, &text_shader_, tex_[tex_text_font]);
    time_text->SetText("Time:");
    time_text->SetScale(1.2f, 0.35f);
    game_objects_.push_back(time_text);

    glm::vec3 timer_offset = glm::vec3(-1.43f, 3.59f, 0.0f);
    TextGameObject* countdown = new TextGameObject(player, timer_offset, sprite_, &text_shader_, tex_[tex_text_font]);
    countdown->SetScale(0.7f, 0.35f);
    game_objects_.push_back(countdown);
    timer_text_ = countdown;

    glm::vec3 keys_text_offset = glm::vec3(0.25f, 3.6f, 0.0f);
    TextGameObject* keys_text = new TextGameObject(player, keys_text_offset, sprite_, &text_shader_, tex_[tex_text_font]);
    keys_text->SetScale(1.2f, 0.35f);
    keys_text->SetText("Keys:");
    game_objects_.push_back(keys_text);

    glm::vec3 keys_offset = glm::vec3(1.1f, 3.59f, 0.0f);
    TextGameObject* num_keys = new TextGameObject(player, keys_offset, sprite_, &text_shader_, tex_[tex_text_font]);
    num_keys->SetScale(0.3f, 0.35f);
    game_objects_.push_back(num_keys);
    num_keys_text_ = num_keys;

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    WallGameObject* wall = new WallGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_transparent], -1);
    wall->SetScale(6.0f, 0.5f);
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_sky], tex_[tex_explosion], -1);
    background->SetScale(1000.0);
    game_objects_.push_back(background);

    //Setting up the tex for enemies and explosion for future enemy generation
    stationary_entity_tex_ = tex_[tex_ufo];
    collectible_entity_tex_ = tex_[tex_collectible];
    enemy_entity_tex_ = tex_[tex_enemy];
    entity_explosion_tex_ = tex_[tex_explosion];
	collectible_key_tex_ = tex_[tex_key];
    projectile_tex_ = tex_[tex_projectile];
	tex_diamond_red_ = tex_[tex_diamond_red];

    //Generate the collectibles)(5)
    for (int i = 0;i < 5;i++) {
        SpawnEntity('C');
    }
    for (int i = 0;i < 3;i++) {
        SpawnEntity('K');
    }
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
}


void Game::HandleControls(double delta_time)
{
    shoot_cooldown_ -= delta_time;

    // Player is always index 0
    GameObject* player_base = game_objects_[0];

    // We need the derived type for physics velocity
    auto* player = static_cast<PlayerGameObject*>(player_base);

    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();

    // Direction vectors
    glm::vec3 dir = player->GetBearing();
    glm::vec3 right = player->GetRight();

    // Movement tuning
    float speed = (float)(delta_time * 500.0);
    float motion_increment = 0.001f * speed;
    float angle_increment = (glm::pi<float>() / 1800.0f) * speed;

    // Physics impulse for arrow keys (FIXED SIZE)
    const float impulse = 0.35f;

    // Terrain parameters
    float terrain_height = -2.1f;

    // Stop player movement after dying
    if (player->HasExploded()) return;

    glm::vec2 mouse;
    if (GetMousePosition(mouse)) {
        // Player position in 2D (same plane as mouse)
        glm::vec2 p(curpos.x, curpos.y);

        // Offset mouse position by player position
        glm::vec2 mousePos(mouse.x + player->GetPosition().x, mouse.y + player->GetPosition().y);

        // Direction from player to mouse
        glm::vec2 d = mousePos - p;

        // Avoid NaN when mouse is exactly on player
        if (glm::length(d) > 0.0001f) {
            float angle = atan2(d.y, d.x);   // radians
            player->SetRotation(angle);
        }
    }

    const float jump_force = 6.0f;
    const float sideways_force = 3.0f;

    bool pressW = glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS;
    bool pressA = glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS;
    bool pressD = glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS;

    if (pressW) {

        glm::vec3 jump(0.0f, jump_force, 0.0f);

        if (pressA)
            jump.x -= sideways_force;

        if (pressD)
            jump.x += sideways_force;

        player->Jump(jump);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
    }
    //New Arrow keys logic
    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS) {
        player->AddVelocity(-right * impulse);
    }
    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player->AddVelocity(right * impulse);
    }
    if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        player->AddVelocity(dir * impulse);
    }
    if (glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS) {
        player->AddVelocity(-dir * impulse);
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
    }

    if (glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS) {
        if (player->isFurry() && toggle_timer_.Finished()) {
			player->FurryToggle();

			toggle_timer_.Start(0.5);
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS && shoot_cooldown_ <= 0.0) {
        glm::vec3 spawn_pos = player->GetPosition() + player->GetBearing() * 0.6f;

        shoot_cooldown_ = shoot_interval_;

        // Direction the player is currently facing (already updated by mouse aim)
        glm::vec3 direction = player->GetBearing();

        if (player->CurrentFurry()) {
            ProjectileObject* projectile = new ProjectileObject(spawn_pos, sprite_, &sprite_shader_, projectile_tex_, direction);
            projectile->SetRotation(player->GetRotation() + (270 * glm::pi<float>() / 180));
            projectile->SetSpeed(12.0f); // faster speed when powered up
            game_objects_.insert(game_objects_.end() - 1, projectile);
        }
        else {
            ProjectileObject* projectile = new ProjectileObject(spawn_pos, sprite_, &sprite_shader_, tex_diamond_red_, direction);
            projectile->SetRotation(player->GetRotation() + (270 * glm::pi<float>() / 180));
            game_objects_.insert(game_objects_.end() - 1, projectile);
        }
    }
}


void Game::Update(double delta_time)
{
    PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);

    // Update countdown timer
    int sec_remaining = std::ceil(game_timer_.GetRemaining());
    std::string sec_string = std::to_string(sec_remaining);
    timer_text_->SetText(sec_string + "s");

    // Update keys collected
    std::string keys_string = std::to_string(player->GetNumKeys());
    num_keys_text_->SetText(keys_string);
    
    //Checks if each object currently exists, and removing if needed
    bool player_exists = true;
    for (size_t i = 0; i < game_objects_.size(); ) {

        if (!game_objects_[i]->return_exist()) {

            if (i == 0) {
                player_exists = false;
            }
            delete game_objects_[i];
            game_objects_.erase(game_objects_.begin() + i);
            continue;
        }

        ++i;
    }

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object if not exploded
        if (!current_game_object->HasExploded()) { current_game_object->Update(delta_time); }

        // Check for collision with other game objects
        //
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];

            HandleCollision(current_game_object, other_game_object);
        }
    }
    //If the player no longer exits, ends the game
    if (!player_exists || game_timer_.Finished()) {
        std::cout << "GAME OVER\n" << std::endl;
        glfwSetWindowShouldClose(window_, true);
    }

    // Update current game time
    current_time_ += delta_time;

    //Spawns copies
    if (entity_spawn_timer_.Finished()) {
        SpawnEntity('E');
        entity_spawn_timer_.Start(10.0);
    }
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix(1.0f);
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    // Get player (always index 0)
    GameObject* player = game_objects_[0];
    glm::vec3 player_pos = player->GetPosition();

    // Camera follows player → translate world in opposite direction
    glm::mat4 camera_translation = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-player_pos.x, -player_pos.y, 0.0f)
    );

    // Zoom
    glm::mat4 camera_zoom_matrix = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(camera_zoom_, camera_zoom_, camera_zoom_)
    );

    // Final view matrix
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_translation;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {

        GameObject* obj = game_objects_[i];
        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);

        // Enable shader BEFORE setting uniforms
        sprite_shader_.Enable();
        health_bar_shader_.Enable();

        // Health bar
        float player_health = 0.0f;
        if (player->getHitPoints() == 3) { player_health = 4.0f; }
        else if (player->getHitPoints() == 2) { player_health = 2.0f; }
        else if (player->getHitPoints() == 1) { player_health = 1.0f; }
        health_bar_shader_.SetUniform1f("bar_length", player_health);

        // Background = last object
        if (i == game_objects_.size() - 1) {
            sprite_shader_.SetUniform1i("tile_texture", 1); // enable tiling
            sprite_shader_.SetUniform1f("tile_factor", 250.0f); // 10 repetitions
        }
        else {
            sprite_shader_.SetUniform1i("tile_texture", 0); // normal sprites
        }

        obj->Render(view_matrix, current_time_);
    }
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


bool Game::GetMousePosition(glm::vec2 &mouse){

    // Get mouse pixel position in the window
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    // Get dimensions of the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    // If the mouse is outside the window, return false
    if (x < 0 || x > width || y < 0 || y > height) {
        return false;
    }

    // Get position in world coordinates corresponding to the mouse
    // pixel position on the window
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        mouse.x = ((2.0f*x - width)*aspect_ratio)/(width*camera_zoom_);
        mouse.y = (-2.0f*y + height)/(height*camera_zoom_);
    } else {
        float aspect_ratio = height/width;
        mouse.x = (2.0f*x - width)/(width*camera_zoom_);
        mouse.y = ((-2.0f*y + height)*aspect_ratio)/(height*camera_zoom_);
    }

    return true;
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize shaders
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());
    health_bar_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/health_fragment_shader.glsl")).c_str());
    text_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/text_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;

    // Set zoom factor
    camera_zoom_ = 0.25f;

    entity_spawn_timer_.Start(10.0);

    snow_spawn_timer_.Start(3.0);

    game_timer_.Start(60.0);
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

//Random function(with assistance from ChatGPT)
static float RandFloat(float a, float b)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng);
}

//Main logic for spawning entities
//S for Stationary, C for collectible, E for Enemy, P for Player
void Game::SpawnEntity(char type)
{
    if (game_objects_.empty()) return;

    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    float halfW, halfH;
    if (width > height) {
        float aspect = (float)width / (float)height;
        halfW = aspect / camera_zoom_;
        halfH = 1.0f / camera_zoom_;
    }
    else {
        float aspect = (float)height / (float)width;
        halfW = 1.0f / camera_zoom_;
        halfH = aspect / camera_zoom_;
    }

    float x = RandFloat(-halfW + 0.5f, halfW - 0.5f);
    float y = RandFloat(-halfH + 0.5f, halfH - 3.0f);


    //Logic to check which entity to generate(C used to be the enemies from last assignment but were revamped into E)
    if (type == 'K') {
        GameObject* key_piece = new CollectibleGameObjectKey(glm::vec3(x, -0.5f, 0.0f), sprite_, &sprite_shader_, collectible_key_tex_, entity_explosion_tex_, 1);
        game_objects_.insert(game_objects_.end() - 1, key_piece);
    } else if (type == 'C') {
        GameObject* entity = new CollectibleGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, collectible_entity_tex_, entity_explosion_tex_, 1);
        game_objects_.insert(game_objects_.end() - 1, entity);
    }
    else if (type == 'S') {
        GameObject* entity = new GameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, stationary_entity_tex_, entity_explosion_tex_, 1);
        game_objects_.insert(game_objects_.end() - 1, entity);
    }
    else if(type == 'E'){
        auto* enemy = new EnemyGameObject(
            glm::vec3(x, y, 0.0f),
            sprite_,
            &sprite_shader_,
            enemy_entity_tex_,
            entity_explosion_tex_,
            1
        );

        // Give enemy access to the player 
        enemy->SetPlayer(game_objects_[0]);

        // Patrol ellipse: center at spawn, with some size
        enemy->SetPatrolEllipse(glm::vec3(x, y, 0.0f), 3.0f, 1.5f);
        enemy->SetPatrolAngularSpeed(1.2f);
        enemy->SetInterceptTriggerRadius(2.0f);
        enemy->SetDesiredInterceptTime(2.0f);
        enemy->SetCourseCorrectionPeriod(2.0f);

        game_objects_.insert(game_objects_.end() - 1, enemy);
    }

}

bool Game::RayCircleIntersect(
    float ox, float oy,     // ray origin
    float dx, float dy,     // ray direction (normalized)
    float cx, float cy,     // circle center
    float r,                // circle radius
    float maxDist           // maximum ray length (projectile travel this frame)
) {
    float Lx = cx - ox;
    float Ly = cy - oy;

    float tca = Lx * dx + Ly * dy;
    if (tca < 0) return false;

    float Lsq = Lx * Lx + Ly * Ly;
    float dsq = Lsq - tca * tca;
    float rsq = r * r;

    if (dsq > rsq) return false;

    float thc = sqrt(rsq - dsq);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return false;
    }

    // Only collide if intersection happens within this frame's movement
    if (t0 > maxDist) return false;

    return true;
}


void Game::HandleCollision(GameObject* a, GameObject* b) {

    if (!a->return_interactable() || !b->return_interactable())
        return;

    float x1 = a->return_x();
    float y1 = a->return_y();
    float r1 = a->return_radius();

    float x2 = b->return_x();
    float y2 = b->return_y();
    float r2 = b->return_radius();

    bool a_is_proj = (a->GetType() == GameObjectType::Projectile);
    bool b_is_proj = (b->GetType() == GameObjectType::Projectile);

	// ray and circle collision (for projectiles)
    if (a_is_proj || b_is_proj) {

        GameObject* proj = a_is_proj ? a : b;
        GameObject* target = a_is_proj ? b : a;

        if (!proj->return_interactable() || !target->return_interactable()) {
            return;
        }

        glm::vec3 ppos = proj->GetPosition();
        glm::vec3 pvel = proj->GetVelocity();

        float ox = ppos.x;
        float oy = ppos.y;

        float dx = pvel.x;
        float dy = pvel.y;

        float maxDist = sqrt(dx * dx + dy * dy);
        if (maxDist == 0) return;

        // normalize direction
        dx /= maxDist;
        dy /= maxDist;

        float cx = target->return_x();
        float cy = target->return_y();
        float r = target->return_radius();

        if (RayCircleIntersect(ox, oy, dx, dy, cx, cy, r, maxDist)) {
            if (!proj->return_interactable()) return;
            proj->collide(target);
            target->collide(proj);

            if (target->return_interactable() == false) {
            
                glm::vec3 explosion_pos = target->GetPosition();

                ExplosionParticles* explosion = new ExplosionParticles(
                    explosion_pos,
                    sprite_,
                    &sprite_shader_,     // you must have this loaded
                    200,                    // number of particles
                    entity_explosion_tex_
                );
				game_objects_.insert(game_objects_.end() - 1, explosion);
            }
        }

        return;
    }

	// circle and circle collision
    float dx = x2 - x1;
    float dy = y2 - y1;
    float r = r1 + r2;

    if (dx * dx + dy * dy <= r * r) {
        bool a_is_enemy = (a->GetType() == GameObjectType::Enemy);
        bool b_is_enemy = (b->GetType() == GameObjectType::Enemy);
        bool a_is_player = (a->GetType() == GameObjectType::Player);
        bool b_is_player = (b->GetType() == GameObjectType::Player);

        // Case 1: a = enemy, b = player
        if (a_is_enemy && b_is_player && a->return_interactable()) {

            glm::vec3 explosion_pos = a->GetPosition();

            ExplosionParticles* explosion = new ExplosionParticles(
                explosion_pos,
                sprite_,
                &sprite_shader_,
                200,
                entity_explosion_tex_
            );

            game_objects_.insert(game_objects_.end() - 1, explosion);
        }

        // Case 2: b = enemy, a = player
        if (b_is_enemy && a_is_player && b->return_interactable()) {

            glm::vec3 explosion_pos = b->GetPosition();

            ExplosionParticles* explosion = new ExplosionParticles(
                explosion_pos,
                sprite_,
                &sprite_shader_,
                200,
                entity_explosion_tex_
            );

            game_objects_.insert(game_objects_.end() - 1, explosion);
        }

        a->collide(b);
        b->collide(a);
    }
}

} // namespace game


