/*
 *  CSE 40166 Computer Graphics, Fall 2012
 *  David Lopes, Isaac Harrington
 * 
 *  FINAL PROJECT
 *
 */

/* must be included first before GL.h */
/*
#include <GL/glew.h>

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <SOIL/soil.h>

#include <CSE40166/CSE40166.h>
using namespace CSE40166;
*/

#include "headers.h" // opengl headers and stuff

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
using namespace std;

#include "ParticleManager.h"
#include "Singularity.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

//global variables to keep track of window width and height.
//set to initial values for convenience, but we need variables
//for later on in case the window gets resized.
int windowWidth = 512, windowHeight = 512;

GLint leftMouseButton = GLUT_UP, rightMouseButton = GLUT_UP;    //status of the mouse buttons
int mouseX = 0, mouseY = 0;					//last known X and Y of the mouse
int leftClickMouseX = 0, leftClickMouseY = 0;

std::map<char, short> keyState;

// level dimensions
float levelWidth = 50, levelHeight = 50, levelDepth = 10;

// keeping track of time
double lastTime = 0;

// a pointer to each of our camera types
Camera *arcballCam = NULL;
// and a pointer to the current camera we are using
Camera *currentCamera = NULL; 
Point *singularityLoc;

// Our particle manager
ParticleManager *particleManager;

// Our hero, the singularity
Singularity *singularity;

// Shader handles
GLint velocityShaderHandle;
GLint velocityVertexShader, velocityFragmentShader;

// Texture handles
std::map<string, GLuint> textures;

// END GLOBAL VARIABLES ///////////////////////////////////////////////////////


void resizeWindow(int w, int h) {
  float aspectRatio = w / (float)h;
	
  windowWidth = w;
  windowHeight = h;
	
  //update the viewport to fill the window
  glViewport(0, 0, w, h);
	
  //update the projection matrix with the new window properties
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,aspectRatio,0.1,100000);
}


void initScene() {
  glEnable(GL_DEPTH_TEST);
		
  //tells OpenGL to blend colors across triangles. Once lighting is
  //enabled, this means that objects will appear smoother - if your object
  //is rounded or has a smooth surface, this is probably a good idea;
  //if your object has a blocky surface, you probably want to disable this.
  glShadeModel(GL_SMOOTH);
	
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void setupCameras() {
  arcballCam = new Camera( ARCBALLCAM );		// create an arcball camera

  currentCamera = arcballCam;
	
  // and give the camera a 'pretty' starting point!
  currentCamera->setRadius( 10.0f );
  currentCamera->setTheta( 0.0f );
  currentCamera->setPhi( M_PI/2 - 0.001f );

  singularityLoc = new Point;
  currentCamera->followPoint( singularityLoc );
  currentCamera->computeArcballPosition();	
}

void setupLights() {
  // by default there is a little bit of ambient light in our scene everywhere
  // so turn if off
  float inkyBlackness[4] = {0.0, 0.0, 0.0, 1.0};
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, inkyBlackness );

}


////////////////////////
// DRAWING FUNCTIONS ///
////////////////////////
void drawBackground (void)  {
  static short firstTime = 1;
  static GLuint backgroundHandle;

  if (!firstTime) {
    glCallList(backgroundHandle);
  }

  firstTime = 0;
  backgroundHandle = glGenLists(1);
  glNewList(backgroundHandle, GL_COMPILE); {  
    glUseProgram(0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, textures["images/space.jpg"] );
    
    glBegin(GL_QUADS); {      
      glTexCoord2f(1.0, 1.0); glVertex3f(-levelWidth, -levelHeight, levelDepth);
      glTexCoord2f(1.0, 0.0); glVertex3f(-levelWidth, levelHeight,  levelDepth);
      glTexCoord2f(0.0, 0.0); glVertex3f( levelWidth, levelHeight,  levelDepth);
      glTexCoord2f(0.0, 1.0); glVertex3f( levelWidth, -levelHeight, levelDepth);
    } glEnd();
    
    glDisable(GL_TEXTURE_2D);
  } glEndList();

}

