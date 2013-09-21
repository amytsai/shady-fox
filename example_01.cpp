
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>


#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;
class Rgb;
class Light;
class Vec3;

class Viewport {
  public:
    int w, h; // width and height
};

class Rgb {
  public:
    float red, green, blue;
    Rgb();
    Rgb(float, float, float);
};

Rgb::Rgb() {
  red = 0.0f;
  green = 0.0f;
  blue = 0.0f;
}

Rgb::Rgb(float r, float g, float b) {
    red = r;
    green = g;
    blue = b;
}

class Light {
  public:
    float x, y, z;
    Rgb rgb;
    bool isPL;
    Light();
    Light(float, float, float, Rgb, bool);
};

Light::Light() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  rgb = Rgb ();
  isPL = false;
}

Light::Light(float a, float b, float c, Rgb color, bool PL) {
  x = a;
  y = b;
  z = c;
  rgb = color;
  isPL = PL;
}

class Vec3 {
  public: 
    float x, y, z;
    Vec3();
    Vec3(float, float, float);
    Vec3 add(Vec3);
    Vec3 sub(Vec3);
    float dot(Vec3);
};

Vec3::Vec3() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

Vec3::Vec3(float a, float b, float c) {
  // constructs a vector, normalizes it if it is not normalized.
  if (a > 1 || b > 1 || c > 1) {
    float len = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
    x = a/len;
    y = b/len;
    z = c/len;
  } else {
    x = a;
    y = b;
    z = c;
  }
}

Vec3 Vec3::add(Vec3 v) {
  float a = x + v.x;
  float b = y + v.y;
  float c = z + v.z;

  if (a > 1 || b > 1 || c > 1) {
    float len = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
    a = a/len;
    b = b/len;
    c = c/len;
  }
  return Vec3(a,b,c);
}

Vec3 Vec3::sub(Vec3 v) {
  float a = x - v.x;
  float b = y - v.y;
  float c = z - v.z;

  if (a > 1 || b > 1 || c > 1) {
    float len = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
    a = a/len;
    b = b/len;
    c = c/len;
  }
  return Vec3(a,b,c);
}


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;
Rgb ka; // ambient color coefficient
Rgb kd; // diffuse color coefficient
Rgb ks; // specular color coefficient
float sp; // specular power coefficient
Light lights[10]; // array to hold up to 10 lights in the scene
int numLights = 0; // number of lights in the scene

//****************************************************
// Simple init function
//****************************************************
void initScene(){

  // Nothing to do here for this simple example.

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Draw a filled circle.  
//****************************************************


void circle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));



  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      float r, g, b;

      if (dist<=radius) {
        // Ambient term
        r = ka.red;
        g = ka.green;
        b = ka.blue;

        // Loop over all lights and calculate diffuse and specular terms
        for(int i = 0; i < numLights; i++) {
          Light curLight = lights[i];
          float lx, ly, lz ;//vector for direction to light source
          if (curLight.isPL) {

          } else {

          }

        }

        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);


        setPixel(i,j,r,g,b);

        // This is amusing, but it assumes negative color values are treated reasonably.
        //setPixel(i,j, x/radius, y/radius, z/radius );
      }


    }
  }


  glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Start drawing
  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //Parsing command line arguments
  std::cout << argc <<endl;
  //printf("asdf %s %s \n", "hello", "world");
  int i = 1;
  while (i < argc) {
    printf("argv[%d] = %s\n", i, argv[i]);
    if (strcmp(argv[i], "-ka") == 0) {
      ka.red = atof(argv[i+1]);
      ka.green = atof(argv[i+2]);
      ka.blue = atof(argv[i+3]);
      printf("ka: %f, %f, %f \n", ka.red, ka.green, ka.blue);
      i+=4;
    }
    else if (strcmp(argv[i], "-kd") == 0) {
      kd.red = atof(argv[i+1]);
      kd.green = atof(argv[i+2]);
      kd.blue = atof(argv[i+3]);
      printf("kd: %f, %f, %f \n", kd.red, kd.green, kd.blue);
      i+=4;
    }
    else if (strcmp(argv[i], "-ks") == 0) {
      ks.red = atof(argv[i+1]);
      ks.green = atof(argv[i+2]);
      ks.blue = atof(argv[i+3]);
      printf("ks: %f, %f, %f \n", ks.red, ks.green, ks.blue);
      i+=4;
    }
    else if (strcmp(argv[i], "-sp") == 0) {
      sp = atof(argv[i+1]);
      printf("sp: %f \n", sp);
      i+=2;
    }
    else if (strcmp(argv[i], "-pl") == 0) {
      Light temp (atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]), Rgb(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])), true);
      lights[numLights] = temp;
      printf("added point light \n");
      numLights++;
      i+=7;
    }
    else if (strcmp(argv[i], "-dl") == 0) {
      Light temp (atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]), Rgb(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])), false);
      lights[numLights] = temp;
      printf("added directional light \n");
      numLights++;
      i+=7;

    } else {
      std::cout << "Not enough or invalid arguments please try again\n";
      sleep(2000);
      exit(0);
    }
  }
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);        // function to run when its time to draw something
  glutReshapeFunc(myReshape);        // function to run when the window gets resized

  glutMainLoop();							// infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}








