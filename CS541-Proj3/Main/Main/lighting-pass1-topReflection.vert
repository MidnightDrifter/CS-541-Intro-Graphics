/////////////////////////////////////////////////////////////////////////
// Vertex shader for the final pass
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix, ViewInverse;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

uniform vec3 lightPos;




in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

out vec3 tangent;
out vec2 texCoord;
out vec3 worldPos;

out vec3 normalVec, lightVec, eyeVec, transformEyeVec;
out vec3 R, RNorm, currentPos;
out float depth;



void main()
{      
    tangent = vertexTangent;
    texCoord = vertexTexture;


	vec4 centerOfReflection = vec4(0.0, 0.0, 0.0, 1.0);
	

    normalVec = normalize(mat3(NormalMatrix)*vertexNormal);    
    worldPos = (ModelMatrix*vertex).xyz;
    vec3 worldVertex = vec3(ModelMatrix * vertex);
    eyeVec = (ViewInverse*vec4(0,0,0,1)).xyz - worldPos;
	//transformEyeVec = (ViewInverse*vec4(0,0,0,1)).xyz - centerOfReflection.xyz;
    transformEyeVec = worldPos - centerOfReflection.xyz;

	lightVec = lightPos - worldPos;
	

	vec3 V = normalize(lightVec);
	vec3 N = normalize(normalVec);


	
	vec3 R =worldPos;
	float length = length(R);
	vec3 RNorm = normalize(R);
	float depth = 1+(RNorm.z);
	vec4 currentPos = vec4((RNorm.x/depth), (RNorm.y/depth), ((RNorm.z * length)/100)-0.9, 1);
	//eyeVec = (ViewInverse*vec4(0,0,0,1)).xyz - centerOfReflection;
	
	gl_Position = currentPos;
	//gl_position = ModelMatrix*vertex;


    //gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vertex;
}
