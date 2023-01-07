/***************************************************
 * Example program: Draw a moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>


#define   PI   3.1415927

float  points[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};

int    face[][4] = {{0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1}, 
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4}};

int    cube[6] = {0, 1, 2, 3, 4, 5};

float  color[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
{1, 1, 0}, {1, 0, 1}, {0, 1, 1}};


int    width=500, height=500;    /* window shape */

float  pos[3] = {4.0, 0.0, 4.0};
float  angle = 0.0;

/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj  *sphere=NULL, *cylind=NULL;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, 499, 499);

  /*-----Set a parallel projection mode-----*/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-8.0, 8.0 , -8.0, 8.0, 0.0, 20.0);
  
  glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
   
  glFlush();/*Enforce window system display the results*/
}


/*--------------------------------------------------------
 * Procedure to draw the cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube(void)
{
  int    i;


  for(i=0;i<6;i++){     /* draw the six faces one by one */
    glColor3fv(color[i]);  /* Set the current color */

    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}


/*----------------------------------------------------------------
 * Procedure to draw a unit rectangle in the z-x plane.
 */
void draw_rectangle(void)
{
	glBegin(GL_POLYGON);
	  glVertex3f(0, 0, 0);
	  glVertex3f(0, 0, 1);
	  glVertex3f(1, 0, 1);
	  glVertex3f(1, 0, 0);
	glEnd();
}

/*------------------------------------------------------------
 * Procedure to draw a floor of black-and-white tiles.
  */
void draw_floor()
{
  int   i,  j;

  for(i=0;i<10;i++)
     for(j=0;j<10;j++){
        if((i+j)%2==0) glColor3f(0.9, 0.9, 0.9);
        else glColor3f(0.1, 0.1, 0.1);
        glBegin(GL_POLYGON);
        glVertex3f(i, 0.0, j);
        glVertex3f(i, 0.0, j+1);
        glVertex3f(i+1, 0.0, j+1);
        glVertex3f(i+1, 0.0, j);
        glEnd();
     }
}


/*------------------------------------------------------------------
 * Procedure to draw a sphere.
 */
void draw_sphere(void)
{
   if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }
  gluSphere(sphere, 1.0,   /* radius=1.5 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 12 stacks */
}

/*------------------------------------------------------------------
 * Procedure to draw a cylinder.
 */
void draw_cylinder(void)
{
   if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
  }
   /*--- Draw a cylinder ---*/
  gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
	      7.0,              /* height of the cylinder */
                      12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */

}


/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(6.0, 6.0, 6.0, 3.0, 0.0, 3.0, 0.0, 1.0, 0.0);

  /*-------Draw the floor ----*/
  
  draw_floor();

  /*---- Draw other parts. ---*/

  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  glFlush(); /*--Display the results----*/
}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{


  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(w>h)
    glOrtho(-10.0, 10.0, -10.0*(float)h/(float)w, 10.0*(float)h/(float)w, 
             0.50, 30.0);
  else
    glOrtho(-10.0*(float)w/(float)h, 10.0*(float)w/(float)h, -10.0, 10.0, 
            0.50, 30.0);
  width = w; height = h;
}


/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int x, int y)
{
  if(key=='Q'||key=='q') exit(0);
  if(key=='Z') pos[2] += 0.5;
  else if(key=='z') pos[2] -= 0.5;
  if(key=='x') pos[0] -= 0.5;  
  else if(key=='X') pos[0] += 0.5;
   else if(key=='R'||key=='r') angle += 3.0;
  else if(key=='L'||key=='l') angle -= 3.0;
  if(angle >= 360.0) angle -= 360.0;
  else if(angle < 0.0) angle += 360.0;
 
  display();
}




/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutCreateWindow("cube");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);

  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_quit);

  glutMainLoop();
}

