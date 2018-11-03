R"zzz(
#version 330 core
in vec4 vertex_position;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float radius;
void main() {
	vec4 pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float tau = 2 * 3.141592654;
    pos.x = vertex_position.x;
	pos.y = cos(tau * vertex_position.y) * radius;
	pos.z = sin(tau * vertex_position.y) * radius;

	gl_Position = projection * view * model * pos;
}
)zzz"
