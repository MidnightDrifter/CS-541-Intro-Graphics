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
uniform sampler2D tex;
uniform sampler2D earthBaseTexture;
uniform sampler2D redEarthTexture;




in vec3 normalVec, lightVec;
in vec3 eyeVec;
in vec2 texCoord;
in vec3 worldPos;

//in vec2 earthBaseTextureCoord;
//in vec2 redEarthTextureCoord;


float PI = 3.14159;

float LN(vec3 light, vec3 normal)
{
return max(dot(light, normal), 0.0);
}





vec3 BRDF(vec3 eye, vec3 normal, vec3 light, vec3 dif, vec3 spec, float shiny)
{

float alpha = pow(8192, shiny);
//alpha.x = pow(8192, shininess.x);
//alpha.y=pow(8192, shininess.y);
//alpha.z=pow(8192, shininess.z);


	vec3 V = normalize(eye);
    vec3 N = normalize(normal);
    vec3 L = normalize(light);
	vec3 H = normalize(L+V);

	float LN = max(dot(L,N), 0.0);
	float HN = max(dot(H,N), 0.0);
	float LH = max(dot(L,H), 0.0);

	vec3 mySpec;// = (1-specular.x, 1-specular.y, 1-specular.z); 
	mySpec.x = 1-spec.x;
	mySpec.y=1-spec.y;
	mySpec.z=1-spec.z;
	vec3 F = spec + (mySpec)*(pow((1-LH), 5));
	float D = ((alpha+2)/(2*PI))*(pow(HN, alpha)); 
	
	//Using approx of G() / (LN * VN) = approx  1/(LH*LH)

	vec3 BRDF = (F*D)/(4*LH*LH) + (dif / PI);
	
	return BRDF;


}


vec3 BRDF( vec3 dif, vec3 spec, float shiny)
{
return BRDF(eyeVec, normalVec, lightVec, dif, spec, shiny);
}

