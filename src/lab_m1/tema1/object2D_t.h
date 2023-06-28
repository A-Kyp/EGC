#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_t
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, int numOfVertexes, glm::vec3 color, bool fill = false);
    Mesh* CreateTri(const std::string& name, glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 color, bool fill = false);
    Mesh* CreateBullet(const std::string& name, glm::vec3 base, glm::vec3 color);
    Mesh* CreateScoreBracket(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color);
    Mesh* CreateScoreBar(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color);
    Mesh* CreateBar(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color);
    Mesh* CreateHeart(const std::string& name, glm::vec3 center, glm::vec3 color);
}
