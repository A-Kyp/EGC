#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <random>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D_t.h"

using namespace std;
using namespace m1;

#define RESPAWNING 3
#define TTL 1000
#define NUM_DUCKS_PER_ROUND 3
#define LIVES 5
#define GUN_AREA_RADIUS 80
#define MAX_BULLETS 3
#define MAX_SCORE 46
#define MIN_SCALING 0.6
#define MAX_SCALING 1

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    ///////////////////////////////////////////   camera set-up   ///////////////////////////////////////////////////
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    ///////////////////////////////////////////   instantiating animation variables   ///////////////////////////////
    translateX = 0;
    translateY = 0;
    angularStep = RADIANS(20);
    //angularStep = 0;

    //create random device so that startinf orientation is not hard coded
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(20, 160);
    int angle = distr(eng);
    if (angle <= 90) {
        if (90 - angle < 20) {
            angle = 70;
        }
    } //makes modifications in case the angle os to close to horizontal or vertical
    else {
        if (angle - 90 < 20) {
            angle = 110;
        }
    }
    StartingAngular = RADIANS(angle);
    
    ///////////////////////////////////////////   instantiating gameplay variables   ////////////////////////////////
    
        //create random device so that coordinates are not hard coded
        std::random_device rd1;
        std::default_random_engine eng1(rd1());
        std::uniform_int_distribution<int> distrX(500, 800);
        std::uniform_int_distribution<int> distrY(0,20);
        std::uniform_int_distribution<int> distrX_variator(-500, 400);
        positionX = distrX(eng1); // where the x coordinate of the center of geometry will be
        positionY = distrY(eng1); // where the y coordinate of the center of geometry will be
        position_variator = distrX_variator(eng1);


        angular_ok = 0;

        //timing related
        ttl = TTL;
        time_to_respawn = RESPAWNING;

        //movement related
        accelaration = 500;
        vertical_acceleration = 500;
        wing_intensity = 2;

        //logic related
        alive = 1;
        can_shoot = 1;
        remaining_lives = LIVES;
        remaining_bullets = MAX_BULLETS;
        num_duck_per_round = NUM_DUCKS_PER_ROUND;
        game_over = false;
        duck_was_shoot = false;

        //score related
        score_color_fill = glm::vec3(0.9, 1, 0.1);
        score = 0;

        //killing spree related
        killing_spree = false;
        consecutive_shots = 0;
        scaling_killing_spree = 0.7;
        scaling_sign = 1;
    
    ///////////////////////////////////////////   mesh initialisations   ////////////////////////////////////////////
    
        Mesh* square = object2D_t::CreateSquare("square", glm::vec3(0, 0, 0), 100, glm::vec3(0, 0, 0), true);
        AddMeshToList(square);

        Mesh* duck_eye = object2D_t::CreateCircle("duck_eye", glm::vec3(positionX + 10, positionY + 10, 0), 10.f, 40, glm::vec3(0, 0, 0), true);
        AddMeshToList(duck_eye);

        Mesh* duck_head = object2D_t::CreateCircle("duck_head", glm::vec3(positionX, positionY, 0), 40.f, 40, glm::vec3(0, 0.2, 0.1), true);
        AddMeshToList(duck_head);

        Mesh* duck_beak = object2D_t::CreateTri("duck_beak", glm::vec3(positionX + 70, positionY - 5, 0),
            glm::vec3(positionX + 30, positionY + 10, 0), glm::vec3(positionX + 30, positionY - 20, 0), glm::vec3(1, 0.5, 0), true);
        AddMeshToList(duck_beak);

        Mesh* duck_body = object2D_t::CreateTri("duck_body", glm::vec3(positionX - 10, positionY, 0),
            glm::vec3(positionX - 140, positionY + 50, 0), glm::vec3(positionX - 140, positionY - 50, 0), glm::vec3(0.4, 0.2, 0), true);
        AddMeshToList(duck_body);

        Mesh* duck_wing1 = object2D_t::CreateTri("duck_wing1", glm::vec3(positionX - 70, positionY + 20, 0),
            glm::vec3(positionX - 40, positionY + 70, 0), glm::vec3(positionX - 100, positionY + 120, 0), glm::vec3(0.4, 0.2, 0), true);
        AddMeshToList(duck_wing1);

        Mesh* duck_wing2 = object2D_t::CreateTri("duck_wing2", glm::vec3(positionX - 70, positionY - 20, 0),
            glm::vec3(positionX - 40, positionY - 70, 0), glm::vec3(positionX - 100, positionY - 120, 0), glm::vec3(0.4, 0.2, 0), true);
        AddMeshToList(duck_wing2);

        Mesh* life = object2D_t::CreateCircle("life", glm::vec3(0, 0, 0), 20.f, 40, glm::vec3(1, 0, 0), true);
        AddMeshToList(life);

        Mesh* heart = object2D_t::CreateHeart("heart", glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
        AddMeshToList(heart);

        Mesh* bullet = object2D_t::CreateBullet("bullet", glm::vec3(0, 0, 0), glm::vec3(0.62, 0.62, 0.62));
        AddMeshToList(bullet);

        Mesh* scoreBracket = object2D_t::CreateScoreBracket("scoreBracket", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
        AddMeshToList(scoreBracket);

        Mesh* scoreBracketWhite = object2D_t::CreateScoreBracket("scoreBracketWhite", glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        AddMeshToList(scoreBracketWhite);

        Mesh* scoreBar = object2D_t::CreateBar("scoreBar", glm::vec3(0, 0, 0), glm::vec3(0.9, 1, 0.1));
        AddMeshToList(scoreBar);

        Mesh* ioana = new Mesh("ioana");
        ioana->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "myModels/ioana"), "untitled.fbx");
        AddMeshToList(ioana);

        Mesh* gameover = new Mesh("gameover");
        gameover->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "gameover"), "untitled.fbx");
        AddMeshToList(gameover);

        Mesh* congrats = new Mesh("congrats");
        congrats->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "congrats"), "untitled.fbx");
        AddMeshToList(congrats);

        Mesh* killingspree = new Mesh("killingspree");
        killingspree->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "killingspree"), "untitled.fbx");
        AddMeshToList(killingspree);
    
}

