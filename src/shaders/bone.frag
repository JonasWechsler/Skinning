R"zzz(
#version 330 core
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
out vec4 fragment_color;
void main() {
    fragment_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
)zzz"
