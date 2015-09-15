///////////////////////////////////////////////////////////////////////
// A slight encapsulation of a shader program. This contains methods
// to build a shader program from multiples files containing vertex
// and pixel shader code, and a method to link the result.  When
// loaded (method "Use"), its vertex shader and pixel shader will be
// invoked for all geometry passing through the graphics pipeline.
// When done, unload it with method "Unuse".
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////


#include "shader.h"
#include <fstream>
#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <GL/freeglut.h>

// Reads a specified file into a string and returns the string.
char* ReadFile(const char* name)
{
    std::ifstream f;
    f.open(name, std::ios_base::binary); // Open
    f.seekg(0, std::ios_base::end);      // Position at end
    int length = f.tellg();              //   to get the length

    char* content = new char [length+1]; // Create buffer of needed length
    f.seekg (0, std::ios_base::beg);     // Position at beginning
    f.read (content, length);            //   to read complete file
    f.close();                           // Close

    content[length] = char(0);           // Finish with a NULL
    return content;
}

// Asks OpenGL to create an empty shader program.
void ShaderProgram::CreateProgram()
{ 
    program = glCreateProgram();
}

// Use a shader program
void ShaderProgram::Use()
{
    glUseProgram(program);
}

// Done using a shader program
void ShaderProgram::Unuse()
{
    glUseProgram(0);
}

// Read, send to OpenGL, and compile a single file into a shader program.
void ShaderProgram::CreateShader(const char* fileName, int type)
{
    // Read the source from the named file
    char* src = ReadFile(fileName);
    const char* psrc[1] = {src};

    // Create a shader and attach, hand it the source, and compile it.
    int shader = glCreateShader(type);
    glAttachShader(program, shader);
    glShaderSource(shader, 1, psrc, NULL);
    glCompileShader(shader);
    delete src;

    // Get the compilation status
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    // If compilation status is not OK, get and print the log message.
    if (status != 1) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetShaderInfoLog(shader, length, NULL, buffer);
        printf("Compile log for %s:\n%s\n", fileName, buffer);
        delete buffer;
        exit(-1); }
}

void ShaderProgram::LinkProgram()
{
    // Link program and check the status
    glLinkProgram(program);
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    // If link failed, get and print log
    if (status != 1) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetProgramInfoLog(program, length, NULL, buffer);
        printf("Link log:\n%s\n", buffer);
        delete buffer;
        exit(-1); }
}
