#ifndef BONE_GEOMETRY_H
#define BONE_GEOMETRY_H

#include <ostream>
#include <vector>
#include <map>
#include <limits>
#include <glm/glm.hpp>
#include <mmdadapter.h>

struct BoundingBox {
	BoundingBox()
		: min(glm::vec3(-std::numeric_limits<float>::max())),
		max(glm::vec3(std::numeric_limits<float>::max())) {}
	glm::vec3 min;
	glm::vec3 max;
};

struct Joint {
	// FIXME: Implement your Joint data structure.
	// Note: PMD represents weights on joints, but you need weights on
	//       bones to calculate the actual animation.
};

class Bone {
    public:
    bool intersects(glm::vec3 position, glm::vec3 direction, float radius);

    glm::mat4 transform(){
        if(parent == nullptr)
            return T*R;
        return parent->transform()*T*R;
    }

    glm::mat4 rotation(){
        if(parent == nullptr)
            return R;
        return parent->rotation()*R;
    }

    glm::vec4 t(){ return rotation()[0]; }
    glm::vec4 n(){ return rotation()[1]; }
    glm::vec4 b(){ return rotation()[2]; }
    glm::vec4 origin(){
        if(parent == nullptr)
            return T*glm::vec4(0,0,0,1);
        return parent->transform()*T*glm::vec4(0,0,0,1);
    }
    glm::vec4 endpoint(){
        if(parent == nullptr)
            return glm::vec4(L,0,0,1);
        return parent->transform()*glm::vec4(L,0,0,1);
    }
    void roll(float radians);

    private:
    Bone* parent;
    double L;
    glm::mat4 T, R;
};

class Skeleton {
    public:
    void create_bone_geometry(std::vector<glm::vec4>& bone_vertices, std::vector<glm::uvec2>& bone_lines);
    int get_bone_by_intersection(glm::vec3 position, glm::vec3 direction, float radius);
    Bone* id_to_bone(int id){ return _id_to_bone[id]; }
    Bone* root(){ return _root; }
    private:
    std::map<int, Bone*> _id_to_bone;
    Bone* _root;
	// FIXME: create skeleton and bone data structures
};

struct Mesh {
	Mesh();
	~Mesh();
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> animated_vertices;
	std::vector<glm::uvec3> faces;
	std::vector<glm::vec4> vertex_normals;
	std::vector<glm::vec4> face_normals;
	std::vector<glm::vec2> uv_coordinates;
	std::vector<Material> materials;
	BoundingBox bounds;
	Skeleton skeleton;

	void loadpmd(const std::string& fn);
	void updateAnimation();
	int getNumberOfBones() const 
	{ 
		return 0;
		// FIXME: return number of bones in skeleton
	}
	glm::vec3 getCenter() const { return 0.5f * glm::vec3(bounds.min + bounds.max); }
private:
	void computeBounds();
	void computeNormals();
};

#endif
