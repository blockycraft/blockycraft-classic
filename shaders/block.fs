#version 120

uniform sampler2D sampler;
uniform float daylight;
uniform int transparency;
uniform int texturing;

varying vec2 fragment_uv;
varying float fragment_ao;
varying float fragment_light;
varying float diffuse;

void main() {
    vec3 color = vec3(texture2D(sampler, fragment_uv));
    if (!bool(texturing)) {
        color = vec3(1.0, 1.0, 1.0);
    }

    if (color == vec3(1.0, 0.0, 1.0) && bool(transparency)) {
        discard;
    }

    float df = min(1.0, diffuse + fragment_light);
    float ao = min(1.0, fragment_ao + fragment_light);
    float value = min(1.0, daylight + fragment_light);
    vec3 light = vec3(value * 0.3 + 0.2) + vec3(value * 0.3 + 0.2) * df;
    
    color = clamp(color * light * ao, vec3(0.0), vec3(1.0));    

    gl_FragColor = vec4(color, 1.0);
}