// Draw a selection box for the user
void drawSelectionBox () {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  // calculate on-screen position of a click  
  GLdouble model_view[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  GLdouble projection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLdouble winX, winY, winZ;

  // super useful function
  gluUnProject(mouseX, windowHeight - mouseY, 0,
	       model_view, projection, viewport,
	       &winX, &winY, &winZ);    
  double x1 = winX, y1 = winY, z1 = winZ;
  gluUnProject(leftClickMouseX, windowHeight - leftClickMouseY, 0,
	       model_view, projection, viewport,
	       &winX, &winY, &winZ);
  double x2 = winX, y2 = winY;
  
  // Vector l = *(currentCamera->getLookAt()) - *(currentCamera->getEye());
  // x1 += l.x * 0.01;
  // x2 += l.x * 0.01;
  // y1 += l.y * 0.01;
  // y2 += l.y * 0.01;
  // z1 += l.z * 0.01;

  cout << x1 << ", " << y1 << " -- " << x2 << ", " << y2 << endl;
  
  glColor3f(1,1,1);
  glBegin(GL_LINES); {
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y1, z1);
  } glEnd();

  glEnable(GL_DEPTH_TEST);  
}

// RENDER SCENE
void renderScene(void) {
  //clear the render buffer
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  //update the modelview matrix based on the camera's position
  glMatrixMode(GL_MODELVIEW);             //make sure we aren't changing the projection matrix!
  glLoadIdentity();
	
  currentCamera->look();
	
  //code to render our OBJ model...
  glEnable(GL_DEPTH_TEST);

  // DRAW STUFF HERE //

  // Send our light positions to the shaders
  glUseProgram(velocityShaderHandle);

  GLfloat lights [16] = {singularityLoc->x,singularityLoc->y,singularityLoc->z,
			 1.0, 1.0, 2.0,
			 0.0, 0.0, 2.0,
			 0.0, 1.0, 1.0};
  GLint amt = glGetUniformLocation(velocityShaderHandle, "amtCelLights");
  glUniform1i(amt, 1);
  GLint pos = glGetUniformLocation(velocityShaderHandle, "lightPos");
  glUniform4fv(pos, 4, lights);

  drawBackground();
  singularity->draw();
  particleManager->draw();  

  if (leftMouseButton == GLUT_DOWN) {
    drawSelectionBox();
  }

  // DONE DRAWING //
  
  //push the back buffer to the screen
  glutSwapBuffers();
}


void normalKeys(unsigned char key, int x, int y) {
  //kindly quit if the user requests!
  if(key == 'q' || key == 'Q')
    exit(0);
  
  keyState[key] = 1;
  switch (key) {    
    /*
  case 's': case 'S':
    keyState['s']
    singularity->vel.x += 0.005;
    break;
  case 'a': case 'A':
    singularity->vel.x -= 0.005;
    break;
    */
  }

  //glutPostRedisplay(); // post only if we changed the camera at all
}

void keyUp(unsigned char key, int x, int y) {
  keyState[key] = 0;
}

void mouseCallback(int button, int state, int thisX, int thisY) {
  //update the left and right mouse button states, if applicable
  if(button == GLUT_LEFT_BUTTON) {
    leftMouseButton = state;

    if (state == GLUT_DOWN) {
      leftClickMouseX = thisX;
      leftClickMouseY = thisY;
    } else {
      //calculate square, send notice to particlemanager
      particleManager->acquireStrayParticlesInBox(leftClickMouseX, windowHeight - leftClickMouseY, 
						  mouseX, windowHeight - mouseY, &(singularity->system));
    }

  } else if(button == GLUT_RIGHT_BUTTON) {
    rightMouseButton = state;
  }
    
  //and update the last seen X and Y coordinates of the mouse
  mouseX = thisX;
  mouseY = thisY;
}

