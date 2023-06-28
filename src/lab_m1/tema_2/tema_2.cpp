#include "lab_m1/tema_2/tema_2.h"
#include "lab_m1/tema_2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema_2::tema_2()
{
}


tema_2::~tema_2()
{
}


void tema_2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(64.63 + 2.0f, 2.0f , -21.63), glm::vec3(64.63, 1, -21.63), glm::vec3(0, 1, 0));
    //camera->Set(glm::vec3(64.63, 2.0f , 2.0f - 21.63), glm::vec3(64.63, 1, -21.63), glm::vec3(0, 1, 0));
    //camera->Set(glm::vec3(49.56, 2.0f , 2.0f - 1.24), glm::vec3(49.56, 1, -1.24), glm::vec3(0, 1, 0));

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    right = 20.f;
    left = -20.f;
    bottom = -10.f;
    top = 10.f;
    fov = 40.f;
    /////ADDED CODE HERE - circuit positions
    {
        circuit_positions = 
        {
            glm::vec3(64.63, 0, -21.63),
            glm::vec3(58.14, 0, -22.81),
            glm::vec3(51, 0, -22.79),
            glm::vec3(44.19, 0, -21.93),
            glm::vec3(38.82, 0, -23.69),
            glm::vec3(32.35, 0, -26.01),
            glm::vec3(27.6, 0, -29.79),
            glm::vec3(23.87, 0, -31.77),
            glm::vec3(18.21, 0, -31.66),
            glm::vec3(14.42, 0, -30.28),
            glm::vec3(9.79, 0, -26.86),
            glm::vec3(5.64, 0, -24.42),
            glm::vec3(0.76, 0,-23.45),
            glm::vec3(-4.37, 0, -23.69), 
            glm::vec3(-10.22, 0, -25.64),
            glm::vec3(-15.1, 0, -28.33),
            glm::vec3(-29.5, 0, -32.48),
            glm::vec3(-42.19, 0, -32.48),
            glm::vec3(-45.62, 0, -32.71),
            glm::vec3(-49.02, 0, -31.99),
            glm::vec3(-51.73, 0, -30.77),
            glm::vec3(-53.9, 0, -28.57),
            glm::vec3(-58.16, 0, -25.06),
            glm::vec3(-60, 0, -20),
            glm::vec3(-66.35, 0, -3.44),
            glm::vec3(-66.59, 0, 10.96),
            glm::vec3(-60.98, 0, 17.3),
            glm::vec3(-53.42, 0, 21.7),
            glm::vec3(-44.14, 0, 22.67),
            glm::vec3(-28.53, 0, 18.52),
            glm::vec3(-21.62, 0, 16.53),
            glm::vec3(-13.88, 0, 15.84),
            glm::vec3(-8.76, 0, 16.33),
            glm::vec3(-4.14, 0, 17.46),
            glm::vec3(0, 0, 20),
            glm::vec3(24.06, 0, 37.76),
            glm::vec3(33.85, 0, 42.22),
            glm::vec3(45.43, 0, 42.66),
            glm::vec3(53, 0, 40.43),
            glm::vec3(65.02, 0, 33.31),
            glm::vec3(73.48, 0, 21.51),
            glm::vec3(78.38, 0, 4.15),
            glm::vec3(75.26, 0, -11.44),
            glm::vec3(74.34, 0, -15.5),
            glm::vec3(70, 0, -20),
        };

        tree_positions = {};
        for (int i = 0; i < circuit_positions.size() - 2; i++) {
            glm::vec3 P1 = circuit_positions[i];
            glm::vec3 P2 = circuit_positions[(i + 1)];
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
            for (int j = 1; j < 6; j++) {
                float x = j;
                int random_int = rand() % 10 + 4;
                float random_float = random_int * 1.0f;
                glm::vec3 R = P1 + random_float * P + (x / 6.0f) * D;
                glm::vec3 A = P1 - random_float * P + (x / 6.0f) * D;
                tree_positions.push_back(R);
                tree_positions.push_back(A);
            }
        }

        enemy1_positions = {};
        for (int i = 0; i < circuit_positions.size() - 2; i++) {
            glm::vec3 P1 = circuit_positions[i];
            glm::vec3 P2 = circuit_positions[(i + 1)];
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
            for (int j = 1; j < 50; j++) {
                float x = j;
                //int random_int = rand() % 3;
                float random_float = 2.0f;
                glm::vec3 R = P1 + random_float * P + (x / 50.0f) * D;
                enemy1_positions.push_back(R);
            }
        }

        enemy2_positions = {};
        for (int i = 0; i < circuit_positions.size() - 2; i++) {
            glm::vec3 P1 = circuit_positions[i];
            glm::vec3 P2 = circuit_positions[(i + 1)];
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
            for (int j = 1; j < 100; j++) {
                float x = j;
                //int random_int = rand() % 3;
                float random_float = -1.0f;
                glm::vec3 R = P1 + random_float * P + (x / 100.0f) * D;
                enemy2_positions.push_back(R);
            }
        }

        enemy3_positions = {};
        for (int i = 0; i < circuit_positions.size() - 2; i++) {
            glm::vec3 P1 = circuit_positions[i];
            glm::vec3 P2 = circuit_positions[(i + 1)];
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
            for (int j = 1; j < 250; j++) {
                float x = j;
                //int random_int = rand() % 3;
                float random_float = -1.0f;
                glm::vec3 R = P1 + random_float * P + (x / 250.0f) * D;
                enemy3_positions.push_back(R);
            }
        }

        vector<VertexFormat> circuit_vertices{};
        for (int i = 0; i < circuit_positions.size() - 1; i ++) {
            glm::vec3 P1 = circuit_positions[i];
            glm::vec3 P2 = circuit_positions[(i + 1)];
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
            for (int j = 1; j < 50; j++) {
                float x = j;
                glm::vec3 R = P1 + 3.0f * P + (x / 50.0f) * D;
                glm::vec3 A = P1 - 3.0f * P + (x / 50.0f) * D;
                circuit_vertices.push_back(VertexFormat(R, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.2, 0.8, 0.6)));
                circuit_vertices.push_back(VertexFormat(A, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.2, 0.8, 0.6)));
            }
            
        }


        vector<unsigned int> circuit_indices{};
        for (int i = 0; i < circuit_vertices.size() - 2; i += 2) {
            circuit_indices.push_back(i);
            circuit_indices.push_back(i + 1);
            circuit_indices.push_back(i + 2);
            circuit_indices.push_back(i + 1);
            circuit_indices.push_back(i + 2);
            circuit_indices.push_back(i + 3);
        }
        circuit_indices.push_back(circuit_vertices.size() - 2);
        circuit_indices.push_back(circuit_vertices.size() - 1);
        circuit_indices.push_back(0);
        circuit_indices.push_back(circuit_vertices.size() - 1);
        circuit_indices.push_back(0);
        circuit_indices.push_back(1);

        CreateMesh("circuit", circuit_vertices, circuit_indices);

    }


    /////ADDED CODE HERE - tree meshes

    {
        vector<VertexFormat> tree_vertices
        {
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0.3, 0.1, 0.0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0.3, 0.1, 0.0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0.54, 0.27, 0.07), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0.54, 0.27, 0.07), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.3, 0.1, 0.0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.3, 0.1, 0.0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.54, 0.27, 0.07), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.54, 0.27, 0.07), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<unsigned int> tree_indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };
        CreateMesh("tree", tree_vertices, tree_indices);
    }

    {
        vector<VertexFormat> tree1_vertices
        {
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 0.27, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.3, 0.4, 0), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<VertexFormat> tree2_vertices
        {
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0, 0.37, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0, 0.37, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0.3, 1, 0.4), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0.3, 1, 0.4), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0.37, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0.37, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.3, 1, 0.4), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.3, 1, 0.4), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<VertexFormat> tree3_vertices
        {
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0.5, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0.5, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.5, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.5, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<unsigned int> tree1_indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangl
            // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };
        CreateMesh("tree1", tree1_vertices, tree1_indices);
        CreateMesh("tree2", tree2_vertices, tree1_indices);
        CreateMesh("tree3", tree3_vertices, tree1_indices);
    }

    /////ADDED CODE HERE - car meshes

    {
        vector<VertexFormat> car_vertices
        {
            VertexFormat(glm::vec3(0, 0, 2), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 2), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 2), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 2), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<VertexFormat> car_vertices1
        {
            VertexFormat(glm::vec3(0, 0, 2), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 2), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 2), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 2), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.75, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.75, 0.75, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.75, 0.75, 0), glm::vec3(0.2, 0.8, 0.6)),

        };

        vector<unsigned int> car_indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };
        CreateMesh("car", car_vertices, car_indices);
        CreateMesh("car1", car_vertices1, car_indices);
    }
    
    CreateGrass("grass");

    // Initialize tx, ty and tz (the translation steps)
    translateX = 64.63;
    translateY = 0;
    translateZ = -21.63;

    //transX = 64.63;
    //transY = 0;
    //transZ = -21.63;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    //angularStepOY = 0;
    angularStepOY = RADIANS(90);
    angStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution2 = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution2.x / 5.f, resolution2.y / 5.f);

  

    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema_2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema_2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

