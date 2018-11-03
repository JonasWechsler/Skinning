#include "procedure_geometry.h"
#include "bone_geometry.h"
#include "config.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

void create_cylinder_lattice(std::vector<glm::vec4>& cylinder_vertices, std::vector<glm::uvec2>& cylinder_lines){
    const int resolution = 10;
    const float step = 1.0/float(resolution);
    for(int i=0;i<resolution;i++){
        for(int j=0;j<resolution;j++){
            float x = i*step;
            float y = j*step;
            cylinder_vertices.push_back(glm::vec4(x, y, 0.0, 1.0));
            int idx = i*resolution + j;
            int left = idx+1;
            int top = idx+resolution;

            if(j < resolution-1)
                cylinder_lines.push_back(glm::uvec2(idx, left));
            if(i < resolution-1)
                cylinder_lines.push_back(glm::uvec2(idx, top));
            else
                cylinder_lines.push_back(glm::uvec2(idx, idx%resolution));
        }
    }
}

// FIXME: create cylinders and lines for the bones
// Hints: Generate a lattice in [-0.5, 0, 0] x [0.5, 1, 0] We wrap this
// around in the vertex shader to produce a very smooth cylinder.  We only
// need to send a small number of points.  Controlling the grid size gives a
// nice wireframe.
