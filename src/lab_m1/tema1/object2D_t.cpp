#include "object2D_t.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


//original function from lab3 - creates a square
Mesh* object2D_t::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

//function which returns the mesh pointer for a circle
// numOfVertexes -> how many points from circle circumference there are, the more points the more realistic
Mesh* object2D_t::CreateCircle(const std::string& name, glm::vec3 center, float radius, int numOfVertexes, glm::vec3 color, bool fill)
{
    glm::vec3 centerPoint = center;
    float doublePI = 2 * 3.142;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= numOfVertexes; i++) {
        glm::vec3 newPoint(center.x + (radius * cos(i * doublePI / numOfVertexes)),
            center.y + (radius * sin(i * doublePI / numOfVertexes)),
            center.z);
        vertices.push_back(VertexFormat(newPoint, color));
        if (i == numOfVertexes) {
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(i);
        }
        else {
            indices.push_back(0);
            indices.push_back(i + 2);
            indices.push_back(i + 1);
        }
    }

    Mesh* circle = new Mesh(name);
    circle->InitFromData(vertices, indices);
    return circle;
}

//function which returns the mesh pointer for a triangle, determined by all 3 corners
Mesh* object2D_t::CreateTri(const std::string& name, glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 color, bool fill)
{
    glm::vec3 vertex1 = corner1;
    glm::vec3 vertex2 = corner2;
    glm::vec3 vertex3 = corner3;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(vertex1, color));
    vertices.push_back(VertexFormat(vertex2, color));
    vertices.push_back(VertexFormat(vertex3, color));

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);

    Mesh* triangle = new Mesh(name);
    triangle->InitFromData(vertices, indices);
    return triangle;
}

//function which returns the mesh pointer for a bullet with hand-made disign
Mesh* object2D_t::CreateBullet(const std::string& name, glm::vec3 base, glm::vec3 color)
{
    glm::vec3 v0 = base;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float x = v0.x;
    float y = v0.y;

    vertices.push_back(VertexFormat(v0, color)); //0
    vertices.push_back(VertexFormat(v0 + glm::vec3(-20, -10, 0), color)); //1
    vertices.push_back(VertexFormat(v0 + glm::vec3(20, -10, 0), color + glm::vec3(-0.2, -0.2, -0.2))); //2
    vertices.push_back(VertexFormat(v0 + glm::vec3(10, 0, 0), color)); //3
    vertices.push_back(VertexFormat(v0 + glm::vec3(20, 10, 0), color + glm::vec3(-0.2, -0.2, -0.2))); //4
    vertices.push_back(VertexFormat(v0 + glm::vec3(20, 70, 0), color + glm::vec3(-0.2, -0.2, -0.2))); //5 
    vertices.push_back(VertexFormat(v0 + glm::vec3(10, 90, 0), color)); //6
    vertices.push_back(VertexFormat(v0 + glm::vec3(10, 100, 0), color + glm::vec3(-0.2, -0.2, -0.2))); //7
    vertices.push_back(VertexFormat(v0 + glm::vec3(0, 120, 0), color + glm::vec3(-0.2, -0.2, -0.2))); //8
    vertices.push_back(VertexFormat(v0 + glm::vec3(-10, 100, 0), color)); //9
    vertices.push_back(VertexFormat(v0 + glm::vec3(-10, 90, 0), color)); //10
    vertices.push_back(VertexFormat(v0 + glm::vec3(-20, 70, 0), color)); //11
    vertices.push_back(VertexFormat(v0 + glm::vec3(-20, 10, 0), color)); //12
    vertices.push_back(VertexFormat(v0 + glm::vec3(-10, 0, 0), color)); //13

    for (int i = 1; i <= 12; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i+1);
    }
    indices.push_back(0);
    indices.push_back(13);
    indices.push_back(1);

    Mesh* bullet = new Mesh(name);
    bullet->SetDrawMode(GL_TRIANGLE_FAN);
    bullet->InitFromData(vertices, indices);
    return bullet;
}

