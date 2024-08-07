#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4  transform;

void main(){
    gl_Position =projection * view * model * vec4(position,1.0);
    v_TexCoord=texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main(){
    color = vec4(1.0,0.0,0.0,1.0);
}
