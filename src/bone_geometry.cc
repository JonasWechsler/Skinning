#include "config.h"
#include "bone_geometry.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>

/*
 * For debugging purpose.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	size_t count = std::min(v.size(), static_cast<size_t>(10));
	for (size_t i = 0; i < count; ++i) os << i << " " << v[i] << "\n";
	os << "size = " << v.size() << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& bounds)
{
	os << "min = " << bounds.min << " max = " << bounds.max;
	return os;
}

bool Bone::intersects(glm::vec3 position, glm::vec3 direction, float radius){
    return false;
}

void Bone::roll(float radians){

}

//Assumes parents are initialized
Bone::Bone(glm::vec3 offset, Bone* parent):parent(parent){
    glm::vec4 origin = glm::vec4(0.0);
    glm::mat4 parent_transform = glm::mat4(1.0);
    if(parent != nullptr){
        origin = parent->endpoint();
        parent_transform = parent->transform();
    }
    glm::vec4 origin_transform = glm::inverse(parent_transform) * origin;
    T = glm::translate(glm::vec3(origin_transform));

    glm::mat4 parent_rotation = glm::mat4(1.0);
    if(parent != nullptr){
        parent_rotation = parent->rotation();
    }
    L = glm::length(offset);

    glm::vec4 relative_offset = glm::transpose(parent_rotation) * glm::vec4(offset, 0.0);
    glm::vec3 t = glm::normalize(glm::vec3(relative_offset));
    std::cout << t << " ";
    t = glm::normalize(glm::vec3(glm::inverse(parent_transform*T) * (origin+glm::vec4(offset,0.0))));
    std::cout << t << std::endl;

    glm::vec3 v = t;
    if(std::abs(v.x) <= std::abs(v.y) && std::abs(v.x) <= std::abs(v.z)){
        v = glm::vec3(1.0, 0.0, 0.0);
    }else if(std::abs(v.y) <= std::abs(v.x) && std::abs(v.y) <= std::abs(v.z)){
        v = glm::vec3(0.0, 1.0, 0.0);
    }else{
        v = glm::vec3(0.0, 0.0, 1.0);
    }
    glm::vec3 n = glm::normalize(glm::cross(t, v));
    glm::vec3 b = glm::normalize(glm::cross(t, n));

    R[0] = glm::vec4(t, 0.0);
    R[1] = glm::vec4(n, 0.0);
    R[2] = glm::vec4(b, 0.0);
    R[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);
}

Skeleton::Skeleton(){}

Skeleton::Skeleton(std::vector<glm::vec3> offsets, std::vector<int> parents, std::vector<SparseTuple> weight):weights(weight){
    std::map<int, std::vector<int>> children;
    for(int id = 0; id < static_cast<int>(parents.size()); ++id){
        children[parents[id]].push_back(id);
    }

    int root = children[-1][0];
    assert(children[-1].size() == 1);
    _id_to_bone[-1] = nullptr;

    std::queue<int> id_queue;
    id_queue.push(root);
    while(id_queue.size() != 0){
        int id = id_queue.front();
        id_queue.pop();
        glm::vec3 offset = offsets[id];

        int pid = parents[id];
        Bone* parent = _id_to_bone[pid];

        _id_to_bone[id] = new Bone(offset, parent);

        for(int cid = 0; cid < static_cast<int>(children[id].size()); ++cid){
            id_queue.push(children[id][cid]);
        }
    }
}

void Skeleton::create_bone_geometry(std::vector<glm::vec4>& bone_vertices, std::vector<glm::uvec2>& bone_lines){
    for(int id = 0; id < size(); id++){
        bone_vertices.push_back(_id_to_bone[id]->origin());
        bone_vertices.push_back(_id_to_bone[id]->endpoint());
        bone_lines.push_back(glm::uvec2(id*2, id*2+1));
    }
}

int Skeleton::get_bone_by_intersection(glm::vec3 position, glm::vec3 direction, float radius){
    return -1;
}

// FIXME: Implement bone animation.


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::loadpmd(const std::string& fn)
{
	MMDReader mr;
	mr.open(fn);
	mr.getMesh(vertices, faces, vertex_normals, uv_coordinates);
	computeBounds();
	mr.getMaterial(materials);
    
    std::vector<glm::vec3> offsets;
    std::vector<int> parents;

    glm::vec3 offset;
    int parent;
    int i=0;
    while(mr.getJoint(i++, offset, parent)){
        offsets.push_back(offset);
        parents.push_back(parent);
    }
    
    std::vector<SparseTuple> weights;
    mr.getJointWeights(weights);
    skeleton = Skeleton(offsets, parents, weights);
	// FIXME: load skeleton and blend weights from PMD file
	//        also initialize the skeleton as needed
}

void Mesh::updateAnimation()
{
	animated_vertices = vertices;
	// FIXME: blend the vertices to animated_vertices, rather than copy
	//        the data directly.
}


void Mesh::computeBounds()
{
	bounds.min = glm::vec3(std::numeric_limits<float>::max());
	bounds.max = glm::vec3(-std::numeric_limits<float>::max());
	for (const auto& vert : vertices) {
		bounds.min = glm::min(glm::vec3(vert), bounds.min);
		bounds.max = glm::max(glm::vec3(vert), bounds.max);
	}
}

