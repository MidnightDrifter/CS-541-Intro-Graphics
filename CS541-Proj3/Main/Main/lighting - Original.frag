/////////////////////////////////////////////////////////////////////////
// Pixel shader for the final pass
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform int mode;               // 0..9, used for debugging
uniform bool direct;            // Direct color -- no lighting calculation

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightValue, lightAmbient;

uniform sampler2D groundTexture;

in vec3 normalVec, lightVec;
 in vec3 eyeVec;
in vec2 texCoord;
in vec3 worldPos
void main()
{



	vec3 V = normalize(eyeVec);
    vec3 N = normalize(normalVec);
    vec3 L = normalize(lightVec);
	vec3 H = normalize(L+V);


    vec3 Kd;
    if (textureSize(groundTexture,0).x>1) // Is the texture defined?
        Kd = texture(groundTexture, 2.0*texCoord.st).xyz;
    else
        Kd = diffuse;

    if (direct || mode>0)
        gl_FragColor.xyz = diffuse;
    else
        // Really *cheap* but effective lighting calculation.
        gl_FragColor.xyz = max(0.0, dot(L, N))*Kd;

}