void main()
{

vec4 colEarthBase = texture(earthBaseTexture, texCoord);
vec4 colEarthRed = texture(redEarthTexture, texCoord);



//vec2 uvEarthBase = ?
//vec4 colorEarthBase = texture(earthBaseTexture, uvEarthBase);

//vec2 uvRedEarth = ?
// vec4 colorRedEarth = texture(redEarthTexture, uvRedEarth);

float PI = 3.14159;
//float alpha = pow(8192, shininess);
//alpha.x = pow(8192, shininess.x);
//alpha.y=pow(8192, shininess.y);
//alpha.z=pow(8192, shininess.z);

//
	//vec3 V = normalize(eyeVec);
    vec3 N = normalize(normalVec);
    vec3 L = normalize(lightVec);
	//vec3 H = normalize(L+V);

	float LN = max(dot(L,N), 0.0);
	
	//float HN = max(dot(H,N), 0.0);
	//float LH = max(dot(L,H), 0.0);

	//vec3 mySpec;// = (1-specular.x, 1-specular.y, 1-specular.z); 
	//mySpec.x = 1-specular.x;
	//mySpec.y=1-specular.y;
	//mySpec.z=1-specular.z;
	//vec3 F = specular + (mySpec)*(pow((1-LH), 5));
	//float D = ((alpha+2)/(2*PI))*(pow(HN, alpha)); 
	
	//Using approx of G() / (LN * VN) = approx  1/(LH*LH)

//	vec3 B = (F*D)/(4*LH*LH) + (diffuse / PI);


//vec3 BRDF = BRDF(eyeVec, normalVec, lightVec, diffuse, specular, shininess);
//float LN = LN(lightVec, normalVec);
//vec3 output = B*LN*lightValue;
vec3 t = BRDF(eyeVec, normalVec, lightVec, diffuse, specular, shininess);
vec3 output = t * LN * lightValue;	

float LNReal = dot(L,N);
vec3 white = vec3(1.0f, 1.0f, 1.0f);



	    if (textureSize(groundTexture,0).x>1) // Is the texture defined?
        {
		
		vec3 temp = BRDF(texture(groundTexture,2.0*texCoord.st).xyz, specular, shininess);
		gl_FragColor.xyz = temp;}
		



		else if(textureSize(earthBaseTexture, 0).x >1 )//|| textureSize(redEarthTexture,0).x > 1  )
				{
		
				vec2 xy= vec2(texCoord.x, 1-texCoord.y);
				vec3 baseEarthColor = (texture(earthBaseTexture, xy).xyz);
				vec3 redEarthColor = (texture(redEarthTexture, xy).xyz);
				vec3 daytimeColor=vec3(252.0f/255, 249/255.0f, 149/255.0f);
				//vec3 nighttimeColor=vec3(102/255.0f, 118/255.0f, 186/255.0f);
				vec3 dawnColor = vec3(203/255.0f, 209/255.0f, 134/255.0f);

				vec3 tempOutput = baseEarthColor;

				vec3 incomingLight = lightValue;
				vec3 tempSpecLight = specular;
				vec3 tempDifLight = baseEarthColor;
				float tempShiny = shininess;

				vec3 cityLights = vec3(0.0f, 0.0f, 0.0f);
				

				if(LNReal>= -0.25 && LNReal <= 0.25 )
					{
						//Dawn color
						incomingLight = lightValue + dawnColor;



					}

				else if(LNReal < -0.25)
					{
						//Night color, show city lights via redEarthColor.y
						//Lights would /emanate/ light, so just add that value at the end
						//incomingLight = lightValue+nightColor;
						//cityLights+= vec3(0.35f, 0.3f, 0.1f);



						//if(textureSize(earthBaseTexture,0).x >1 && textureSize(redEarthTexture,0).x >1 && redEarthColor.x ==0)
						//	{
			
								//Is ground at night, apply nightlights

								if(redEarthColor.x ==0)
									{cityLights+= vec3(PI *redEarthColor.y,PI * redEarthColor.y,PI * redEarthColor.y);}
									

						//	}


					
				


					}

				else if(LNReal > 0.25)
					{
						//Day color
						//incomingLight=(0.90*lightValue)+(0.10*daytimeColor);
						//incomingLight = lightValue;
		
						//if(textureSize(earthBaseTexture,0).x >1 && textureSize(redEarthTexture,0).x >1)
						
								 if (redEarthColor.x == 1)
									{



											//Is ocean, up specular reflection
									//tempSpecLight = vec3(0.01f, 0.01f, 0.01f);
									//tempShiny *= 50;
									tempSpecLight = vec3(0.75f, 0.75f, 0.75f);
									//tempDifLight = ((1.25*(1-redEarthColor.y))*tempDifLight)+((1.25*redEarthColor.y)*white);
									
			
									}

									else
									{


									
											//Is land, blend clouds with ground via redEarthColor.z
											//Ground color is a surface / ground property, so diffuse light

											tempDifLight = ((1.25*(1-redEarthColor.z))*tempDifLight)+((1.25*redEarthColor.z)*white);
											
											tempShiny *= 0.1;


										//	gl_FragColor.xyz = (redEarthColor.y*baseEarthColor)+(1-redEarthColor.y)*white;
										//gl_FragColor.xyz = baseEarthColor;






							
									}
				

					}

					tempOutput = (BRDF( tempDifLight, tempSpecLight, tempShiny) * LN * lightValue) + cityLights;

					gl_FragColor.xyz=tempOutput;
					//gl_FragColor.xyz=baseEarthColor;


				//gl_FragColor.xyz = (texture(earthBaseTexture, xy).xyz);
				//gl_FragColor.xyz = vec3(0.0f, 0.0f, 0.0f);
				}
		
		else
		 { 
			gl_FragColor.xyz =output;
		 }
	  
	  
	  //gl_FragColor.xyz = BRDF;
	 // gl_FragColor.xyz=LN*lightValue;

	 //gl_FragColor.xyz=specular;
	// gl_FragColor.xyz = diffuse;


	 


  //  vec3 Kd;

 //   else
  //      Kd = diffuse;

  //  if (direct || mode>0)
       // gl_FragColor.xyz = diffuse;
  //  else
   
	// gl_FragColor.xyz=vec3(1.0, 0.5, 0.0);  //orange
	    // Really *cheap* but effective lighting calculation.
       // gl_FragColor.xyz = max(0.0, dot(L, N))*Kd;0
	   

	   }