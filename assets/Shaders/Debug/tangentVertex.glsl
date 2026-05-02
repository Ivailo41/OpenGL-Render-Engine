#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 position;
    vec3 tangent;
    vec3 normal;
} vs_out;

void main() {
    vs_out.position = aPos;
    vs_out.tangent = aTangent;
    vs_out.normal = aNormal;
    gl_Position = vec4(aPos, 1.0);
}