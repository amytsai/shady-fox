
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
    Vec3 dirToLight(Light);
    float dot(Vec3);
    Vec3 times(float);
    Vec3 sub(Vec3);

};

Vec3::Vec3() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

Vec3::Vec3(float a, float b, float c) {
  // constructs a vector, normalizes it if it is not normalized.
    float len = sqrt(a*a + b*b + c*c);
    x = a/len;
    y = b/len;
    z = c/len;
}

Vec3 Vec3::dirToLight(Light l) {
  float a = l.x - x;
  float b = l.y - y;
  float c = l.z - z;

  float len = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
  a = a/len;
  b = b/len;
  c = c/len;

  return Vec3(a,b,c);
}

float Vec3::dot(Vec3 v) {
  return x*v.x + y*v.y + z*v.z;
}

Vec3 Vec3::times(float k) {
  float a = k*x;
  float b = k*y;
  float c = k*z;
  return Vec3(a,b,c);
}

Vec3 Vec3::sub(Vec3 v) {
  float a = x - v.x;
  float b = y - v.y;
  float c = z - v.z;

  float len = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
  a = a/len;
  b = b/len;
  c = c/len;
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
float innerRad; // inner radius of a torus
float outerRad; // outer radius of a torus
bool isTor = false; // whether we draw a torus or not
bool isToon = false; // whether or not to use toon shading

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
// Calculates a "toon" color by narrowing the allowable color range
//****************************************************
float toonify(float color) {
  if (color < 0.25f) {
    return 0.1f;
  } else if (color < 0.5) {
    return 0.5f;
  } else {
    return 1.0f;
  }
}

float toonify2(float color) {
  if (color < 0.125f) {
    return 0.0f;
  } else if (color < 0.375f) {
    return 0.25f;
  } else if (color < 0.625f) {
    return 0.5f;
  } else if (color < 0.875f) {
    return 0.75f;
  } else {
    return 1.0f;
  }
}

float toonify3(float color) {
  if (color < 0.25f) {
    return 0.2f;
  } else if (color < 0.5) {
    return 0.4f;
  } else if (color < 0.75) {
    return 0.6f;
  } else if (color < 0.95f) {
    return 0.8f;
  } else {
    return 1.0f;
  }
}

//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  if (isToon) {
    glColor3f(toonify(r), toonify(g), toonify(b));
  } else {
    glColor3f(r, g, b);
  } 
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

        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        // Loop over all lights and calculate diffuse and specular terms
        for(int i = 0; i < numLights; i++) {
          Light curLight = lights[i];
          Vec3 n (x, y, z); // normal to surface
		      Vec3 eye (0.0f, 0.0f, 1.0f); //viewer
          if (curLight.isPL) {

			      //diffuse shading
            Vec3 l = n.dirToLight(curLight);
            float dotp = l.dot(n);
            r += max(0.0f, kd.red*dotp*curLight.rgb.red);
            g += max(0.0f, kd.green*dotp*curLight.rgb.green);
            b += max(0.0f, kd.blue*dotp*curLight.rgb.blue);

      			//specular shading
            float dotp2 = dotp*2;
            Vec3 ref(n.x*dotp2 - l.x, n.y*dotp2 - l.y, n.z*dotp2 - l.z);
      			float dotps =  eye.dot(ref);
      			r += ks.red*curLight.rgb.red*pow(max(0.0f, dotps), sp);
      			g += ks.green*curLight.rgb.green*pow(max(0.0f, dotps), sp);
      			b += ks.blue*curLight.rgb.blue*pow(max(0.0f, dotps), sp);

          } else {
            Vec3 dl (-curLight.x, -curLight.y, -curLight.z);
			      float dotp = dl.dot(n);
            r += max(0.0f, kd.red * curLight.rgb.red * dotp);
            g += max(0.0f, kd.green * curLight.rgb.green * dotp);
            b += max(0.0f, kd.blue * curLight.rgb.blue * dotp);

			      //specular shading
            float dotp2 = dotp*2;
            Vec3 ref(n.x*dotp2 - dl.x, n.y*dotp2 - dl.y, n.z*dotp2 - dl.z);
      			float dotps =  eye.dot(ref);
      			r += ks.red*curLight.rgb.red*pow(max(0.0f, dotps), sp);
      			g += ks.green*curLight.rgb.green*pow(max(0.0f, dotps), sp);
      			b += ks.blue*curLight.rgb.blue*pow(max(0.0f, dotps), sp);
          }
        }

        r = min(r, 1.0f);
        g = min(g, 1.0f);
        b = min(b, 1.0f);
        setPixel(i,j,r,g,b);

        // This is amusing, but it assumes negative color values are treated reasonably.
        //setPixel(i,j, x/radius, y/radius, z/radius );
      } 
      if(isToon) {
        if (dist > radius && dist < radius + 3) {
          setPixel(i,j,0.0f,0.0f,0.0f);
        } else if (dist > radius) {
          setPixel(i,j,1.0f,1.0f,1.0f);
        }
        
      }


    }
  }


  glEnd();
}

