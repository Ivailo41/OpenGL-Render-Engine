#version 330 core
in vec4 FragPos;

uniform float far_plane;
uniform vec3 lightPosition;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPosition);
    // Map to [0,1] range for depth
    gl_FragDepth = lightDistance / far_plane;
}
