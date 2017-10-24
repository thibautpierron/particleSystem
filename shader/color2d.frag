#version 400

in float  color;

out vec4 outputColor;

void main(){
    outputColor = vec4(1, 1 - (color / 15), 0.0, 1.0);
}