//****************************************************
// Draw a filled torus.  
//****************************************************


void torus(float centerX, float centerY, float innerRadius, float outerRadius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

  //int minI = max(0,(int)floor(centerX-radius));
  //int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  //int minJ = max(0,(int)floor(centerY-radius));
  //int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));

  outerRadius = min(viewport.w, viewport.h) / 3.0;
  innerRadius = min(viewport.w, viewport.h) / 6.0;


  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));
	  float torusCenter = (innerRadius + outerRadius)/2;

      float r, g, b;

      if (dist<=outerRadius && dist >= innerRadius) {
        // Ambient term
        r = ka.red;
        g = ka.green;
        b = ka.blue;

        // This is the front-facing Z coordinate
        float z = sqrt((outerRadius - dist)*(dist - innerRadius));

        // Loop over all lights and calculate diffuse and specular terms
        for(int i = 0; i < numLights; i++) {
          Light curLight = lights[i];
          Vec3 n (x - (torusCenter)/dist * x, y - (torusCenter)/dist * y, z); // normal to surface
		      Vec3 eye (0.0f, 0.0f, 1.0f); //viewer
          if (curLight.isPL) {

			      //diffuse shading
            Vec3 l = n.dirToLight(curLight);
            float dotp = l.dot(n);
            r += max(0.0f, kd.red*dotp*curLight.rgb.red);
            g += max(0.0f, kd.green*dotp*curLight.rgb.green);
            b += max(0.0f, kd.blue*dotp*curLight.rgb.blue);

      			//specular shading
            float dotp2 = dotp*2;
            Vec3 ref(n.x*dotp2 - l.x, n.y*dotp2 - l.y, n.z*dotp2 - l.z);
      			float dotps =  eye.dot(ref);
      			r += ks.red*curLight.rgb.red*pow(max(0.0f, dotps), sp);
      			g += ks.green*curLight.rgb.green*pow(max(0.0f, dotps), sp);
      			b += ks.blue*curLight.rgb.blue*pow(max(0.0f, dotps), sp);

          } else {
            Vec3 dl (-curLight.x, -curLight.y, -curLight.z);
			      float dotp = dl.dot(n);
            r += max(0.0f, kd.red * curLight.rgb.red * dotp);
            g += max(0.0f, kd.green * curLight.rgb.green * dotp);
            b += max(0.0f, kd.blue * curLight.rgb.blue * dotp);

			      //specular shading
            float dotp2 = dotp*2;
            Vec3 ref(n.x*dotp2 - dl.x, n.y*dotp2 - dl.y, n.z*dotp2 - dl.z);
      			float dotps =  eye.dot(ref);
      			r += ks.red*curLight.rgb.red*pow(max(0.0f, dotps), sp);
      			g += ks.green*curLight.rgb.green*pow(max(0.0f, dotps), sp);
      			b += ks.blue*curLight.rgb.blue*pow(max(0.0f, dotps), sp);
          }
        }

        r = min(r, 1.0f);
        g = min(g, 1.0f);
        b = min(b, 1.0f);
        setPixel(i,j,r,g,b);

        // This is amusing, but it assumes negative color values are treated reasonably.
        //setPixel(i,j, x/radius, y/radius, z/radius );
      } else if(isToon) {
        if (dist > outerRadius && dist <= outerRadius+2) {
          setPixel(i,j, 0.0f, 0.0f, 0.0f);
        } else if (dist < innerRadius && dist >= innerRadius-2){
          setPixel(i,j, 0.0f, 0.0f, 0.0f);
        } else {
          setPixel(i,j,1.0f,1.0f,1.0f);          
        }

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
  if(isTor) {
	  torus(viewport.w / 2.0 , viewport.h / 2.0 , innerRad, outerRad);
  }
  else {
		circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);
  }
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
  Vec3 test1(0.1f, 0.0f, 0.0f);
  printf("test1 (%f, %f, %f) \n", test1.x, test1.y, test1.z);

  Vec3 test2(0.0f, 0.0f, 0.1f);
  printf("test2 (%f, %f, %f) \n", test1.x, test1.y, test1.z);

  printf("test1 dot test2: %f\n", test1.dot(test2));
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
    } 
	else if (strcmp(argv[i], "-tor") == 0) {
		innerRad = atof(argv[i+1]);
		outerRad = atof(argv[i+2]);
		isTor = true;
		printf("tor: %f, %f \n", innerRad, outerRad);
		i+=3;
    } 
  else if (strcmp(argv[i], "-toon") == 0) {
    isToon = true;
    i+=1;
  }
	else {
      std::cout << "Not enough or invalid arguments please try again\n";
      //sleep(2000);
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








