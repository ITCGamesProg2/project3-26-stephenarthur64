#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float timer;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord)*colDiffuse*fragColor;

    gl_FragColor = texelColor * vec4(1.0, 1.0 - (timer / 2.0), 1.0 - (timer / 2.0), 1.0);
} 