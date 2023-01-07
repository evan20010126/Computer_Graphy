/*-----------------------------------------------------------
 * An example program to draw a car
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#define  PI   3.141592653

#define Step  0.5

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int    face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
float  colors[6][3]={{0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
                     {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7}};
/* indices of the box faces */
int    cube[6] = {0, 1, 2, 3, 4, 5};

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj  *sphere=NULL, *cylind=NULL, *disk;


/*-Declare car position, orientation--*/
float  self_ang=0.0, glob_ang=0.0;
float  position[3]={8.0, 0.0, 0.0};

/*-----Define window size----*/
int width=512, height=512;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  /*---Create quadratic objects---*/
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }
  if(disk==NULL){
    disk = gluNewQuadric();
    gluQuadricDrawStyle(disk, GLU_FILL);
    gluQuadricNormals(disk, GLU_SMOOTH);
  }
}


/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_cube()
{
  int    i;

  for(i=0;i<6;i++){
    glColor3fv(colors[i]);  /* Set color */
    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}


/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{
  int  i, j;

  for(i=0;i<10;i++)
    for(j=0;j<10;j++){
      if((i+j)%2==0) glColor3f(1.0, 0.8, 0.8);
      else glColor3f(0.1, 0.1, 0.7);
      glBegin(GL_POLYGON);
        glVertex3f((i-5.0)*10.0, -2.5, (j-5.0)*10.0);
        glVertex3f((i-5.0)*10.0, -2.5, (j-4.0)*10.0);
        glVertex3f((i-4.0)*10.0, -2.5, (j-4.0)*10.0);
        glVertex3f((i-4.0)*10.0, -2.5, (j-5.0)*10.0);
      glEnd();
    }
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{

  /*----Draw a white sphere to represent the original-----*/
  glColor3f(0.9, 0.9, 0.9);

  gluSphere(sphere, 2.0,   /* radius=2.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
  glColor3f(0.0, 0.4, 0.0);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  
  /* Draw Y axis */
  glPushMatrix();
  glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
  glColor3f(0.95, 0.0, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  glPopMatrix();

  /* Draw X axis */
  glColor3f(0.95, 0.95, 0.0);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
  gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  glPopMatrix();
  /*-- Restore the original modelview matrix --*/
  glPopMatrix();
}


/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  static float  ang_self=0.0;  /*Define the angle of self-rotate */
  static float  angle=0.0;

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(8.0, 5.0, 15.0, 4.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  draw_floor();

  draw_axes();
  /*-------Draw the car body which is a cube----*/
  glTranslatef(position[0], 0.0, position[2]); 
  glRotatef(self_ang, 0.0, 1.0, 0.0);

  glPushMatrix();              /* Save M1 coord. sys */
  glScalef(17.0, 3.0, 6.0);    /* Scale up the axes */
  draw_cube();
  glPopMatrix();               /* Get M1 back */

  /*-------Draw the front wheels -----*/
  glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(-6.0, 0.0, 4.0); /* Go to left wheel position */
  glutSolidTorus(1.0,  /* inner radius */
                 2.0,  /* outer radius */
                 24,   /* divided into 18 segments */
                 12);  /* 12 rings */
  glPopMatrix();

  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(-6.0, 0.0, -4.0);/* Go to right wheel position */
  glutSolidTorus(1.0,  /* inner radius */
                 2.0,  /* outer radius */
                 24,   /* divided into 18 segments */
                 12);  /* 12 rings */
  glPopMatrix();

  /*------Draw back wheels ----*/
  glColor3f(1.0, 0.4, 0.0);
  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(6.0, 0.0, 4.0); /* Go to left wheel position */
  glutSolidTorus(1.0,  /* inner radius */
                 2.0,  /* outer radius */
                 24,   /* divided into 18 segments */
                 12);  /* 12 rings */
  glPopMatrix();

  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(6.0, 0.0, -4.0);/* Go to right wheel position */
  glutSolidTorus(1.0,  /* inner radius */
                 2.0,  /* outer radius */
                 24,   /* divided into 18 segments */
                 12);  /* 12 rings */
  glPopMatrix();

  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  return;
}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w>h)
    glOrtho(-40.0, 40.0, -40.0*(float)h/(float)w, 40.0*(float)h/(float)w, 
             -100.0, 100.0);
  else
    glOrtho(-40.0*(float)w/(float)h, 40.0*(float)w/(float)h, -40.0, 40.0, 
            -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int x, int y)
{
  if(key=='Q'||key=='q') exit(0);
  if(key=='f'){
    position[0] += Step*cos(self_ang*PI/180.0);
    position[2] -= Step*sin(self_ang*PI/180.0);
  }else if(key=='b'){
    position[0] -= Step*cos(self_ang*PI/180.0);
    position[2] += Step*sin(self_ang*PI/180.0);
  }else if(key=='r'){
    self_ang += 10.0;
  }else if(key=='c') self_ang -= 10.0;
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

  glutInitWindowSize(width, height);
  glutCreateWindow("windmill");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  /*  glutIdleFunc(display); */
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_quit);

  glutMainLoop();
}

