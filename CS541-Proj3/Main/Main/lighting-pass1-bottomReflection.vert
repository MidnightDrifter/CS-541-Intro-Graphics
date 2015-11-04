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
//out vec3 R, RNorm;
out vec4 currentPos;
out float depth;



void main()
{      
    tangent = vertexTangent;
    texCoord = vertexTexture;

		vec4 centerOfReflection = vec4(0.0, 0.0, 0.0, 1.0);
	 

    normalVec = normalize(mat3(NormalMatrix)*vertexNormal);    
    worldPos = (ModelMatrix*vertex).xyz;
    //vec3 worldVertex = vec3(ModelMatrix * vertex);
    eyeVec = (ViewInverse*vec4(0,0,0,1)).xyz - worldPos;
    lightVec = lightPos - worldPos;
	vec4 worldVertex = ModelMatrix * vertex;

	//vec3 V = normalize(eyeVec);
	//vec3 N = normalize(normalVec);

	//vec4 currentPos = worldVertex / worldVertex.w;
	//float length = length(currentPos.xyz);
	//currentPos = currentPos/length;
//	currentPos.z +=1;
	//currentPos.x = currentPos.x/currentPos.z;
	//currentPos.y=currentPos.y/currentPos.z;
	//currentPos.z = (length - 0.9)/100;
	//currentPos.w = 1;
	
	
	//vec3 eyeVec = currentPos.xyz; //Center of reflection is at origin, so no need to subtract
	//gl_Position = currentPos;
	
	
	transformEyeVec = worldPos - centerOfReflection.xyz;
	
	vec3 R = worldPos;
	vec3 RNorm = normalize(R);
	float depth = 1-(RNorm.z);
	vec4 currentPos = vec4((RNorm.x/depth), (RNorm.y/depth), ( -RNorm.z*(length(R)/100) -0.9), 1);
	
	gl_Position = currentPos;
	//vec3((RNorm.x/depth), (RNorm.y/depth), (RNorm.z * (length(R)/100) -0.9)) * ModelMatrix * vertex;
	
	
	
	
	//vec4((RNorm.x/depth), (RNorm.y/depth), (RNorm.z * (length(R)/100) -0.9), 1);
	
	//vec3 eyeVec = currentPos.xyz; //Center of reflection is at origin, so no need to subtract
	//gl_Position = currentPos;



    //gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vertex;
}