//only added some background color here
void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8, 1, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    if (game_over == false) {
        glm::ivec2 resolution = window->GetResolution();

        //if killing spree render symbol so that it is in the foreground+ logic for the animation 
        
            if (killing_spree == true) {
                RenderMesh(meshes["killingspree"], glm::vec3(1200, 640, 0),
                    glm::vec3(100 * scaling_killing_spree, 100 * scaling_killing_spree, 1));
            }

            //logic for killing spree animation
            scaling_killing_spree += 0.01 * scaling_sign;
            if (scaling_killing_spree > MAX_SCALING) {
                scaling_sign = -1;
            }
            if (scaling_killing_spree < MIN_SCALING) {
                scaling_sign = 1;
            }
        
        //logic for lives -> if 0 then game over, if >0 corresponding symbols on the screen
        
            int offset_X = 30;
            int offset_Y = 50;
            int copy_lives = remaining_lives;
            if (remaining_lives == 0) {
                game_over = true;
                return;
            }
            while (copy_lives) {
                glm::mat3 modelMatrix_life = glm::mat3(1);
                modelMatrix_life *= transform2D::Translate(resolution.x - offset_X, offset_Y);
                modelMatrix_life *= transform2D::Scale(0.7, 0.7);
                RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix_life);

                offset_X += offset_Y;
                copy_lives--;
            }
        

        //logic for bullets -> if 0 then duck escape, if >0 corresponding symbols on the screen
        
            int offset_X_bullet = 30;
            int offset_Y_bullet = 80;
            int offset_increment = 40;
            int copy_bullet = remaining_bullets;
            if (remaining_bullets == 0) {
                can_shoot = 0;
                ttl = -10;

            }
            while (copy_bullet) {
                glm::mat3 modelMatrix_bullet = glm::mat3(1);
                modelMatrix_bullet *= transform2D::Translate(resolution.x - offset_X_bullet, offset_Y_bullet);
                modelMatrix_bullet *= transform2D::Scale(0.5, 0.5);
                RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix_bullet);

                offset_X_bullet += offset_increment;
                copy_bullet--;
            }
        

        //drawing score braket
        
            glm::mat3 modelMatrix_scoreBracket = glm::mat3(1);
            modelMatrix_scoreBracket *= transform2D::Translate(40, 20);
            RenderMesh2D(meshes["scoreBracket"], shaders["VertexColor"], modelMatrix_scoreBracket);
        

        //drawing score bars coresponding to score + score logic -> if maxim then finish game
        
            int offset_X_score = 40;
            int offset_Y_score = 20;
            //int offset_Y_score = 100;
            glm::mat3 modelMatrix_score = glm::mat3(1);
            modelMatrix_score *= transform2D::Translate(offset_X_score, offset_Y_score);
            modelMatrix_score *= transform2D::Scale(score, 1);
            RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix_score);
            if (score >= MAX_SCORE) {
                score = MAX_SCORE;
                game_over = true;
            }
        

        //drawing a black small background for lives and bullets
        
            glm::mat3 modelMatrix_square = glm::mat3(1);
            modelMatrix_square *= transform2D::Translate(970, 0);
            modelMatrix_square *= transform2D::Scale(4, 1.5);
            RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix_square);
        

        //render the grass on the bottom part of the screen
        RenderMesh(meshes["ioana"], glm::vec3(640, 150, 0), glm::vec3(600, 400, 1));

        //if duck not escaped and not shoot
        if (alive) {
            if (ttl > 0 && duck_was_shoot == false) {
                float direction = StartingAngular;

                //flying animation logic
                translateX += cos(direction) * deltaTimeSeconds * accelaration;
                translateY += sin(direction) * deltaTimeSeconds * accelaration;
                if (direction >= 0 && direction < RADIANS(90)) {
                    if (translateX > resolution.x - positionX - position_variator) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(180) - StartingAngular;
                        //ttl--;
                    }
                    if (translateY > resolution.y - positionY) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(360) - StartingAngular;
                        //ttl--;
                    }
                }
                if (direction >= RADIANS(90) && direction < RADIANS(180)) {
                    if (translateX < - positionX - position_variator) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(180) - StartingAngular;
                        //ttl--;
                    }
                    if (translateY > resolution.y - positionY) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(360) - StartingAngular;
                        //ttl--;
                    }
                }
                if (direction >= RADIANS(180) && direction < RADIANS(270)) {
                    if (translateX < -positionX - position_variator) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(540) - StartingAngular;
                        //ttl--;
                    }
                    if (translateY < - positionY) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(360) - StartingAngular;
                        //ttl--;
                    }
                }
                if (direction >= RADIANS(270) && direction < RADIANS(360)) {
                    if (translateX > resolution.x - positionX - position_variator) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(540) - StartingAngular;
                        //ttl--;
                    }
                    if (translateY < -positionY) {
                        translateX -= cos(direction) * deltaTimeSeconds * accelaration;
                        translateY -= sin(direction) * deltaTimeSeconds * accelaration;
                        StartingAngular = RADIANS(360) - StartingAngular;
                        //ttl--;
                    }
                }
            }
            else if (ttl <= 0 && duck_was_shoot == false){
                can_shoot = 0; //so that you can't lose bullets before new duck comes
                StartingAngular = RADIANS(90);
                translateY += deltaTimeSeconds *  vertical_acceleration;
                // when escaping duck reachese upper bound it desapears and we go on to generate new duck
                if (translateY > resolution.y - positionY) {
                    alive = 0;
                    remaining_lives--;
                    //lose killing spree;
                    consecutive_shots = 0;
                    killing_spree = false;
                }
            }

            if (duck_was_shoot == false) {
                //wing movement logic
                if (angular_ok == 0) {
                    angularStep += wing_intensity * deltaTimeSeconds;
                    if (angularStep > RADIANS(30)) {
                        angular_ok = 1;
                    }
                }
                else {
                    angularStep -= wing_intensity * deltaTimeSeconds;
                    if (angularStep < RADIANS(-40)) {
                        angular_ok = 0;
                    }
                }

                //decrease ttl
                ttl -= deltaTimeSeconds;
            }
            else {
                can_shoot = 0; //once shot cannot lose anymore bullets
                StartingAngular = RADIANS(270);
                translateY -= deltaTimeSeconds * vertical_acceleration;
                // when dying duck reaches bottom bound it desapears and we go on to generate new duck
                if (translateY < - positionY) {
                    alive = 0;
                }
            }

            //render all parts of the duck to obtain it
            
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(translateX, translateY);
                modelMatrix *= transform2D::Translate(position_variator, 0);
                modelMatrix *= transform2D::Translate(positionX, positionY);
                modelMatrix *= transform2D::Rotate(StartingAngular);
                modelMatrix *= transform2D::Translate(-positionX, -positionY);
                RenderMesh2D(meshes["duck_eye"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["duck_beak"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], modelMatrix);

                glm::mat3 matrixWing1 = modelMatrix;
                matrixWing1 *= transform2D::Translate(positionX - 70, positionY + 20);
                matrixWing1 *= transform2D::Rotate(-angularStep);
                matrixWing1 *= transform2D::Translate(-positionX + 70, -positionY - 20);
                RenderMesh2D(meshes["duck_wing1"], shaders["VertexColor"], matrixWing1);

                glm::mat3 matrixWing2 = modelMatrix;
                matrixWing2 *= transform2D::Translate(positionX - 70, positionY - 20);
                matrixWing2 *= transform2D::Rotate(angularStep);
                matrixWing2 *= transform2D::Translate(-positionX + 70, -positionY + 20);
                RenderMesh2D(meshes["duck_wing2"], shaders["VertexColor"], matrixWing2);
            
        }
        else {
            // if waiting time for new duck is over, then create the new one
            if (time_to_respawn < 0) {
                //restart the logic for a round of hunting a duck
                alive = 1;
                can_shoot = 1;
                time_to_respawn = RESPAWNING;
                ttl = TTL;
                duck_was_shoot = false;
                remaining_bullets = MAX_BULLETS;

                std::random_device rd;
                std::default_random_engine eng(rd());
                std::uniform_int_distribution<int> distr(20, 160);
                int angle = distr(eng);
                if (angle <= 90) {
                    if (90 - angle < 20) {
                        angle = 70;
                    }
                }
                else {
                    if (angle - 90 < 20) {
                        angle = 110;
                    }
                }
                StartingAngular = RADIANS(angle);
                
                std::random_device rd1;
                std::default_random_engine eng1(rd1());
                std::uniform_int_distribution<int> distrX_variator(-500, 400);
                position_variator = distrX_variator(eng1);

                translateX = 0;
                translateY = 0;

                //if 5 ducks were spawned increase difficulty (flying speed)
                if (num_duck_per_round == 0) {
                    accelaration *= 1.2;
                    wing_intensity *= 1.2;
                    num_duck_per_round = NUM_DUCKS_PER_ROUND;
                }
                else {
                    num_duck_per_round--;
                }
            }
            else {
                time_to_respawn -= deltaTimeSeconds;
            }
        }
    }
    else {
        glm::mat3 modelMatrix_scoreBracket = glm::mat3(1);
        modelMatrix_scoreBracket *= transform2D::Translate(200, 70);
        RenderMesh2D(meshes["scoreBracketWhite"], shaders["VertexColor"], modelMatrix_scoreBracket);

        int offset_X_score = 200;
        int offset_Y_score = 70;
        glm::mat3 modelMatrix_score = glm::mat3(1);
        modelMatrix_score *= transform2D::Translate(offset_X_score, offset_Y_score);
        modelMatrix_score *= transform2D::Scale(score, 1);
        RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix_score);

        if (score >= 23) {
            RenderMesh(meshes["congrats"], glm::vec3(640, 250, 0), glm::vec3(200, 200, 1));
        }
        
        RenderMesh(meshes["gameover"], glm::vec3(640, 450, 0), glm::vec3(900, 900, 1));
    }
}


