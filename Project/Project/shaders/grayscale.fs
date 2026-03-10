#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 circleCentre;
uniform float radius;

// NOTE: Add your custom variables here

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord)*colDiffuse*fragColor;

    float distanceSqrd = ((fragTexCoord.x - circleCentre.x) * (fragTexCoord.x - circleCentre.x)) + ((fragTexCoord.y - circleCentre.y) * (fragTexCoord.y - circleCentre.y));

    float gray = distanceSqrd;

    if (distanceSqrd < radius)
    {
        // Convert texel color to grayscale using NTSC conversion weights
        gray = dot(texelColor.rgb, vec3(0.1, 0.587, 0.114));
        gl_FragColor = vec4(gray, gray, gray, texelColor.a);
    }
    else
    {
        gl_FragColor = texelColor;
    }

} 