#version 400

layout(location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform vec4 gravityPosition;

out float    color;

void main()
{
    gl_Position = (projectionMatrix * modelMatrix) * vec4(position, 1.0);
    gl_PointSize = 2.0;
    color = distance(gravityPosition, vec4(position, 1.0));
}