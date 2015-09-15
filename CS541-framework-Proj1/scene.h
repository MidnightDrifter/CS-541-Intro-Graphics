////////////////////////////////////////////////////////////////////////
// The scene class contains all the parameters needed to define and
// draw the (really) simple scene, including:
//   * Models (in a VAOs as)
//   * Light parameters
//   * Surface properties
//   * Viewport size parameters
//   * Viewing transformation values
//   * others ...
//
// Some of these parameters are set when the scene is built, and
// others are set by the framework in response to user mouse/keyboard
// interactions.  All of them may be used to draw the scene.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "models.h"
#include "shader.h"
#include "texture.h"

class Scene
{
public:
    // Some user controllable parameters
    int mode;  // Communicated to the shaders as "mode".  Keys '0'-'9'
    int nSpheres;
    bool drawSpheres;
    bool drawGround;

    int centralType;
    int centralModel;
    MAT4 centralTr;

    // Viewing transformation parameters
    // Fixme:  This is a good place to define your viewing paameters, spin, tilt, ...
    

    // Light colors and position parameters;  Mouse buttons SHIFT 1-3
    vec3 ambientColor;
    vec3 lightColor;
    float lightSpin;
    float lightTilt;
    vec3 lightDir;
    float lightDist;
    


    // Viewport
    int width, height;

    // Shader programs
    ShaderProgram lightingShader;

    // The polygon models (VAOs - Vertex Array Objects)
    Model* centralPolygons;
    Model* spherePolygons;
    Model* groundPolygons;

    // Texture
    Texture groundTexture;

    // Main methods
    void InitializeScene();
    void DrawScene();

    // Helper methods
    void SetCentralModel(const int i);
    void DrawSun(unsigned int program, MAT4& ModelTr);
    void DrawSpheres(unsigned int program, MAT4& ModelTr);
    void DrawGround(unsigned int program, MAT4& ModelTr);
};