void mouseMotion(int x, int y) {
  if ( leftMouseButton == GLUT_DOWN ) {
    //update theta and phi! 
    currentCamera->setTheta( currentCamera->getTheta() + (x-mouseX)*0.005 );
    currentCamera->setPhi( currentCamera->getPhi() + (y-mouseY)*0.005 );
			
    // we don't care if theta goes out of bounds; it'll just loop around.
    // phi, though.. it'll flip out if it flips over top. keep it in (0, M_PI)
    if( currentCamera->getPhi()  <= 0 )
      currentCamera->setPhi( 0+0.001 );
    if( currentCamera->getPhi() >= M_PI )
      currentCamera->setPhi( M_PI-0.001 );
			
    //update camera (x,y,z) based on (radius,theta,phi)
    currentCamera->computeArcballPosition();

  } else if( rightMouseButton == GLUT_DOWN ) {
    //for the right mouse button, just determine how much the mouse has moved total.
    //not the best "zoom" behavior -- if X change is positive and Y change is negative,
    //(along the X = Y line), nothing will happen!! but it's ok if you zoom by
    //moving left<-->right or up<-->down, which works for most people i think.
    double totalChangeSq = (x - mouseX) + (y - mouseY);
    currentCamera->setRadius( currentCamera->getRadius() + totalChangeSq*0.02 );
    
    //limit the camera radius to some reasonable values so the user can't get lost
    if( currentCamera->getRadius() < 2.0 )
      currentCamera->setRadius( 2.0 );
    if( currentCamera->getRadius() > 20.0 )
      currentCamera->setRadius( 20.0 );
    
    //update camera (x,y,z) based on (radius,theta,phi)
    currentCamera->computeArcballPosition();
  }

  mouseX = x;
  mouseY = y;
}

// (value taken in is a dummy)
void myTimer(int value) {	

  double currTime = glutGet(GLUT_ELAPSED_TIME);
  double timePassed = (currTime - lastTime)/1000;
  lastTime = currTime;

  if (keyState['s'] == 1) { // the s key is pressed
    singularity->vel.x += 0.005;
  }
  if (keyState['a'] == 1) {
    singularity->vel.x -= 0.005;
  }

  particleManager->update(timePassed);
  singularity->update(timePassed);

  glutPostRedisplay();
  glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
}

// readTextFile() //////////////////////////////////////////////////////////////
//  Reads in a text file as a single string. Used to aid in shader loading.
////////////////////////////////////////////////////////////////////////////////
void readTextFile(string filename, char* &output)
{
  string buf = string("");
  string line;
  
  ifstream in(filename.c_str());
  while(getline(in, line))
    buf += line + "\n";
  output = new char[buf.length()+1];
  strncpy(output, buf.c_str(), buf.length());
  output[buf.length()] = '\0';
  
  in.close();
}


// printLog() //////////////////////////////////////////////////////////////////
//  Check for errors from compiling or linking a vertex/fragment/shader program
//	Prints to terminal
////////////////////////////////////////////////////////////////////////////////
void printLog( GLuint handle ) {
  int infologLength = 0;
  int maxLength;
	
  /* check if the handle is to a vertex/fragment shader */
  if( glIsShader( handle ) )
    glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
  /* check if the handle is to a shader program */
  else
    glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
	
  /* create a buffer of designated length */
  char infoLog[maxLength];
	
  if( glIsShader( handle ) )
    /* get the info log for the vertex/fragment shader */
    glGetShaderInfoLog(  handle, maxLength, &infologLength, infoLog );
  else
    /* get the info log for the shader program */
    glGetProgramInfoLog( handle, maxLength, &infologLength, infoLog );
	
  /* if the length of the log is greater than 0 */
  if( infologLength > 0 )
    /* print info to terminal */
    printf( "[INFO]: Shader Handle %d: %s\n", handle, infoLog );
}

// Helper function to reduce clutter. 
// Loads a single texture using SOIL
void loadOneTexture (GLuint& index, const char* tex) {
  index = SOIL_load_OGL_texture( tex, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
				 SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT );
}

