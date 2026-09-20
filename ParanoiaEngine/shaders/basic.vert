#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;      // Исправлено: vec4
layout (location = 2) in vec2 aTexCoord;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 uViewProjection; // Убрали transform

void main(){
    Color = aColor;
    TexCoord = aTexCoord;
    gl_Position = uViewProjection * vec4(aPos, 0.0, 1.0);
}