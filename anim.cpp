////////////////////////////////////////////////////
// anim.cpp version 4.1
// Template code for drawing an articulated figure.
// CS 174A 
////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GLUT/glut.h>
#endif

#include "Ball.h"
#include "FrameSaver.h"
#include "Timer.h"
#include "Shapes.h"
#include "tga.h"

#include "Angel/Angel.h"

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif

FrameSaver FrSaver ;
Timer TM ;

BallData *Arcball = NULL ;
int Width = 800;
int Height = 800 ;
int Button = -1 ;
float Zoom = 1 ;
int PrevY = 0 ;

int Animate = 0 ;
int Recording = 0 ;

void resetArcball() ;
void save_image();
void instructions();
void set_colour(float r, float g, float b) ;

const int STRLEN = 100;
typedef char STR[STRLEN];

#define PI 3.1415926535897
#define X 0
#define Y 1
#define Z 2

float t = 0;
float headr = 0;

//texture

GLuint texture_cube;
GLuint texture_earth;
GLuint texture_roof;
GLuint texture_sky;
GLuint texture_trees;
GLuint texture_leaf;
GLuint texture_stone;

// Structs that hold the Vertex Array Object index and number of vertices of each shape.
ShapeData cubeData;
ShapeData sphereData;
ShapeData coneData;
ShapeData cylData;
ShapeData pyramidData;

// Matrix stack that can be used to push and pop the modelview matrix.
class MatrixStack {
    int    _index;
    int    _size;
    mat4*  _matrices;

   public:
    MatrixStack( int numMatrices = 32 ):_index(0), _size(numMatrices)
        { _matrices = new mat4[numMatrices]; }

    ~MatrixStack()
	{ delete[]_matrices; }

    void push( const mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
    }

    mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

MatrixStack  mvstack;
mat4         model_view;
GLint        uModelView, uProjection, uView;
GLint        uAmbient, uDiffuse, uSpecular, uLightPos, uShininess;
GLint        uTex, uEnableTex;

// The eye point and look-at point.
// Currently unused. Use to control a camera with LookAt().
Angel::vec4 eye(0, 0.0, 50.0,1.0);
Angel::vec4 ref(0.0, 0.0, 0.0,1.0);
Angel::vec4 up(0.0,1.0,0.0,0.0);

double TIME = 0.0 ;

/////////////////////////////////////////////////////
//    PROC: drawCylinder()
//    DOES: this function 
//          render a solid cylinder  oriented along the Z axis. Both bases are of radius 1. 
//          The bases of the cylinder are placed at Z = 0, and at Z = 1.
//
//          
// Don't change.
//////////////////////////////////////////////////////
void drawCylinder(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
}

void drawTrunk(void)
{
    glBindTexture( GL_TEXTURE_2D, texture_trees );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

//////////////////////////////////////////////////////
//    PROC: drawCone()
//    DOES: this function 
//          render a solid cone oriented along the Z axis with base radius 1. 
//          The base of the cone is placed at Z = 0, and the top at Z = 1. 
//         
// Don't change.
//////////////////////////////////////////////////////
void drawCone(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( coneData.vao );
    glDrawArrays( GL_TRIANGLES, 0, coneData.numVertices );
}




//////////////////////////////////////////////////////
//    PROC: drawCube()
//    DOES: this function draws a cube with dimensions 1,1,1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////

void drawCube(void)
{
    glBindTexture( GL_TEXTURE_2D, texture_cube );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawOldCube(void)
{
//    glBindTexture( GL_TEXTURE_2D, texture_cube );
 //   glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
 //   glUniform1i( uEnableTex, 0 );
}

void drawSky(void)
{
    glBindTexture( GL_TEXTURE_2D, texture_sky );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}




//////////////////////////////////////////////////////
//    PROC: drawSphere()
//    DOES: this function draws a sphere with radius 1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////

void drawSphere(void)
{
 //   glBindTexture( GL_TEXTURE_2D, texture_earth);
 //   glUniform1i( uEnableTex, 1);
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
    //glUniform1i( uEnableTex, 0 );
}


void drawPyramid(void)
{
	glBindTexture( GL_TEXTURE_2D, texture_roof );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( pyramidData.vao );
    glDrawArrays( GL_TRIANGLES, 0, pyramidData.numVertices );
	  glUniform1i( uEnableTex, 0 );
}

void drawLeaf(void)
{
	glBindTexture( GL_TEXTURE_2D, texture_leaf );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( pyramidData.vao );
    glDrawArrays( GL_TRIANGLES, 0, pyramidData.numVertices );
	  glUniform1i( uEnableTex, 0 );
}

void drawStone(void)
{
	glBindTexture( GL_TEXTURE_2D, texture_stone );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( pyramidData.vao );
    glDrawArrays( GL_TRIANGLES, 0, pyramidData.numVertices );
	  glUniform1i( uEnableTex, 0 );
}


void resetArcball()
{
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}


//////////////////////////////////////////////////////
//    PROC: myKey()
//    DOES: this function gets caled for any keypresses
// 
//////////////////////////////////////////////////////

void myKey(unsigned char key, int x, int y)
{
    float time ;
    switch (key) {
        case 'q':
        case 27:
            exit(0); 
        case 's':
            FrSaver.DumpPPM(Width,Height) ;
            break;
        case 'r':
            resetArcball() ;
            glutPostRedisplay() ;
            break ;
        case 'a': // togle animation
            Animate = 1 - Animate ;
            // reset the timer to point to the current time		
            time = TM.GetElapsedTime() ;
            TM.Reset() ;
            // printf("Elapsed time %f\n", time) ;
            break ;
        case '0':
            //reset your object
            break ;
        case 'm':
            if( Recording == 1 )
            {
                printf("Frame recording disabled.\n") ;
                Recording = 0 ;
            }
            else
            {
                printf("Frame recording enabled.\n") ;
                Recording = 1  ;
            }
            FrSaver.Toggle(Width);
            break ;
        case 'h':
        case '?':
            instructions();
            break;
    }
    glutPostRedisplay() ;

}

/*********************************************************
    PROC: myinit()
    DOES: performs most of the OpenGL intialization
     -- change these with care, if you must.

**********************************************************/

void myinit(void)
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram(program);

    // Generate vertex arrays for geometric shapes
    generateCube(program, &cubeData);
    generateSphere(program, &sphereData);
    generateCone(program, &coneData);
    generateCylinder(program, &cylData);
	generatePyramid(program, &pyramidData);

    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView       = glGetUniformLocation( program, "View"       );

    glClearColor( .1, .1, .6, 1.0 ); // dark blue background

    uAmbient   = glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse   = glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular  = glGetUniformLocation( program, "SpecularProduct" );
    uLightPos  = glGetUniformLocation( program, "LightPosition"   );
    uShininess = glGetUniformLocation( program, "Shininess"       );
    uTex       = glGetUniformLocation( program, "Tex"             );
    uEnableTex = glGetUniformLocation( program, "EnableTex"       );

    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);

    glEnable(GL_DEPTH_TEST);

    TgaImage coolImage;
    if (!coolImage.loadTGA("dirt.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    TgaImage earthImage;
    if (!earthImage.loadTGA("earth.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

	 TgaImage roofImage;
    if (!roofImage.loadTGA("roof.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

	TgaImage treeImage;
    if (!treeImage.loadTGA("tree.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

	TgaImage skyImage;
    if (!skyImage.loadTGA("sky.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

	TgaImage stoneImage;
    if (!stoneImage.loadTGA("stone.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

	TgaImage leafImage;
    if (!leafImage.loadTGA("leaf.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    glGenTextures( 1, &texture_cube );	
    glBindTexture( GL_TEXTURE_2D, texture_cube );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, coolImage.width, coolImage.height, 0,
                 (coolImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, coolImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	
    
    
    glGenTextures( 1, &texture_earth );
    glBindTexture( GL_TEXTURE_2D, texture_earth );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, earthImage.width, earthImage.height, 0,
                 (earthImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, earthImage.data );
    
	//roof
	 glGenTextures( 1, &texture_roof );
    glBindTexture( GL_TEXTURE_2D, texture_roof );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	 glTexImage2D(GL_TEXTURE_2D, 0, 4, roofImage.width, roofImage.height, 0,
            (roofImage.byteCount == 3) ? GL_BGR : GL_BGRA,
             GL_UNSIGNED_BYTE, roofImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		 glGenTextures( 1, &texture_trees );
    glBindTexture( GL_TEXTURE_2D, texture_trees );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	 glTexImage2D(GL_TEXTURE_2D, 0, 4, treeImage.width, treeImage.height, 0,
            (treeImage.byteCount == 3) ? GL_BGR : GL_BGRA,
             GL_UNSIGNED_BYTE, treeImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glGenTextures( 1, &texture_leaf );
    glBindTexture( GL_TEXTURE_2D, texture_leaf );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	 glTexImage2D(GL_TEXTURE_2D, 0, 4, leafImage.width, leafImage.height, 0,
            (leafImage.byteCount == 3) ? GL_BGR : GL_BGRA,
             GL_UNSIGNED_BYTE, leafImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glGenTextures( 1, &texture_stone );
    glBindTexture( GL_TEXTURE_2D, texture_stone );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	 glTexImage2D(GL_TEXTURE_2D, 0, 4, stoneImage.width, stoneImage.height, 0,
            (stoneImage.byteCount == 3) ? GL_BGR : GL_BGRA,
             GL_UNSIGNED_BYTE, stoneImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    
    // Set texture sampler variable to texture unit 0
    // (set in glActiveTexture(GL_TEXTURE0))
    
    glUniform1i( uTex, 0);
    
    Arcball = new BallData;
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}

/*********************************************************
    PROC: set_colour();
    DOES: sets all material properties to the given colour
    -- don't change
**********************************************************/

void set_colour(float r, float g, float b)
{
    float ambient  = 0.2f;
    float diffuse  = 0.6f;
    float specular = 0.2f;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1.0f);
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1.0f);
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1.0f);
}

void drawWood(void)
{
	mvstack.push(model_view);
	set_colour(0.5f, 0.35f, 0.0f);
	model_view *= Scale(16.0f, 1.5f, 0.15f);
	drawCube();
	model_view = mvstack.pop();
}

void drawHandle(void)
{
	mvstack.push(model_view);
	set_colour(0.2, 0.2, 0.2);
	model_view *= Scale(0.5, 3.0f, 0.25f);
	drawCube();
	model_view = mvstack.pop();
}

void drawDesert(void)
{
	mvstack.push(model_view);
	set_colour(.9, .8, .6);
	model_view *= Scale(500, 1, 500);
	drawCube();
	model_view = mvstack.pop();
}



void drawBench(void)
{
	mvstack.push(model_view);
	drawWood();
	model_view *= Translate (0, -2.5 ,0);
	drawWood();

	model_view *= Translate (0, -1, 1);
	model_view *= RotateX(90);
	model_view *= Scale(1, 0.25, 2);
	drawWood();
	model_view *= Translate(0, 2, 0);
	drawWood();
	model_view *= Translate(0, 2, 0);
	drawWood();
	model_view *= Translate(0, 2, 0);
	drawWood();
	model_view *= Translate(0, 2, 0);
	drawWood();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	set_colour(.2,.2,.2);
	model_view *= Translate (-6.5 , -.7 , 0.25);
	drawHandle();
	model_view *= Translate (0, -.7, 1.25);
	model_view *= RotateX(90);
	drawHandle();
	model_view *= Scale (1, 1, 1.5);
	model_view *= Translate (0, 1.3, 1.5);
	model_view *= RotateX(90);
	drawHandle();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate(6.5, -.7, 0.25);
	drawHandle();
	model_view *= Translate (0, -.7, 1.25);
	model_view *= RotateX(90);
	drawHandle();
	model_view *= Scale (1, 1, 1.5);
	model_view *= Translate (0, 1.3, 1.5);
	model_view *= RotateX(90);
	drawHandle();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate (6.5, -4.5, 1);
	drawHandle();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate (-6.5, -4.5, 1);
	drawHandle();
	model_view = mvstack.pop();
}

void drawGreen(void)
{
	mvstack.push(model_view);
	set_colour(.1, .4, .1);
	model_view *= Scale (10, 7, 12);
	model_view *= RotateX(-90);
	drawLeaf();
	model_view = mvstack.pop();
}

void drawTree(void)
{
	mvstack.push(model_view);
	set_colour(.4, .2, .1);
	model_view *= Scale (3, 8, 3);
	model_view *= RotateX(90);
	drawTrunk();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate(0, 5, 0);
	model_view *= RotateX(90);
	drawGreen();
	model_view = mvstack.pop();
}


void drawBanana1(void)
{
	mvstack.push(model_view);
	set_colour(.95, .9, .8);
	model_view *= Scale(0.08, 0.2, 0.1);
	model_view *= RotateX(90);
	drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	set_colour(.95, .9, .1);
	model_view *= Scale(0.1, 0.3, 0.112);
	model_view *= Translate (0, -.5, 0);
	model_view *= RotateX(90);
	drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	set_colour (.1, .1, .2);
	model_view *= Translate(0, -0.45, 0);
	model_view *= Scale(.025, .025,.025);
	drawSphere();
	model_view = mvstack.pop();


}

void drawHbase(void)
{
	mvstack.push(model_view);
	set_colour(.55, .5, .45);
	model_view *= Scale(20, 20, 20);
	drawCube();
	model_view = mvstack.pop();
}

void drawRoof(void)
{
	mvstack.push(model_view);
	set_colour(.92, .5, .25);
	model_view *= Scale(20, 20, 20);
	drawPyramid();
	model_view = mvstack.pop();
}

void drawHouse(void)
{
	mvstack.push(model_view);
	drawHbase();
	model_view *= Translate(0, 20, 0);
	drawRoof();
	model_view = mvstack.pop();
	
	mvstack.push(model_view);
	set_colour(.84, 0, .05);
	model_view *= Translate(0, -5, 10);
	model_view *= Scale(5, 10, 0.5);
	drawCube();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	set_colour(.8, .8, 0);
	model_view *= Translate(2, -5, 10);
	model_view *= Scale (0.5, 0.5, 0.5);
	drawSphere();
	model_view = mvstack.pop();
}

void drawHead1(void)
{
		//head
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		drawSphere();
	model_view = mvstack.pop();

	//eyes
	mvstack.push(model_view);
		set_colour(.1,.1,.1);
		model_view *= Translate(-.27, .3, .7);
		model_view *= Scale(.2, .2, .2);
		drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour(.1,.1,.1);
		model_view *= Translate(.27, .3, .7);
		model_view *= Scale(.2, .2, .2);
		drawSphere();
	model_view = mvstack.pop();

	
}

void drawTorso1(void)
{
	//torso
	mvstack.push(model_view);
		set_colour(.1, .1, .1);
		model_view *= Translate (0, -1.5, 0);
		model_view *= Scale(1, 2, 1);
		drawCube();
	model_view = mvstack.pop();
}
void drawBlade(float r, float g, float b)
{
	mvstack.push(model_view);
		set_colour(r,g,b);
		model_view *= Scale(0.5, 9, 0.5);
		drawSphere();
		model_view = mvstack.pop();
}
void drawSaber(float r, float g, float b)
{
	mvstack.push(model_view);
		set_colour(.3, .3, .4);
		model_view *= Scale(0.5, 2, 0.5);
		model_view *= RotateX(90);
		drawCylinder();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour(.9, 0, 0);
		model_view *= Translate(0,.4,.5);
		model_view *= Scale(.15, .15, .15);
		drawCylinder();
	model_view = mvstack.pop();
	
	if (TIME >= 10)
	{
		mvstack.push(model_view);
			model_view *=  Translate(0, 7, 0);
			drawBlade(r, g, b);
		model_view = mvstack.pop();
	}
}

void drawTopArm1Left(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (-.75, -1.25, 0);
		model_view *= Scale(.25, .5, .25);
		drawSphere();
	model_view = mvstack.pop();
}

void drawBotArm1(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (-.75, -2, 0);
		// hand cut off
		if (TIME >= 21 && TIME < 22)
			model_view *= Translate (0, 21 - TIME, 0);
		else if (TIME >=22)
			model_view *= Translate (0, -1, 0);
		model_view *= Scale(.25, .3, .25);
		drawSphere();
	model_view = mvstack.pop();

	if(TIME < 5)
	{
		mvstack.push(model_view);
			model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale (1.2, 1.5, 1.2);
			model_view *= RotateX(90);
			drawBanana1();
		model_view = mvstack.pop();
	}
	else if (TIME >= 8 && TIME < 17)
	{
		mvstack.push(model_view);
		
			model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale(.2, .2, .2);
			model_view *= RotateX(90);
			model_view *= RotateY(180);
			drawSaber(0, 1, 0);
		model_view = mvstack.pop();
	}
	else if (TIME >= 17 && TIME < 19)
	{
		mvstack.push(model_view);
				model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale(.2, .2, .2);
			model_view *= RotateX(90);
			model_view *= RotateY(180);
			model_view *= Translate(15, -(17*12)+TIME*12, 0);
			model_view *= RotateY((17*45)-TIME*45);
			model_view *= RotateX(-6120 + TIME*360);
			drawSaber(0, 1, 0);
		model_view = mvstack.pop();
	}
	else if (TIME >= 19 && TIME < 21)
	{
		mvstack.push(model_view);
			model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale(.2, .2, .2);
			model_view *= RotateX(90);
			model_view *= RotateY(180);
			model_view *= Translate(15, 24, 0);
			model_view *= RotateY(90);
			model_view *= RotateX(720);
			model_view *= Translate(19 - TIME, 266 - TIME*14, 0);
			model_view *= RotateX(4275 - TIME*225);
			drawSaber(0,1,0);
		model_view = mvstack.pop();
	}
	else if (TIME >= 21)
	{
		mvstack.push(model_view);
			model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale(.2, .2, .2);
			model_view *= RotateX(90);
			model_view *= RotateY(180);
			model_view *= Translate(15, 24, 0);
			model_view *= RotateY(90);
			model_view *= RotateX(720);
			model_view *= Translate(-2, -28, 0);
			model_view *= RotateX(-450);
			drawSaber(0, 1,0);
		model_view =mvstack.pop();
	}

			
}

void drawArm1(void)
{
	mvstack.push(model_view);
		drawTopArm1Left();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		drawBotArm1();
	model_view = mvstack.pop();
}


void drawTopArm1Right(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (.75, -1.25, 0);
		model_view *= Scale(.25, .5, .25);
		drawSphere();
	model_view = mvstack.pop();
}

void drawLegPart1(void)
{
	mvstack.push(model_view);
		set_colour(.1, .2, .75);
		model_view *= Scale(.47, 1.5, .47);
		drawOldCube();
	model_view = mvstack.pop();

}

void drawLegPart2(void)
{	
	mvstack.push(model_view);
		set_colour(.1, .1, .75);
		model_view *= Scale(.47, 1.5, .47);
		drawOldCube();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour (.1, .1, .1);
		model_view *= Scale (.5, .2 ,.7);
		model_view *= Translate(0, -3.4, .1);
		drawOldCube();
	model_view = mvstack.pop();
}


void drawGuy1(void)
{
	mvstack.push(model_view);
	if (TIME >= 3 && TIME < 4)
	{
		model_view *= RotateY(-135 + TIME*45);
		drawHead1();
	}
	else if(TIME >=4 && TIME < 5)
	{
		model_view *= RotateY(45);
		model_view *= RotateY(180 - TIME*45);
		drawHead1();
	}
	else
		drawHead1();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		drawTorso1();
	model_view = mvstack.pop();
	
	// banana arm
	mvstack.push(model_view);
		if (TIME < 3)
		{
			model_view *= RotateX(-(17*3)+TIME*17);
		}
		else if (TIME >= 3 && TIME < 4)
		{
			model_view *= RotateX(17);
			model_view *= RotateX(36 - TIME*12);
		}
		else if( TIME >= 4 && TIME < 10)
		{
			model_view *= RotateX(20*sin(3*TIME));
		}
		else if (TIME >= 10 && TIME < 10.5)
		{
			model_view *= RotateX(959 - TIME*102);
		}
		else if (TIME >= 10.5 && TIME < 16)
		{
			model_view *= RotateY(30*sin(5*TIME));
			model_view *= RotateX(-45);
		}
		else if (TIME >= 17 && TIME < 17.5)
		{
			model_view *= RotateX(80*sin(20*TIME));
		}
			drawArm1();
	model_view = mvstack.pop();

	//static arm
	mvstack.push(model_view);
	if( TIME >= 4 && TIME < 10)
		{
			model_view *= RotateX(-20*sin(3*TIME));
		}
		drawTopArm1Right();
	model_view = mvstack.pop();

	//legs

	//LEG1
	mvstack.push(model_view);
	if (TIME > 5 && TIME < 9)
	{
		model_view *= RotateX(5*sin(5*TIME));
	}
	else if (TIME >= 16.5 && TIME < 17)
	{
		model_view *= RotateX(5*sin(5*TIME));
	}
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(-.25, -2.5, 0);
			model_view *= RotateX(180);
			//model_view *= RotateX(5+sin(2*TIME));
			drawLegPart1();
			model_view *= Translate(0, .9, 0);
			model_view *= Scale (.2, .2, .2);
			drawSphere();
		model_view = mvstack.pop();

		//LOWERLEG 1
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(-.25, -4.32, 0);
			if (TIME > 5 && TIME < 9)
				model_view *= RotateX(5*sin(5*TIME));
			else if (TIME >= 16.5 && TIME < 17)
			{
				model_view *= RotateX(5*sin(5*TIME));
			}
			drawLegPart2();
		model_view = mvstack.pop();
	model_view = mvstack.pop();

	//LEG2	
	mvstack.push(model_view);
	if (TIME > 5 && TIME < 9)
	{
		model_view *= RotateX(-5*sin(5*TIME));
	}
	else if (TIME >= 16.5 && TIME < 17)
	{
		model_view *= RotateX(-5*sin(5*TIME));
	}
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(.25, -2.5, 0);
			model_view *= RotateX(180);
			drawLegPart1();
			model_view *= Translate(0, .9, 0);
			model_view *= Scale (.2, .2, .2);
			drawSphere();
		model_view = mvstack.pop();
		//LOWER LEG  2
		mvstack.push(model_view);
				set_colour(.8, .8, .8);
				model_view *= Translate(.25, -4.32, 0);
				if (TIME > 5 && TIME < 9)
					model_view *= RotateX(-5*sin(5*TIME));
				else if (TIME >= 16.5 && TIME < 17)
				{
					model_view *= RotateX(-5*sin(5*TIME));
				}
				drawLegPart2();
		model_view = mvstack.pop();

	model_view = mvstack.pop();
}


// GUY 2

void drawHead2(void)
{
		//head
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		drawSphere();
	model_view = mvstack.pop();
	//hat
	mvstack.push(model_view);
		set_colour(.1, .1, .1);
		model_view *= Translate (0, 1.5, 0);
		model_view *= RotateX(90);
		drawCone();
	model_view = mvstack.pop();
	//stache
	mvstack.push(model_view);
		set_colour(.1,.1,.1);
		model_view *= Translate (0, -.25, 1);
		model_view *= Scale(.2, .1, .05);
		drawCube();
	model_view = mvstack.pop();

	//eyes
	mvstack.push(model_view);
		set_colour(.1,.1,.1);
		model_view *= Translate(-.27, .3, .7);
		model_view *= Scale(.2, .2, .2);
		drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour(.1,.1,.1);
		model_view *= Translate(.27, .3, .7);
		model_view *= Scale(.2, .2, .2);
		drawSphere();
	model_view = mvstack.pop();

	
}

void drawTorso2(void)
{
	//torso
	mvstack.push(model_view);
		set_colour(.1, .1, .1);
		model_view *= Translate (0, -1.5, 0);
		model_view *= Scale(1, 2, 1);
		drawCube();
	model_view = mvstack.pop();
}
void drawTopArm2Left(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (-.75, -1.25, 0);
		model_view *= Scale(.25, .5, .25);
		drawSphere();
	model_view = mvstack.pop();
}

void drawBotArm2(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (-.75, -2, 0);
		model_view *= Scale(.25, .3, .25);
		drawSphere();
	model_view = mvstack.pop();


		mvstack.push(model_view);
		
			model_view *= Translate (-.75, -2.2, .4);
			model_view *= Scale(.2, .2, .2);
			model_view *= RotateX(90);
			model_view *= RotateY(180);
			drawSaber(1, 0, 0);
		model_view = mvstack.pop();
}

void drawArm2(void)
{
	mvstack.push(model_view);
		drawTopArm2Left();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		drawBotArm2();
	model_view = mvstack.pop();
}


void drawTopArm2Right(void)
{
	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (.75, -1.25, 0);
		model_view *= Scale(.25, .5, .25);
		drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour(.95, .9, .75);
		model_view *= Translate (.75, -2, 0);
		model_view *= Scale(.25, .3, .25);
		drawSphere();
	model_view = mvstack.pop();
}

void drawLegPart3(void)
{
	mvstack.push(model_view);
		set_colour(.1, .2, .1);
		model_view *= Scale(.47, 1.5, .47);
		drawOldCube();
	model_view = mvstack.pop();

}

void drawLegPart4(void)
{	
	mvstack.push(model_view);
		set_colour(.1, .1, .1);
		model_view *= Scale(.47, 1.5, .47);
		drawOldCube();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		set_colour (.1, .1, .1);
		model_view *= Scale (.5, .2 ,.7);
		model_view *= Translate(0, -3.4, .1);
		drawOldCube();
	model_view = mvstack.pop();
}

void drawGuy2(void)
{
	mvstack.push(model_view);
	if (TIME >= 20 && TIME < 20.5)
	{
		model_view *= RotateX(-600+TIME*30);
	}
	else if (TIME >= 20.5 && TIME < 21.5)
	{
		model_view *= RotateX(15);
	}
	else if (TIME >= 21.5 && TIME < 22)
	{
		model_view *= RotateX(15);
		model_view *= RotateX((21.5*30) - TIME*30);
	}
	else if (TIME >= 22 && TIME < 22.5)
	{
		model_view *= RotateY((22*90)- TIME*90);
	}
	else if (TIME >= 22.5 && TIME < 23)
	{
		model_view *= RotateY(-45);
	}
	else if (TIME >= 23 && TIME < 23.5)
	{
		model_view *= RotateY(-45);
		model_view *= RotateY((-23*90) + TIME*90);
	}
	else if (TIME >= 23.5 && TIME < 24)
	{
		model_view *= RotateY((-23.5 * 90) + TIME*90);
	}
	else if (TIME >= 24 && TIME < 24.5)
	{
		model_view *= RotateY(45);
	}
	else if (TIME >= 24.5 && TIME < 25)
	{
		model_view *= RotateY(45);
		model_view *= RotateY((24.5*90) - TIME*90);
	}
	else if (TIME >= 25 && TIME < 25.5)
	{
		model_view *= RotateX ((-25*30) + TIME*30);
	}
	else if (TIME >= 25.5 && TIME < 28.5)
	{
		model_view *= RotateX(15);
	}
	else if (TIME >= 28.5 && TIME < 29.5)
	{
		model_view *= RotateX((28.5*15) - TIME*15);
	}
	drawHead2();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		drawTorso2();
	model_view = mvstack.pop();
	
	// saber armi
	mvstack.push(model_view);
	if (TIME >=6 && TIME < 9.5)
	{
		model_view *= RotateX(-20*sin(3*TIME));
	}
	else if (TIME >= 9.5 && TIME < 10)
	{
		model_view *= RotateX(959 - TIME*102);
	}
	else if (TIME >= 10 && TIME < 16)
	{
		model_view *= RotateY(30*sin(5*TIME));
		model_view *= RotateX(-45);
	}
	else if (TIME >= 16 && TIME < 20)
	{
		model_view *= RotateX(-45);
	}
	else if (TIME >= 20 && TIME < 20.5)
	{
		model_view *= RotateX(-45);
		model_view *= RotateX(-1800 + TIME*20.5);
	}
	else if (TIME >= 29.5)
	{
		model_view *= RotateX(30*sin(20*TIME));
	}
	drawArm2();
	model_view = mvstack.pop();

	//static 
	mvstack.push(model_view);
	if (TIME >=6 && TIME < 10)
	{
		model_view *= RotateX(20*sin(3*TIME));
	}
	else if (TIME >= 10 &&  TIME < 20)
	{
		model_view *= RotateX(-45);
	}	
	else if (TIME >= 20 && TIME < 20.5)
	{
		model_view *= RotateX(-45);
		model_view *= RotateX(-1800 + TIME*20.5);
	}
	else if (TIME >= 29.5)
	{
		model_view *= RotateX(-30*sin(20*TIME));
	}
		drawTopArm2Right();
	model_view = mvstack.pop();

	//legs

	//LEG1
	mvstack.push(model_view);
	if (TIME >= 6 && TIME < 10)
	{
		model_view *= RotateX(5*sin(5*TIME));
	}
	else if (TIME >= 26.5 && TIME < 28.5)
	{
		model_view *= RotateX(5*sin(5*TIME));
	}
	else if (TIME >= 29.5)
	{
		model_view *= RotateX(5*sin(20*TIME));
	}
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(-.25, -2.5, 0);
			model_view *= RotateX(180);
			//model_view *= RotateX(5+sin(2*TIME));
			drawLegPart3();
			model_view *= Translate(0, .9, 0);
			model_view *= Scale (.2, .2, .2);
			drawSphere();
		model_view = mvstack.pop();

		//LOWERLEG 1
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(-.25, -4.32, 0);
			if (TIME >= 6 && TIME < 10)
			{
				model_view *= RotateX(5*sin(5*TIME));
			}
			else if (TIME >= 26.5 && TIME < 28.5)
			{
			model_view *= RotateX(5*sin(5*TIME));
			}
			else if (TIME >= 29.5)
			{
				model_view *= RotateX(5*sin(20*TIME));
			}
			drawLegPart4();
		model_view = mvstack.pop();
	model_view = mvstack.pop();

	//LEG2	
	mvstack.push(model_view);
		if (TIME >= 6 && TIME < 10)
		{
			model_view *= RotateX(-5*sin(5*TIME));
		}
		else if (TIME >= 26.5 && TIME < 28.5)
		{
			model_view *= RotateX(-5*sin(5*TIME));
		}
		else if (TIME >= 29.5)
		{
			model_view *= RotateX(-5*sin(20*TIME));
		}
		mvstack.push(model_view);
			set_colour(.8, .8, .8);
			model_view *= Translate(.25, -2.5, 0);
			model_view *= RotateX(180);
			drawLegPart3();
			model_view *= Translate(0, .9, 0);
			model_view *= Scale (.2, .2, .2);
			drawSphere();
		model_view = mvstack.pop();
		//LOWER LEG  2
		mvstack.push(model_view);
				set_colour(.8, .8, .8);
				model_view *= Translate(.25, -4.32, 0);
				if (TIME >= 6 && TIME < 10)
				{
					model_view *= RotateX(-5*sin(5*TIME));
				}
				else if (TIME >= 26.5 && TIME < 28.5)
				{
					model_view *= RotateX(-5*sin(5*TIME));
				}
					else if (TIME >= 29.5)
				{
					model_view *= RotateX(-5*sin(20*TIME));
				}
				drawLegPart4();
		model_view = mvstack.pop();

	model_view = mvstack.pop();
}

void drawWorld(void)
{
	mvstack.push(model_view);
	drawDesert();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate (0, 6 , -30);
	model_view *= RotateY(180);
	drawBench();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate (25, 4, -30);
	drawTree();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Scale(.9, .9, .9);
	model_view *= Translate (-25, 4, -30);
	drawTree();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Scale(1, 2, 1);
	model_view *= Translate (25, 4, 30);
	drawTree();
	model_view = mvstack.pop();
	

	//house
	mvstack.push(model_view);
	model_view *= Translate(-40, 10, 0);
	model_view *= RotateY(90);
	drawHouse();
	model_view = mvstack.pop();
	

	//moon
	mvstack.push(model_view);
	set_colour(.9, .9, .8);
	model_view *= Translate (-70, 75, 200);
	model_view *= Scale(30, 30, 30);
	drawSphere();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate(160,100, 0);
	model_view *= Scale (150, 200, 250);
	drawStone();
	model_view = mvstack.pop();

	mvstack.push(model_view);
	model_view *= Translate(-160,100, 0);
	model_view *= Scale (150, 200, 250);
	drawStone();
	model_view = mvstack.pop();

	if (TIME >= 7 && TIME < 8)
	{
		mvstack.push(model_view);
		model_view *= Translate (0, 10, -20);
		model_view *= Scale(1.2, 1.6, 1.2);
		model_view *= Translate (0, 66.5- TIME*9.5, -35 + TIME*5);
		model_view *= RotateX((180*7)-TIME*180);
		drawBanana1();
		model_view = mvstack.pop();
	}
	else if (TIME >= 8)
	{
		mvstack.push(model_view);
		model_view *= Translate (0, 0.5, -16.8);
		model_view *= Scale (1.2, 1.6, 1.2);
		model_view *= RotateX(180);
		drawBanana1();
		model_view = mvstack.pop();
	}
}

/*********************************************************
**********************************************************
**********************************************************

    PROC: display()
    DOES: this gets called by the event handler to draw
          the scene, so this is where you need to build
          your ROBOT --  
      
        MAKE YOUR CHANGES AND ADDITIONS HERE

    Add other procedures if you like.

**********************************************************
**********************************************************
**********************************************************/

void updateObjects()
{
}

void cam()
{
	if (TIME < 3){
		eye.x = 2;
		eye.y = 7;
		eye.z = -50;
	}
	else if (TIME >= 3 && TIME < 5)
	{ 
		eye.x = 2 + (3 - TIME);
	}
	else if (TIME >= 5 && TIME < 11)
	{
		eye.x = 80;
		eye.y = 28;
		eye.z = 0;
		ref.x = 0;
		ref.y = 17;
		ref.z = 0;
	}
	else if (TIME >= 11 && TIME < 14.0)
	{
		eye.x = 80*cos(2.41*TIME);
		eye.y = 40;
		eye.z = 80*sin(2.41*TIME);
	}
	else if (TIME >= 14.5 && TIME < 17)
	{
		eye.x = -3.5;
		eye.y = 2;
		eye.z = -25;
		ref.x = 0;
		ref.y = 4;
		ref.z = 0;
	}
	else if (TIME >= 18 && TIME < 21.5)
	{
		eye.y = 29;
		eye.z = -27;
		ref.z = -10;
	}
	else if (TIME >= 21.5)
	{
		eye.x = -3.5;
		eye.y = 2;
		eye.z = -25;
		ref.x = 0;
		ref.y = 4;
		ref.z = 0;
	}
}


double prev = 0;
int frames = 0;
int fps = 0;



void display(void)
{
    // Clear the screen with the background colour (set in myinit)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model_view = mat4(1.0f);
    
	double curprev = TM.GetElapsedTime() - prev;
	frames++;

	if (curprev > 1)
	{
		prev = TM.GetElapsedTime();
		fps = frames / curprev;
		frames = 0;
		printf("FPS: %i\n", fps);
	}
    model_view *= Translate(0.0f, 0.0f, -15.0f);
    HMatrix r;
    Ball_Value(Arcball,r);

    mat4 mat_arcball_rot(
        r[0][0], r[0][1], r[0][2], r[0][3],
        r[1][0], r[1][1], r[1][2], r[1][3],
        r[2][0], r[2][1], r[2][2], r[2][3],
        r[3][0], r[3][1], r[3][2], r[3][3]);
    model_view *= mat_arcball_rot;
    
    mat4 view = model_view;
    
    
   // model_view = Angel::LookAt(eye, ref, up);//just the view matrix;

    glUniformMatrix4fv( uView, 1, GL_TRUE, model_view );

    // Previously glScalef(Zoom, Zoom, Zoom);
    model_view *= Scale(Zoom);

	
    // Draw Something
	mvstack.push(model_view);
	drawWorld();
	model_view = mvstack.pop();
	//guy 1
	mvstack.push(model_view);
	model_view *= Translate (0, 8, -40);
	model_view *= Scale(1.5, 1.5, 1.5);
	model_view *= RotateY(180);
	// turn
	if (TIME >= 5 && TIME < 6)
	{
		model_view *= RotateY(-900 + TIME*180);
	}
	// move up
	else if (TIME >= 6 && TIME < 6.5)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0 , -36 + TIME*6);
	}
	// jump on bench
	else if (TIME >= 6.5 && TIME < 7)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, 3);
		model_view *= Translate (0, -16.25 + TIME*2.5, -13 + TIME*2);
	}
	// jump off bench
	else if (TIME >= 7 && TIME < 7.5)
	{
		model_view *= RotateY(180);
		model_view *= Translate(0, 1.25, 4);
		model_view *= Translate(0, -112 + TIME*16, -112 + TIME*16);
		model_view *= RotateX(-5040 + TIME*720);
	}
	// land
	else if (TIME >= 7.5 && TIME < 8)
	{
		model_view *= RotateY(180);
		model_view *= Translate(0, 9.25, 15);
		model_view *= Translate (0, 138.75-TIME*18.5, 0);
	}
	// walk toward other guy
	else if (TIME >= 8 && TIME < 9)
	{
		model_view *= RotateY(180);
		model_view *= Translate(0, 0, 15);
		model_view *= Translate(0, 0, -64 + TIME*8);
	}
	// stand for a bit
	else if (TIME >= 9 && TIME < 15)
	{
		model_view *= RotateY(180);
		model_view *= Translate(0, 0, 23);
		model_view *= RotateY(3*sin(5*TIME));
	}
	// spin thing
	else if (TIME >= 15 && TIME < 16)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, 23);
		model_view *= RotateY(-5400 + TIME*360);
	}
	// backflip thing
	else if (TIME >= 16 && TIME < 16.5)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, 23);
		model_view *= RotateX(5040 - TIME*720);
	}
	//walk backwards
	else if (TIME >= 16.5 && TIME < 17)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, 23);
		model_view *= Translate (0, 0, 264 - TIME*16);
	}
	//fall
	else if (TIME >= 17 && TIME < 17.5)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, 15);
		model_view *= Translate (0, (17*9) - TIME*9, 0);
		model_view *= RotateX (3060 - TIME*180);
	}
	// lay on ground
	else if (TIME >= 17.5)
	{
		model_view *= RotateY(180);
		model_view *= Translate (0, -4.5, 15);
		model_view *= RotateX (-90);
	}
	drawGuy1();
	model_view = mvstack.pop();

	//GUY 2
	mvstack.push(model_view);
	model_view *= Translate (-20, 8, 35);
	model_view *= Scale(1.5, 1.5, 1.5);
	model_view *= RotateY(180);
	if (TIME >= 6 && TIME < 8)
	{
		model_view *= Translate(39 - TIME*6.5, 0, 0);
		model_view *= RotateY(270 - TIME*45);
	}
	else if( TIME >= 8 && TIME  < 10)
	{
		model_view *= Translate (-13, 0, 0);
		model_view *= Translate (0, 0, -(8*10.5) + TIME*10.5);
	}
	else if (TIME >= 10 && TIME < 15)
	{
		model_view *= Translate (-13,0, 21);
		model_view *= RotateY(3*sin(5*TIME));
	}
	else if (TIME >= 15 && TIME < 26.5)
	{
		model_view *= Translate(-13, 0, 21);
	}
	else if (TIME >= 26.5 && TIME < 28.5)
	{
		model_view *= Translate (-13, 0, 21);
		model_view *= Translate (0, 0, (26.5*5) - TIME*5);
	}
	else if (TIME >= 28.5 && TIME < 29.5)
	{
		model_view *= Translate (-13, 0, 11);
		model_view *= RotateY((180*28.5) - TIME*180);
	}
	else if (TIME >= 29.5 && TIME < 31.5)
	{
		model_view *= Translate(-13, 0, 11);
		model_view *= RotateY(180);
		model_view *= Translate (0, 0, -(29.5*15) + 15*TIME);
	}


	drawGuy2();
	model_view = mvstack.pop();

	cam();
	


	if (TIME > 31.5)
		exit(0);

    glutSwapBuffers();
    if(Recording == 1)
        FrSaver.DumpPPM(Width, Height) ;
}

/**********************************************
    PROC: myReshape()
    DOES: handles the window being resized 
    
      -- don't change
**********************************************************/

void myReshape(int w, int h)
{
    Width = w;
    Height = h;

    glViewport(0, 0, w, h);

    mat4 projection = Perspective(50.0f, (float)w/(float)h, 1.0f, 1000.0f);
    glUniformMatrix4fv( uProjection, 1, GL_TRUE, projection );
}

void instructions() 
{
    printf("Press:\n");
    printf("  s to save the image\n");
    printf("  r to restore the original view.\n") ;
    printf("  0 to set it to the zero state.\n") ;
    printf("  a to toggle the animation.\n") ;
    printf("  m to toggle frame dumping.\n") ;
    printf("  q to quit.\n");
}

// start or end interaction
void myMouseCB(int button, int state, int x, int y)
{
    Button = button ;
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width-1.0;
        arcball_coords.y = -2.0*(float)y/(float)Height+1.0;
        Ball_Mouse(Arcball, arcball_coords) ;
        Ball_Update(Arcball);
        Ball_BeginDrag(Arcball);

    }
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    {
        Ball_EndDrag(Arcball);
        Button = -1 ;
    }
    if( Button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        PrevY = y ;
    }


    // Tell the system to redraw the window
    glutPostRedisplay() ;
}

// interaction (mouse motion)
void myMotionCB(int x, int y)
{
    if( Button == GLUT_LEFT_BUTTON )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width - 1.0 ;
        arcball_coords.y = -2.0*(float)y/(float)Height + 1.0 ;
        Ball_Mouse(Arcball,arcball_coords);
        Ball_Update(Arcball);
        glutPostRedisplay() ;
    }
    else if( Button == GLUT_RIGHT_BUTTON )
    {
        if( y - PrevY > 0 )
            Zoom  = Zoom * 1.03 ;
        else 
            Zoom  = Zoom * 0.97 ;
        PrevY = y ;
        glutPostRedisplay() ;
    }
}


void idleCB(void)
{
    if( Animate == 1 )
    {
        // TM.Reset() ; // commenting out this will make the time run from 0
        // leaving 'Time' counts the time interval between successive calls to idleCB
        if( Recording == 0 )
            TIME = TM.GetElapsedTime() ;
        else
            TIME += 0.033 ; // save at 30 frames per second.
        
      //  eye.x = 20*sin(TIME);
       // eye.z = 20*cos(TIME);

		updateObjects();
        
        printf("TIME %f\n", TIME) ;
        glutPostRedisplay() ; 
    }
}
/*********************************************************
     PROC: main()
     DOES: calls initialization, then hands over control
           to the event handler, which calls 
           display() whenever the screen needs to be redrawn
**********************************************************/

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    // If your code fails to run, uncommenting these lines may help.
    //glutInitContextVersion(3, 2);
    //glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Width,Height);
    glutCreateWindow(argv[0]);
    printf("GL version %s\n", glGetString(GL_VERSION));
    glewExperimental = GL_TRUE;
    glewInit();
    
    myinit();

    glutIdleFunc(idleCB) ;
    glutReshapeFunc (myReshape);
    glutKeyboardFunc( myKey );
    glutMouseFunc(myMouseCB) ;
    glutMotionFunc(myMotionCB) ;
    instructions();

    glutDisplayFunc(display);
    glutMainLoop();

    TM.Reset() ;
    return 0;         // never reached
}