void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
    //restart the game -> same things as in init
    if (key == GLFW_KEY_R) {
        angularStep = RADIANS(-20);
        ttl = TTL;
        time_to_respawn = RESPAWNING;

        angular_ok = 1;

        translateX = 0;
        translateY = 0;
        accelaration = 200;
        wing_intensity = 2;

        alive = 1;
        num_duck_per_round = NUM_DUCKS_PER_ROUND;
        remaining_lives = LIVES;
        can_shoot = 1;
        game_over = false;
        duck_was_shoot = false;
        remaining_bullets = MAX_BULLETS;

        score_color_fill = glm::vec3(0.9, 1, 0.1);
        score = 0;

        scaling_killing_spree = 0.7;
        scaling_sign = 1;

        killing_spree = false;

        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(20, 160);
        int angle = distr(eng);
        if (angle <= 90) {
            if (90 - angle < 20) {
                angle = 70;
            }
        }
        else {
            if (angle - 90 < 20) {
                angle = 110;
            }
        }
        StartingAngular = RADIANS(angle);
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    glm::ivec2 resolution = window->GetResolution();
    //if left click
    if (button == GLFW_MOUSE_BUTTON_2 || button == GLFW_MOUSE_BUTTON_LEFT) {
        //if shooting is counted
        if (can_shoot == 1) {
            //if mouse click intersects the duck
            if (abs(mouseX - positionX - translateX - position_variator) < GUN_AREA_RADIUS 
                && abs(resolution.y - mouseY - positionY - translateY) < GUN_AREA_RADIUS) {

                // successfully shooting the duck logic
                duck_was_shoot = true;
                consecutive_shots++;
                if (consecutive_shots >= 3) {
                    score += 2;
                    killing_spree = true;
                }
                else {
                    score++;
                }
            }
        //a click = a bullet
        remaining_bullets--;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