// Load all textures that will be used in our scene
// Also set some parameters for our textures
void loadTextures () {
  
  const int amt_textures = 1;
  const char* names [amt_textures] = {"images/space.jpg"};  
  
  for (int i = 0; i < amt_textures; ++i) {
    GLuint& t = textures[names[i]];
    loadOneTexture ( t, names[i] );
    cout << "Texture named " << names[i] << " indexed at " << textures[names[i]] << endl;
  }

  // TODO: find out what these do =P
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}


int setupShaders() {
	char *vertexShaderString, *fragmentShaderString;
	
	/* read in each text file and store the contents in a string */
	readTextFile( "velocity_shader.v.glsl", vertexShaderString   );
	readTextFile( "velocity_shader.f.glsl", fragmentShaderString );
	
	/* create a handle to our vertex and fragment shaders */
	velocityVertexShader   = glCreateShader( GL_VERTEX_SHADER   );
	velocityFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	
	/* send the contents of each program to the GPU */
	glShaderSource( velocityVertexShader,   1, (const char**)&vertexShaderString,   NULL );	printLog( velocityVertexShader   );
	glShaderSource( velocityFragmentShader, 1, (const char**)&fragmentShaderString, NULL );	printLog( velocityFragmentShader );
	
	/* we are good programmers so free up the memory used by each buffer */
	// ... why are we mixing malloc and delete? that's baaaad
	//delete [] vertexShaderString;
	//delete [] fragmentShaderString;
	
	/* compile each shader on the GPU */
	glCompileShader( velocityVertexShader   );	printLog( velocityVertexShader   );
	glCompileShader( velocityFragmentShader );	printLog( velocityFragmentShader );
	
	/* get a handle to a shader program */
	velocityShaderHandle = glCreateProgram();
	
	/* attach the vertex and fragment shaders to the shader program */
	glAttachShader( velocityShaderHandle, velocityVertexShader   );	printLog( velocityVertexShader   );
	glAttachShader( velocityShaderHandle, velocityFragmentShader );	printLog( velocityFragmentShader );
	
	/* link all the programs together on the GPU */
	glLinkProgram( velocityShaderHandle );	printLog( velocityShaderHandle );
	
	/* return success */
	return 0;
}


int main(int argc, char **argv) {

  /////////////////////////////
  // SETUP AND CREATE WINDOW //
  /////////////////////////////

  //create a double-buffered GLUT window at (50,50) with predefined windowsize
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(50,50);
  glutInitWindowSize(windowWidth,windowHeight);
  glutCreateWindow("Final Project");
	

  //////////////////////////
  // INITIALIZE LIBRARIES //
  //////////////////////////
  
  /* initialize CSE40166 */
  CSE40166Init( true, false );	
  /* initialize GLEW */
  GLenum glewResult = glewInit();
  if( glewResult != GLEW_OK ) {
    printf( "Error initalizing GLEW\n");
    return 0;
  }
  // ensure OpenGL 2.0 is supported
  if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
    printf( "System does not support OpenGL 2.0 and GLSL\n" );
    return 0;
  }


  //////////////////////
  // INITIALIZE SCENE //
  //////////////////////
	
  initScene();		// do some basic OpenGL setup
  setupCameras();		// setup our cameras
  setupLights();		// setup our lights
  loadTextures(); // load our textures

  int shaderResult = setupShaders();	// setup our shaders
  if( shaderResult != 0 ) {
    cout << "Bad shaders." << endl;
    return 0;
  }


  /////////////////////////////
  // INITIALIZE GAME OBJECTS //
  /////////////////////////////

  singularity = new Singularity( singularityLoc );  
  
  // create particle manager
  particleManager = new ParticleManager;
  particleManager->setShader(velocityShaderHandle);
  
  // link it to the particle systems
  particleManager->manageSystem( &(singularity->system) );
  singularity->manager = particleManager;
	
  //register callback functions...
  glutKeyboardFunc(normalKeys);
  glutKeyboardUpFunc(keyUp);
  glutDisplayFunc(renderScene);
  glutReshapeFunc(resizeWindow);
  glutMouseFunc( mouseCallback );
  glutMotionFunc( mouseMotion );	
  glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);

  //and enter the GLUT loop, never to exit.
  glutMainLoop();
	
  return 0;
}
