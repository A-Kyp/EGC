#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        //base matrix for common operations used on the duck
        glm::mat3 modelMatrix;

        //modifiable parameters for animations
        float translateX, translateY;
        float scaleX, scaleY;
        float StartingAngular;
        float angularStep;

        //gameplay variables
        int ttl;                    //how much does a duck need for escaping
        bool alive;                 //if the duck is alive (not dead, not escaped)
        float time_to_respawn;      //how much t owait in between the apparitions of duck
        int accelaration;           //increases the speed of the duck
        int vertical_acceleration;  //used for the escaping and dead animations
        int angular_ok;             //switching direction of the wing animations
        float wing_intensity;       //how fast do the wings move
        float positionX;            //base X position for whole geometry
        float positionY;            //base Y position for whole geometry
        int position_variator;      //used to respawn ducks in different positions
        int num_duck_per_round;     //how many ducks before next round (before increasing speed)

        int remaining_lives;        //how many lives left
        //int num_shot_ducks;       
        int can_shoot;              //activate the click to consume bullets
        bool game_over;             //either you dead wither reached max score
        bool duck_was_shoot;        //if the shot was succesful 

        int remaining_bullets;      //how many bullets left for this round
        glm::vec3 score_color_fill; //parameter for score mesh
        int score;                  //score of the player

        bool killing_spree;         //if the player is in a killing spree
        int consecutive_shots;      //counter for the consecutive succesful shots used to determine the killing spree
        int scaling_sign;           //whether the scaling factor for the killing free is >1 or <1, can be 1 or -1 

        float scaling_killing_spree;//scaling factor for the killing spree symbol

    };
}
