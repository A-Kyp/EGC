#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include <vector>

namespace m1
{
    class tema_2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };
         tema_2();
        ~tema_2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        Mesh* CreateTriangle(const std::string& name, glm::vec3 leftAngle, glm::vec3 rightAngle, glm::vec3 topAngle, glm::vec3 color);
        void RenderScene(float deltaTimeSeconds);
        void CreateGrass(const char* name);
     protected:
        glm::mat4 modelMatrix;
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        ViewportArea miniViewportArea;
        // TODO(student): If you need any other class variables, define them here.
        float translateX, translateY, translateZ;
        float transX, transY, transZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ, angStepOY;
        GLfloat fov;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        bool projectionType;

        glm::vector<glm::vec3> tree_positions;
        glm::vector<glm::vec3> enemy1_positions;
        glm::vector<glm::vec3> enemy2_positions;
        glm::vector<glm::vec3> enemy3_positions;
        glm::vector<glm::vec3> circuit_positions;

        int e1_index = 0;
        int e2_index = 0;
        int e3_index = 0;

        bool accident = false;
        float raza = 0.4f;
    };
}   // namespace m1
