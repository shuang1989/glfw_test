#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cfloat>

#include "vec.h"
#include "util.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

using namespace std;
unsigned int frame= 0;
unsigned int highest_frame = 10000;
string file_path;
vector<Vec2f> particles;
float particle_radius;
string frame_number="frame 0";

void drawCircle2d( GLfloat x, GLfloat y, GLfloat radius, GLint numberOfSides );
bool read_frame(int newframe)；

int main( int argc, char **argv )
{
    if (argc != 2) {
        cerr << "The first parameter should be thefoder to read the input liquid particles files. (eg. /Users/data)" << endl;
        return -1;
    }
    file_path=argv[1];
    
    
    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Liquid Data Viewer", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        
        glClear( GL_COLOR_BUFFER_BIT );
        
        // render OpenGL here
        
        drawCircle2d( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,  2, 20);
        
        // Swap front and back buffers
        glfwSwapBuffers( window );
        
        // Poll for and process events
        glfwPollEvents( );
    }
    
    glfwTerminate( );
    
    return 0;
}

void drawCircle2d( GLfloat x, GLfloat y, GLfloat radius, GLint numberOfSides )
{
    int numberOfVertices = numberOfSides + 2;
    
    GLfloat twicePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    
    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    
    for ( int i = 1; i < numberOfVertices; i++ )
    {
        circleVerticesX[i] = x + ( radius * cos( i *  twicePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * twicePi / numberOfSides ) );
    }
    
    GLfloat allCircleVertices[( numberOfVertices ) * 2];
    GLfloat colors[( numberOfVertices ) * 3 ];
    for ( int i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 2] = circleVerticesX[i];
        allCircleVertices[( i * 2 ) + 1] = circleVerticesY[i];
        colors[i * 3] = 0;
        colors[i * 3 + 1] = 0;
        colors[i * 3 + 2] = 255;
    }
    
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer( 2, GL_FLOAT, 0, allCircleVertices );
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays( GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState(GL_COLOR_ARRAY);
}

bool read_frame(int newframe){
    if(newframe<0) newframe = highest_frame;
    std::ostringstream strout;
    strout << file_path << "particles_" << newframe << ".txt";
    printf("File path %s\n", strout.str().c_str());
    std::ifstream particles_in(strout.str().c_str());
    if(!particles_in.good()) {
        printf("Failed to open particles!\n");
        return false;
    }
    unsigned int p_count;
    particles_in >> p_count >> particle_radius;
    particles.resize(p_count);
    for(unsigned int p = 0; p < p_count; ++p) {
        Vec2f pos;
        particles_in >> pos[0] >> pos[1] >> pos[2];
        particles[p] = pos;
    }
    printf("Particle count: %d\n", p_count);
    printf("Particle radius: %f\n", particle_radius);
    
    if(newframe > (int)highest_frame)
        highest_frame = newframe;
    
    strout.str("");
    
    frame=newframe;
    
    strout.str("");
    strout << "frame " << frame;
    frame_number = strout.str();
    
    return true;
}
