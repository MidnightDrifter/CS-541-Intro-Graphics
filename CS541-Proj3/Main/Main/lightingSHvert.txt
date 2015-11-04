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


//Earth textures

//in vec2 earthBaseTexture;
//in vec2 redEarthTexture;

in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

out vec3 tangent;
out vec2 texCoord;
out vec3 worldPos;
out vec3 normalVec, lightVec, eyeVec;



//out vec2 earthBaseTextureCoord;


//out vec2 redEarthTextureCoord;
void main()
{      
    tangent = vertexTangent;
    texCoord = vertexTexture;

	//earthBaseTextureCoord=earthBaseTexture;
	//redEarthTextureCoord = redEarthTexture;

	 

    normalVec = normalize(mat3(NormalMatrix)*vertexNormal);    
    worldPos = (ModelMatrix*vertex).xyz;
    //vec3 worldVertex = vec3(ModelMatrix * vertex);
    eyeVec = (ViewInverse*vec4(0,0,0,1)).xyz - worldPos;
    lightVec = lightPos - worldPos;
	//worldPos = (ModelMatrix*vertex).xyz;
    gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vertex;
}
