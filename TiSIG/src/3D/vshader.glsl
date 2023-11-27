#version 330 core
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform float power_light;
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

out float ambDirLightCoef;
out float ambUniLightCoef;

vec3 ambDirLightDirection = vec3(0.5, -0.5, 1.0);

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position, 1.0);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;

    ambDirLightDirection = normalize(ambDirLightDirection);
    vec3 normal = normalize(vec3(mvp_matrix * vec4(a_normal, 0.0)));

    ambDirLightCoef = pow(clamp(-dot(ambDirLightDirection, normal), 0.0, 1.0), power_light);
    ambUniLightCoef = 0.4;
}
