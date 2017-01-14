#version 120

uniform mat4 matrix;
uniform vec3 camera;

attribute vec4 position;
attribute vec3 normal;
attribute vec4 uv;

varying vec2 fragment_uv;
varying float fragment_ao;
varying float fragment_light;
varying float diffuse;

const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));

void main() {
    gl_Position = matrix * position;
    float camera_distance = distance(camera, vec3(position));

    fragment_uv = uv.xy;
    fragment_ao = 0.3 + (1.0 - uv.z) * 0.7;
    fragment_light = uv.w;

    diffuse = max(0.0, dot(normal, light_direction));
}
