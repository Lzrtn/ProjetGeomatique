#version 330 core
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec3 symbology_color;
uniform float symbology_opacity;

varying vec2 v_texcoord;

vec3 ambDirLightColor = vec3(1.0, 0.8, 0.8);  // color for incandescent light
vec3 ambUniLightColor = vec3(0.8, 1.0, 1.0);

in float ambDirLightCoef;
in float ambUniLightCoef;

//! [0]
void main()
{
    // Set fragment color from texture
    vec4 color = (1.0-symbology_opacity) * texture2D(texture, v_texcoord) + symbology_opacity * vec4(symbology_color, 1);
    color.xyz *= (ambDirLightCoef * ambDirLightColor + ambUniLightCoef * ambUniLightColor);
    gl_FragColor = color;
}
//! [0]

