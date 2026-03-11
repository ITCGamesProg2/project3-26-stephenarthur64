#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float grayStrength;
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
        if (distanceSqrd > radius - 0.01)
        {
            //gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
            gl_FragColor = vec4(1.0 - texelColor.r, 1.0 - texelColor.g, 1.0 - texelColor.b, texelColor.a);
        }
        else
        {
            // Convert texel color to grayscale using NTSC conversion weights
            gray = dot(texelColor.rgb, vec3(grayStrength, grayStrength, 0.1));

            if (grayStrength > 0.6)
            {
                gray = dot(texelColor.rgb, vec3(0.5, 0.5, 0.1));
                gl_FragColor = vec4(gray, gray, gray, texelColor.a);
            }
            else
            {
                gl_FragColor = vec4(mix(1.0 - texelColor.r, gray, grayStrength), mix(1.0 - texelColor.g, gray, grayStrength), mix(1.0 - texelColor.b, gray, grayStrength), texelColor.a);
            }
        }
    }
    else
    {
        gl_FragColor = texelColor;
    }

} 