Mesh* tema_2::CreateTriangle(
    const std::string& name,
    glm::vec3 leftAngle,
    glm::vec3 rightAngle,
    glm::vec3 topAngle,
    glm::vec3 color)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftAngle, color),
        VertexFormat(rightAngle, color),
        VertexFormat(topAngle, color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 2, 1, 0 };
    triangle->SetDrawMode(GL_TRIANGLES);
    triangle->InitFromData(vertices, indices);
    return triangle;
}



void tema_2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void tema_2::FrameStart()
{
    
}

void tema_2::CreateGrass(const char* name) {
    std::vector<VertexFormat> vertices = {};
    for (int i = -300; i < 200; i += 4) {
        for (int j = -300; j < 200; j += 4) {
            vertices.push_back(VertexFormat(glm::vec3(i, 0, j), glm::vec3(0, 0.5, 0), glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(glm::vec3(i, 0, j + 4), glm::vec3(0, 0.5, 0), glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(glm::vec3(i + 4, 0, j), glm::vec3(0, 0.5, 0), glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(glm::vec3(i + 4, 0, j + 4), glm::vec3(0, 0.5, 0), glm::vec3(0.2, 0.8, 0.6)));
        }
    }
    std::vector<unsigned int> indices = {};
    for (int i = 0; i < vertices.size() - 4; i++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
    }
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void tema_2::RenderScene(float deltaTimeSeconds) {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.65f);
    modelMatrix *= transform3D::Translate(-0.5f, 0, -0.5f);
    //modelMatrix *= transform3D::Translate(-25.5f, 0, 20.f);
    RenderMesh(meshes["car1"], shaders["TemaShader"], modelMatrix);

    //enemies
    //modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(80, 0, 0);
    //modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.65f);
    //RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);
    //modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(0, 0, -20);
    //modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.65f);
    //RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);
    //modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(0, 0, 15);
    //modelMatrix *= transform3D::Scale(0.55f, 0.55f, 0.65f);
    //RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);
    // 
    //grass
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, -0.1, 0);
    RenderMesh(meshes["grass"], shaders["TemaShader"], modelMatrix);
      
   

    //tree
    for (int i = 0; i < tree_positions.size() - 1; i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(tree_positions[i].x, tree_positions[i].y, tree_positions[i].z);
        modelMatrix *= transform3D::Scale(0.5f, 1, 0.5f);
        modelMatrix *= transform3D::Translate(0, -0.1, 0);
        RenderMesh(meshes["tree"], shaders["TemaShader"], modelMatrix);
        modelMatrix *= transform3D::Scale(2, 1, 2);

        //int tree_type_determiner = rand() % 2;
        if (i % 3 != 0) {
            modelMatrix *= transform3D::Translate(-0.25, 1, -0.25);
            RenderMesh(meshes["tree1"], shaders["TemaShader"], modelMatrix);

            modelMatrix *= transform3D::Translate(0.65, -0.15, 0.25);
            modelMatrix *= transform3D::Scale(0.2, 0.2, 0.2);
            RenderMesh(meshes["tree3"], shaders["TemaShader"], modelMatrix);
        }
        else {
            modelMatrix *= transform3D::Translate(-1.7, 1, -0.25);
            RenderMesh(meshes["tree2"], shaders["TemaShader"], modelMatrix);

            modelMatrix *= transform3D::Scale(2, 2, 2);
            modelMatrix *= transform3D::Translate(0.4, -0.1, -0.25);
            RenderMesh(meshes["tree2"], shaders["TemaShader"], modelMatrix);
        }

    }

    //enemy1
    glm::vec3 e1_crt_pos = enemy1_positions[e1_index];
    e1_index = (e1_index + 1) % (enemy1_positions.size());
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(e1_crt_pos.x, e1_crt_pos.y, e1_crt_pos.z);
    modelMatrix *= transform3D::RotateOY(angStepOY);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);

    float distance = ((e1_crt_pos.x - translateX) * (e1_crt_pos.x - translateX)) + ((e1_crt_pos.z - translateZ) * (e1_crt_pos.z - translateZ));
    if (distance < 2 * raza) {
        accident = true;
    }
    else {
        accident = false;
    }

    //enemy2
    glm::vec3 e2_crt_pos = enemy2_positions[e2_index];
    e2_index = (e2_index + 1) % (enemy2_positions.size());
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(e2_crt_pos.x, e2_crt_pos.y, e2_crt_pos.z);
    modelMatrix *= transform3D::RotateOY(angStepOY);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);

    distance = ((e2_crt_pos.x - translateX) * (e2_crt_pos.x - translateX)) + ((e2_crt_pos.z - translateZ) * (e2_crt_pos.z - translateZ));
    if (distance < 2 * raza) {
        accident = true;
    }

    //enemy3
    glm::vec3 e3_crt_pos = enemy3_positions[e3_index];
    e3_index = (e3_index + 1) % (enemy3_positions.size());
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(e3_crt_pos.x, e3_crt_pos.y, e3_crt_pos.z);
    modelMatrix *= transform3D::RotateOY(angStepOY);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["car"], shaders["TemaShader"], modelMatrix);

    distance = ((e3_crt_pos.x - translateX) * (e3_crt_pos.x - translateX)) + ((e3_crt_pos.z - translateZ) * (e3_crt_pos.z - translateZ));
    if (distance < 2 * raza) {
        accident = true;
    }
        
    //road 
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0, 0.01, 0);
    RenderMesh(meshes["circuit"], shaders["TemaShader"], modelMatrix);
}

