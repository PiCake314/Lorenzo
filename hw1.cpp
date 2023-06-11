/*
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2 // or -DRES=1
#ifndef RES
#define RES 1
#endif

//  Rotation angles
double xTh = 15;
double yTh = 65;


double xShift = -.8;
double yShift = 0;
double zShift = -3.5;


float len = 0.8;

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}


/*
 * Convenience function for text
 */
#define LEN 8192  //  Maximum amount of text
void Print(const char* format , ...)
{
   char    buf[LEN]; // Text storage
   char*   ch=buf;   // Text pointer
   //  Create text to be display
   va_list args;
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display text string
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.1666;
double r  = 28;

char letter = 'G';


const char *text(char chr){
   switch (chr){
      case 'G':
         return "Params: (S)=%g B=%g R=%g, X=%g Y=%g Z=%g, XTh=%g YTh=%g";
      
      case 'B':
         return "Params: S=%g (B)=%g R=%g, X=%g Y=%g Z=%g, XTh=%g YTh=%g";

      case 'R':
         return "Params: S=%g B=%g (R)=%g, X=%g Y=%g Z=%g, XTh=%g YTh=%g";
   }

   return "Params: S=%g B=%g R=%g, X=%g Y=%g Z=%g, XTh=%g YTh=%g";
}


void display(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   glTranslatef(xShift, yShift, zShift);
   glRotatef(xTh, 1, 0, 0);
   glRotatef(yTh, 0, 1, 0);
   glTranslatef(-xShift, -yShift, -zShift);

   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;

   /*  Time step  */
   double dt = 0.001;
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < 50000 ; i++){
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += (dt*dx);
      y += (dt*dy);
      z += (dt*dz);
      // printf("%5d %8.3f %8.3f %8.3f\n", i+1, x, y, z);

      // glScalef(zShift, zShift,zShift);
      double xColor = (x + 40)/80;
      double yColor = (y + 40)/80;
      double zColor = (z + 40)/80;
      glColor3f(xColor, yColor, zColor);
      // glTranslatef(xShift, yShift, zShift);
      glVertex3f((x*0.03 + xShift), (y*0.03 + yShift), (z*.03 + zShift));
   }
   glEnd();


   // Drawing axis
   glColor3f(1,1,1);
   glBegin(GL_LINES);

   glVertex3d(xShift, yShift, zShift);
   glVertex3d((len + xShift), yShift, zShift);

   
   glVertex3d(xShift, yShift, zShift);
   glVertex3d(xShift, (len + yShift), zShift);

   
   glVertex3d(xShift, yShift, zShift);
   glVertex3d(xShift, yShift, (len + zShift));


   glEnd();

   glWindowPos2i(5,5);
   // Label axis

   
   glRasterPos3d((len + xShift), yShift, zShift);
   Print("X");

   glRasterPos3d(xShift, (len + yShift), zShift);
   Print("Y");

   glRasterPos3d(xShift, yShift, (len + zShift));
   Print("Z");



   glWindowPos2i(5,5);


   Print(text(letter), s, b, r, xShift, yShift, zShift, xTh, yTh);

   glFlush();
   glutSwapBuffers();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height)
{
   //  Set viewport as entire window
   glViewport(0, 0, RES*width, RES*height);
   //  Select projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Set projection to identity
   glLoadIdentity();


   // //  Orthogonal projection:  unit cube adjusted for aspect ratio
   // double asp = (height>0) ? (double)width/height : 1;
   // glOrtho(-asp,+asp, -1, +1, -1.f, 1.f);

   // Apply perspective projection
   double aspectRatio = (double)width / height;
   gluPerspective(45, aspectRatio, 0.1, 100);


   //  Select model view matrix
   glMatrixMode(GL_MODELVIEW);
   //  Set model view to identity
   glLoadIdentity();
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y){

   switch (key){
      case GLUT_KEY_RIGHT: // Right arrow - increase rotation by 5 degree
         yTh += 5;
         break;
   
      case GLUT_KEY_LEFT: // Left arrow - decrease rotation by 5 degree
         yTh -= 5;
         break;
   
      case GLUT_KEY_UP: // Up arrow - decrease rotation by 5 degree
         xTh -= 5;
         break;
   
      case GLUT_KEY_DOWN: // Down arrow - increases rotation by 5 degree
         xTh += 5;
         break;
   }


   //  Request display update
   glutPostRedisplay();
}


void keyboard(unsigned char ch,int x,int y){
   switch (ch){
      case 27:   // Exit on ESC
         exit(0);
         break;
      
      case 'w':
         zShift += 0.1;
         break;
      
      case 's':
         zShift -= 0.1;
         break;

      case 'a': // Move to the left
         xShift += 0.1;
         break;

      case 'd': // Move to the right
         xShift -= 0.1;
         break;

      case ' ':
         yShift -= 0.1;
         break;

      case '`':
         yShift += 0.1;
         break;

      case '0':  // Reset the view angle
         xTh = yTh = 0;
         break;
      
      case 'g':
         letter = 'G';
         break;
      
      case 'b':
         letter = 'B';
         break;
      
      case 'r':
         letter = 'R';
         break;
      
      case '<':
         if (letter == 'G') s -= 0.1;
         else if (letter == 'B') b -= 0.1;
         else if (letter == 'R') r -= 0.1;
      
         break;

      case '>':
         if (letter == 'G') s += 0.1;
         else if (letter == 'B') b += 0.1;
         else if (letter == 'R') r += 0.1;

         break;
         
   }

   //  Request display update
   glutPostRedisplay();
}

/*
 *  Main
 */
int main(int argc, char *argv[])
{
   
   //  Initialize GLUT
   glutInit(&argc,argv);
   glutInitWindowSize(1080, 720);
   //  Request double buffered true color window with Z-buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //  Create window
   glutCreateWindow("Assignment 1: Ali Almutawa Jr. - Lorenz Attractor");

#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif

   //  Register display, reshape and key callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);

   //  Enable Z-buffer depth test
   glEnable(GL_DEPTH_TEST);

   //  Pass control to GLUT for events
   glutMainLoop();

   return 0;
}