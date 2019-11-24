#version 460
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform DATA_BLOCK {
    mat4 basicTransform;
    mat4 localTransforms[256];
} data;

layout(push_constant) uniform PUSH_CONST{
    uint id;
} pushconst;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = (vec4(pos, 1, 1) * data.basicTransform * data.localTransforms[pushconst.id]);
    uvOut = uv;
}