void tema_2::Update(float deltaTimeSeconds)
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.53, 0.81, 0.92, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution2 = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution2.x, resolution2.y);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene(deltaTimeSeconds);

    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix = glm::ortho(left, right, bottom, top, -100.f, 400.f);
    RenderScene(deltaTimeSeconds);
   
}


void tema_2::FrameEnd()
{
    /*DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);*/
}


void tema_2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint location = glGetUniformLocation(shader->GetProgramID(), "car_position");
    glm::vec3 car_position = glm::vec3(translateX, translateY, translateZ);
    glUniform3fv(location, 1, glm::value_ptr(car_position));
    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema_2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 10.0f;
        if (window->KeyHold(GLFW_KEY_F)) {
            cameraSpeed = 25.0f;
        }

        if (!accident) {
            if (window->KeyHold(GLFW_KEY_S))
            {
                float tempZ = translateZ + deltaTime * cameraSpeed * cos(angularStepOY);
                float tempX = translateX + deltaTime * cameraSpeed * sin(angularStepOY);

                bool ok = false;
                for (int i = 0; i < circuit_positions.size() - 1; i++) {
                    glm::vec3 P1 = circuit_positions[i];
                    glm::vec3 P2 = circuit_positions[i + 1];
                    glm::vec3 P3 = glm::vec3(tempX, 0, tempZ);

                    float u = ((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z)) / (length(P2 - P1) * length(P2 - P1));

                    if ((u < -0.2f) || (u > 1.2f)) {
                        continue;
                    }
                    glm::vec3 D = P2 - P1;
                    glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
                    glm::vec3 R = P1 + 3.0f * P;

                    float x = P1.x + u * (P2.x - P1.x);
                    float z = P1.z + u * (P2.z - P1.z);
                    float d = distance(P3, glm::vec3(x, 0, z));

                    if (d < distance(R, P1)) {
                        ok = true;
                        break;
                    }
                }

                glm::vec3 P1 = circuit_positions[circuit_positions.size() - 1];
                glm::vec3 P2 = circuit_positions[0];
                glm::vec3 P3 = glm::vec3(tempX, 0, tempZ);

                float u = ((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z)) / (length(P2 - P1) * length(P2 - P1));

                if ((u < -0.2f) || (u > 1.2f)) {

                }
                else {
                    glm::vec3 D = P2 - P1;
                    glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
                    glm::vec3 R = P1 + 3.0f * P;

                    float x = P1.x + u * (P2.x - P1.x);
                    float z = P1.z + u * (P2.z - P1.z);
                    float d = distance(P3, glm::vec3(x, 0, z));

                    if (d < distance(R, P1)) {
                        ok = true;
                    }
                }

                if (ok) {
                    translateZ += deltaTime * cameraSpeed * cos(angularStepOY);
                    translateX += deltaTime * cameraSpeed * sin(angularStepOY);
                    camera->MoveForward(-deltaTime * cameraSpeed);
                }
            }
            if (window->KeyHold(GLFW_KEY_W))
            {
                float tempZ = translateZ - deltaTime * cameraSpeed * cos(angularStepOY);
                float tempX = translateX - deltaTime * cameraSpeed * sin(angularStepOY);

                bool ok = false;
                for (int i = 0; i < circuit_positions.size() - 1; i++) {
                    glm::vec3 P1 = circuit_positions[i];
                    glm::vec3 P2 = circuit_positions[i + 1];
                    glm::vec3 P3 = glm::vec3(tempX, 0, tempZ);

                    float u = ((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z)) / (length(P2 - P1) * length(P2 - P1));

                    if ((u < -0.2f) || (u > 1.2f)) {
                        continue;
                    }
                    glm::vec3 D = P2 - P1;
                    glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
                    glm::vec3 R = P1 + 3.0f * P;

                    float x = P1.x + u * (P2.x - P1.x);
                    float z = P1.z + u * (P2.z - P1.z);
                    float d = distance(P3, glm::vec3(x, 0, z));

                    if (d < distance(R, P1)) {
                        ok = true;
                        break;
                    }
                }
                glm::vec3 P1 = circuit_positions[circuit_positions.size() - 1];
                glm::vec3 P2 = circuit_positions[0];
                glm::vec3 P3 = glm::vec3(tempX, 0, tempZ);

                float u = ((P3.x - P1.x) * (P2.x - P1.x) + (P3.z - P1.z) * (P2.z - P1.z)) / (length(P2 - P1) * length(P2 - P1));

                if ((u < -0.2f) || (u > 1.2f)) {

                }
                else {
                    glm::vec3 D = P2 - P1;
                    glm::vec3 P = glm::normalize(cross(D, glm::vec3(0, 1, 0)));
                    glm::vec3 R = P1 + 3.0f * P;

                    float x = P1.x + u * (P2.x - P1.x);
                    float z = P1.z + u * (P2.z - P1.z);
                    float d = distance(P3, glm::vec3(x, 0, z));

                    if (d < distance(R, P1)) {
                        ok = true;
                    }
                }
                if (ok) {
                    translateZ -= deltaTime * cameraSpeed * cos(angularStepOY);
                    translateX -= deltaTime * cameraSpeed * sin(angularStepOY);
                    camera->MoveForward(deltaTime * cameraSpeed);
                }
            }
            if (window->KeyHold(GLFW_KEY_D))
            {
                if (window->KeyHold(GLFW_KEY_F)) {
                    angularStepOY -= 2 * deltaTime;
                    camera->RotateThirdPerson_OY(-2 * deltaTime);
                }
                else {
                    angularStepOY -= deltaTime;
                    camera->RotateThirdPerson_OY(-deltaTime);
                }
                
            }
            if (window->KeyHold(GLFW_KEY_A))
            {
                if (window->KeyHold(GLFW_KEY_F)) {
                    angularStepOY += 2 * deltaTime;
                    camera->RotateThirdPerson_OY(2 * deltaTime);
                }
                else {
                    angularStepOY += deltaTime;
                    camera->RotateThirdPerson_OY(deltaTime);
                }
            }
        }
        
        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }

}


void tema_2::OnKeyPress(int key, int mods)
{
    // Add key press event
    
}


void tema_2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema_2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
    }
}


void tema_2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void tema_2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema_2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema_2::OnWindowResize(int width, int height)
{
}