//function which returns the mesh pointer for a score bracket (basically an empty square)
//refer to the sketch for more details
Mesh* object2D_t::CreateScoreBracket(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(corner, color));
    vertices.push_back(VertexFormat(corner + glm::vec3(920, 0, 0), color));
    vertices.push_back(VertexFormat(corner + glm::vec3(920, 50, 0), color));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, 50, 0), color));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);

    Mesh* scoreBracket = new Mesh(name);
    scoreBracket->SetDrawMode(GL_LINE_LOOP);
    scoreBracket->InitFromData(vertices, indices);
    return scoreBracket;
}

//function which returns the mesh pointer for a score bar (the visual element representing the score)
Mesh* object2D_t::CreateScoreBar(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(corner, color));
    vertices.push_back(VertexFormat(corner + glm::vec3(20, 0, 0), color + glm::vec3(-0.5, 0, 0.2)));
    vertices.push_back(VertexFormat(corner + glm::vec3(20, 50, 0), color + glm::vec3(-0.5, 0, 0.2)));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, 50, 0), color));

    //vertices.push_back(VertexFormat(corner, color));
    //vertices.push_back(VertexFormat(corner + glm::vec3(20, 0, 0), color));
    //vertices.push_back(VertexFormat(corner + glm::vec3(20, 50, 0), color));
    //vertices.push_back(VertexFormat(corner + glm::vec3(0, 50, 0), color));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);

    Mesh* scoreBar = new Mesh(name);
    scoreBar->SetDrawMode(GL_POLYGON);
    scoreBar->InitFromData(vertices, indices);
    return scoreBar;
}

Mesh* object2D_t::CreateBar(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(corner, color));
    vertices.push_back(VertexFormat(corner + glm::vec3(20, 0, 0), color + glm::vec3(-0.5, 0, 0.2)));
    vertices.push_back(VertexFormat(corner + glm::vec3(20, 50, 0), color + glm::vec3(-0.5, 0, 0.2)));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, 50, 0), color));

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(1);
    //indices.push_back(0);
    //indices.push_back(2);

    Mesh* scoreBar = new Mesh(name);
    scoreBar->SetDrawMode(GL_QUADS);
    scoreBar->InitFromData(vertices, indices);
    return scoreBar;
}


//function which returns the mesh pointer for a heart with hand-made disign
//refer to the sketch for more details
Mesh* object2D_t::CreateHeart(const std::string& name, glm::vec3 center, glm::vec3 color)
{
    glm::vec3 v0 = center;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float x = v0.x;
    float y = v0.y;

    vertices.push_back(VertexFormat(v0, color)); //0
    vertices.push_back(VertexFormat(v0 + glm::vec3(0, 10, 0), color)); //1
    vertices.push_back(VertexFormat(v0 + glm::vec3(-10, 20, 0), color)); //2
    vertices.push_back(VertexFormat(v0 + glm::vec3(-20, 20, 0), color)); //3
    vertices.push_back(VertexFormat(v0 + glm::vec3(-30, 10, 0), color)); //4
    vertices.push_back(VertexFormat(v0 + glm::vec3(-30, 0, 0), color)); //5 
    vertices.push_back(VertexFormat(v0 + glm::vec3(-20, -10, 0), color)); //6
    vertices.push_back(VertexFormat(v0 + glm::vec3(-10, -20, 0), color + glm::vec3(0, 0.4, 0.4))); //7
    vertices.push_back(VertexFormat(v0 + glm::vec3(0, -30, 0), color + glm::vec3(0, 0.5, 0.6))); //8
    vertices.push_back(VertexFormat(v0 + glm::vec3(10, -20, 0), color + glm::vec3(0, 0.4, 0.4))); //9
    vertices.push_back(VertexFormat(v0 + glm::vec3(20, -10, 0), color)); //10
    vertices.push_back(VertexFormat(v0 + glm::vec3(30, 0, 0), color)); //11
    vertices.push_back(VertexFormat(v0 + glm::vec3(30, 10, 0), color)); //12
    vertices.push_back(VertexFormat(v0 + glm::vec3(20, 20, 0), color)); //13
    vertices.push_back(VertexFormat(v0 + glm::vec3(10, 20, 0), color)); //14

    for (int i = 1; i <= 13; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(0);
    indices.push_back(14);
    indices.push_back(1);

    Mesh* heart = new Mesh(name);
    heart->SetDrawMode(GL_TRIANGLE_FAN);
    heart->InitFromData(vertices, indices);
    return heart;
}


