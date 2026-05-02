#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 position;
    vec3 tangent;
    vec3 normal;
} gs_in[];

uniform mat4 transform;
uniform mat4 viewMat;
uniform mat4 perspectiveMat;

uniform float lineWidth;

out vec3 color;

void main() {
    vec3 pos = gs_in[0].position;
    vec3 tangent = normalize(gs_in[0].tangent);
    vec3 normal = normalize(gs_in[0].normal);
    vec3 bitangent = normalize(cross(normal, tangent));

    // Tangent (green)
    gl_Position = perspectiveMat * viewMat * transform * vec4(pos, 1.0);
    color = vec3(0, 1, 0); // Red for tangent
    EmitVertex();

    // End point (vertex position + scaled tangent)
    gl_Position = perspectiveMat * viewMat * transform * vec4(pos + tangent * lineWidth, 1.0);
    color = vec3(0, 1, 0);
    EmitVertex();

    EndPrimitive();

    // Normal (blue)
    gl_Position = perspectiveMat * viewMat * transform * vec4(pos, 1.0);
    color = vec3(0, 0, 1);
    EmitVertex();

    gl_Position = perspectiveMat * viewMat * transform * vec4(pos + normal * lineWidth, 1.0);
    color = vec3(0, 0, 1);
    EmitVertex();

    EndPrimitive();

    // Bitangent (red)
    gl_Position = perspectiveMat * viewMat * transform * vec4(pos, 1.0);
    color = vec3(1, 0, 0);
    EmitVertex();

    gl_Position = perspectiveMat * viewMat * transform * vec4(pos + bitangent * lineWidth, 1.0);
    color = vec3(1, 0, 0);
    EmitVertex();

    EndPrimitive();
}