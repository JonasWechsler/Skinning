#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>

class LineMesh;

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
void create_cylinder_lattice(std::vector<glm::vec4>& cylinder_vertices, std::vector<glm::uvec2>& cylinder_lines);
// FIXME: Add functions to generate the bone mesh.

#endif
