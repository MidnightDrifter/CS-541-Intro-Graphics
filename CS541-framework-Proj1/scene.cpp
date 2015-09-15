
//////////////////////////////////////////////////////////////////////
// Defines and draws a scene.  There are two main procedures here:
//
// (1) void Scene::InitializeScene(): is called before the main loop
// is entered, and is expected to setup all OpenGL objects needed for
// the rendering loop.
//
// (2) void Scene::DrawScene(): Is called each time the screen needs
// to be refreshed. (Which will be whenever the screen is
// expose/resized, anytime the preiodic animation clock ticks, or
// anytime there is user mouse/keyboard interaction.
////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <stdlib.h>

#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <GL/freeglut.h>
 
#include "scene.h"

static MAT4 Identity = MAT4();
const float PI = 3.14159f;
const float rad = PI/180.0f;

////////////////////////////////////////////////////////////////////////
// This macro makes it easy to sprinkle checks for OpenGL errors
// throught your code.  Most OpenGL calls can record errors, and a
// careful programmer will check the error status *often*, perhaps as
// often as after each OpenGL call.  At the very least, once per
// refresh will tell you if something is going wrong.
#define CHECKERROR {int err = glGetError(); if (err) { fprintf(stderr, "OpenGL error in scene.cpp at line %d: %s\n", __LINE__, gluErrorString(err)); exit(-1);} }

////////////////////////////////////////////////////////////////////////
// A small function to provide a more friendly method of defining
// colors.  The parameters are hue (0..1: fraction of distance around
// the color wheel; red at 0, green at 1/3, blue at 2/3), saturation
// (0..1: achromatic to saturated), and value (0..1: brightness).
vec3 HSV2RGB(const float h, const float s, const float v)
{
    if (s == 0.0)
        return vec3(v,v,v);

    int i = (int)(h*6.0);
    float f = (h*6.0f) - i;
    float p = v*(1.0f - s);
    float q = v*(1.0f - s*f);
    float t = v*(1.0f - s*(1.0f-f));
    if (i%6 == 0)     return vec3(v,t,p);
    else if (i == 1)  return vec3(q,v,p);
    else if (i == 2)  return vec3(p,v,t);
    else if (i == 3)  return vec3(p,q,v);
    else if (i == 4)  return vec3(t,p,v);
    else if (i == 5)  return vec3(v,p,q);
}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called once during setup to create all the
// textures, model VAOs, render target FBOs, and shader programs as
// well as a number of other parameters.
void Scene::InitializeScene()
{
    CHECKERROR;

    //////////////////////////////////////////////////////////////////////
    // Initialize various scene parameters.

    // Scene creation parameters
    mode = 0;
    nSpheres = 16;
    drawSpheres = true;
    drawGround = true;

    // Scene transformation parameters
    // Fixme:  This is a good place to initialize your scene variables.

    // Light colors and position parammeters
    ambientColor = vec3(0.5f, 0.5f, 0.5f);
    lightColor = vec3(0.8f, 0.8f, 0.8f);
    lightSpin = -90.0f;
    lightTilt = -60.0f;
    lightDist = 60.0f;

    // Enable OpenGL depth-testing
    glEnable(GL_DEPTH_TEST);

    //////////////////////////////////////////////////////////////////////
    // Create the models which will compose the scene.  These are all
    // built (in models.cpp) as Vertex Array Objects (VAO's) and sent
    // to the graphics card.
    spherePolygons = new Sphere(32);
    groundPolygons = new Ground(50.0, 100);
    SetCentralModel(0);         // Teapot, sphere, or some PLY model, or ...

    //////////////////////////////////////////////////////////////////////
    // Create a shader program  (See shader.h and shader.cpp.)
    
    // Create the program
    lightingShader.CreateProgram();

    // Read and compile the source from two files.
    lightingShader.CreateShader("lighting.vert", GL_VERTEX_SHADER);
    lightingShader.CreateShader("lighting.frag", GL_FRAGMENT_SHADER);

    // Chooses specific locations for the four vertex attributes.
    // These names must match the "in" variables of the vertex shader,
    // and the location values 0..3 must match the locations specified
    // via glVertexAttribPointer in models.cpp.
    glBindAttribLocation(lightingShader.program, 0, "vertex");
    glBindAttribLocation(lightingShader.program, 1, "vertexNormal");
    glBindAttribLocation(lightingShader.program, 2, "vertexTexture");
    glBindAttribLocation(lightingShader.program, 3, "vertexTangent");

    // Link the shader (checking for errors and aborting if necessary).
    lightingShader.LinkProgram();


    //////////////////////////////////////////////////////////////////////
    // Read a texture and store its id in groundTexture.  Abort
    // program on error.
    groundTexture.Read("images/6670-diffuse.jpg");
    CHECKERROR;
}


void Scene::SetCentralModel(const int i)
{
    delete centralPolygons;
    centralPolygons = NULL;
    centralModel = i;

    if (centralModel==0) {
        centralPolygons =  new Teapot(12);    
        float s = 3.0/centralPolygons->size;
        centralTr =
            Scale(s,s,s)*
            Translate(-centralPolygons->center); }

    else if (centralModel==1) {
        centralPolygons = new Ply("bunny.ply");
        float s = 3.0/centralPolygons->size;
        centralTr =
            Rotate(2, 180.0f)
            *Rotate(0, 90.0f)
            *Scale(s,s,s)
            *Translate(-centralPolygons->center); }

    else if (centralModel==2) {
        centralPolygons = new Ply("dragon.ply");
        float s = 3.0/centralPolygons->size;
        centralTr =
            Rotate(2, 180.0f)
            *Rotate(0, 90.0f)
            *Scale(s,s,s)
            *Translate(-centralPolygons->center); }

    else {       // Fallback model
        centralPolygons = new Sphere(32);
        float s = 3.0/centralPolygons->size;
        centralTr = Scale(s,s,s); }
}

////////////////////////////////////////////////////////////////////////
// A small helper function to draw a model after settings its lighting
// and modeling parmaeters.
void DrawModel(const int program, Model* m, MAT4& ModelTr)
{
    int loc = glGetUniformLocation(program, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, ModelTr.Pntr());

    loc = glGetUniformLocation(program, "NormalMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, ModelTr.inverse().Pntr());

    loc = glGetUniformLocation(program, "diffuse");
    glUniform3fv(loc, 1, &m->diffuseColor[0]);

    loc = glGetUniformLocation(program, "specular");
    glUniform3fv(loc, 1, &m->specularColor[0]);

    loc = glGetUniformLocation(program, "shininess");
    glUniform1f(loc, m->shininess);

    m->DrawVAO();
}

////////////////////////////////////////////////////////////////////////
// A small helper function for DrawScene to draw all the environment
// spheres.
void Scene::DrawSpheres(unsigned int program, MAT4& ModelTr)
{
    CHECKERROR;
    float t = 1.0;
    float s = 200.0;
    

    int loc;

    loc = glGetUniformLocation(program, "specular");
    glUniform3fv(loc, 1, &spherePolygons->specularColor[0]);

    loc = glGetUniformLocation(program, "shininess");
    glUniform1f(loc, spherePolygons->shininess);

    for (int i=0;  i<2*nSpheres;  i+=2) {
        float u = float(i)/(2*nSpheres);

        for (int j=2;  j<=nSpheres/2;  j+=2) {
            float v = float(j)/(nSpheres);
            vec3 color = HSV2RGB(u, 1.0f-2.0f*fabs(v-0.5f), 1.0f);

            float s = 3.0f* sin(v*3.14f);
            MAT4 M = ModelTr*Rotate(2, 360.0f*u)*Rotate(1, 180.0f*v)
                     *Translate(0.0f, 0.0f, 30.0f)*Scale(s,s,s) ;
            loc = glGetUniformLocation(program, "ModelMatrix");
            glUniformMatrix4fv(loc, 1, GL_TRUE, M.Pntr());
            
            loc = glGetUniformLocation(program, "NormalMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, M.inverse().Pntr());

            loc = glGetUniformLocation(program, "diffuse");
            glUniform3fv(loc, 1, &color[0]);
            spherePolygons->DrawVAO(); } }
    
    loc = glGetUniformLocation(program, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, Identity.Pntr());
    loc = glGetUniformLocation(program, "NormalMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, Identity.Pntr());
    CHECKERROR;
}

void Scene::DrawGround(unsigned int program, MAT4& ModelTr)
{
    int loc = glGetUniformLocation(program, "diffuse");
    glUniform3fv(loc, 1, &groundPolygons->diffuseColor[0]);

    loc = glGetUniformLocation(program, "specular");
    glUniform3fv(loc, 1, &groundPolygons->specularColor[0]);

    loc = glGetUniformLocation(program, "shininess");
    glUniform1f(loc, groundPolygons->shininess);

    groundTexture.Bind(1);      // Choose texture unit 1
    loc = glGetUniformLocation(program, "groundTexture");
    glUniform1i(loc, 1);        // Tell the shader about unit 1

    loc = glGetUniformLocation(program, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, ModelTr.Pntr());
    loc = glGetUniformLocation(program, "NormalMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, ModelTr.inverse().Pntr());

    groundPolygons->DrawVAO();
    CHECKERROR;

    glActiveTexture(GL_TEXTURE1); // Choose texture unit 1
    groundTexture.Unbind();

}

void Scene::DrawSun(unsigned int program, MAT4& ModelTr)
{
    vec3 white(100,1,1);
    
    int loc = glGetUniformLocation(program, "direct");
    glUniform1i(loc, 1);

    loc = glGetUniformLocation(program, "diffuse");
    glUniform3fv(loc, 1, &white[0]);

    loc = glGetUniformLocation(program, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, ModelTr.Pntr());

    spherePolygons->DrawVAO();
    CHECKERROR;

    loc = glGetUniformLocation(program, "direct");
    glUniform1i(loc, 0);

}

////////////////////////////////////////////////////////////////////////
// Called regularly to update the rotation of the surrounding sphere
// environment.  Set to rotate once every two minutes.
float atime = 0.0;
void animate(int value)
{
    atime = 360.0f*glutGet(GLUT_ELAPSED_TIME)/120000.0f;
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void Scene::DrawScene()
{
    CHECKERROR;

    int loc;

    // Calculate the light's position.
    vec3 lPos = vec3(lightDist*cos(lightSpin*rad)*sin(lightTilt*rad),
                     lightDist*sin(lightSpin*rad)*sin(lightTilt*rad),
                     lightDist*cos(lightTilt*rad) );

    // FIXME:  These hard-coded perspective and viewing matrices should be removed and replaced.
    MAT4 WorldView, WorldProj;
    
    WorldProj[0][0] =   -4.3301;
    WorldProj[0][1] =    2.5000;
    WorldProj[0][2] =    0.0000;
    WorldProj[0][3] =    0.0000;
    WorldProj[1][0] =   -1.2500;
    WorldProj[1][1] =   -2.1651;
    WorldProj[1][2] =    4.3301;
    WorldProj[1][3] =   -5.0000;
    WorldProj[2][0] =   -0.4330;
    WorldProj[2][1] =   -0.7500;
    WorldProj[2][2] =   -0.5000;
    WorldProj[2][3] =  159.8032;
    WorldProj[3][0] =   -0.4330;
    WorldProj[3][1] =   -0.7500;
    WorldProj[3][2] =   -0.5000;
    WorldProj[3][3] =  160.0000;


    ///////////////////////////////////////////////////////////////////
    // Lighting pass: Draw the scene with lighting being calculated in
    // the lighting shader.
    ///////////////////////////////////////////////////////////////////

    // Set the viewport, and clear the screen
    glViewport(0,0,width, height);
    glClearColor(0.5,0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // Use lighting pass shader
    lightingShader.Use();
    int program = lightingShader.program;

    // Send the screen height and width to the shader
    loc = glGetUniformLocation(program, "WIDTH");
    glUniform1i(loc, width);
    loc = glGetUniformLocation(program, "HEIGHT");
    glUniform1i(loc, height);

    // Send the perspective and viewing matrices to the shader
    loc = glGetUniformLocation(program, "ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldProj.Pntr());
    loc = glGetUniformLocation(program, "ViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldView.Pntr());
    loc = glGetUniformLocation(program, "ViewInverse");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldView.inverse().Pntr());
    CHECKERROR;

    // Send the initial model matrix and normal matrix to the shader
    loc = glGetUniformLocation(program, "ModelMatrix");
    glUniformMatrix4fv(loc, 1, GL_TRUE, Identity.Pntr());
    loc = glGetUniformLocation(program, "NormalMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, Identity.Pntr());
    CHECKERROR;

    // Send lighting parameters to the shader
    loc = glGetUniformLocation(program, "lightAmbient");
    glUniform3fv(loc, 1, &ambientColor[0]);
    loc = glGetUniformLocation(program, "lightPos");
    glUniform3fv(loc, 1, &lPos[0]);
    loc = glGetUniformLocation(program, "lightValue");
    glUniform3fv(loc, 1, &lightColor[0]);

    // Send mode to the shader (used to choose alternate shading
    // strategies in the shader)
    loc = glGetUniformLocation(program, "mode");
    glUniform1i(loc, mode);

    // Draw the scene objects.
    MAT4 SphereModelTr = Rotate(2, atime);
    MAT4 SunModelTr = Translate(lPos);
    DrawSun(program, SunModelTr);
    if (drawSpheres) DrawSpheres(program, SphereModelTr);
    if (drawGround) DrawGround(program, Identity);
    DrawModel(program, centralPolygons, centralTr);
    CHECKERROR;

    // Done with shader program
    lightingShader.Unuse();
    CHECKERROR;

    // After all drawing, schedule a call to the animate procedure in 10 ms.
    glutTimerFunc(10, animate, 1);

}
