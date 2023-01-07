/***************************************************
 * Example program: Draw a moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>


#define   PI   3.1415927
//Define polygon drwaing models, filled or outlined.
#define   FILL   1
#define   LINE  2   
//coordinates of the 8 corners.(0,0,0)-(1,1,1)
float  points[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
//Define 6 faces using the 8 corners (vertices)
int    face[][4] = {{0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1}, 
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4}};
//Define the cube using the 6 faces.
int    cube[6] = {0, 1, 2, 3, 4, 5};
//Define colors
float  color[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
{1.0, 0.0, 1.0},{0.0, 1.0, 1.0}};

 /* window shape */
int    width=500, height=500;   
//define a base position in the z-x plane
float  pos[3] = {0.0, 0.0, 0.0};
//declare the rotational angle.
float  angle = 0.0;

//Quodri objects for drawing the world coordinate system.
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj  *sphere=NULL, *cylind=NULL;
int    polygonMode=FILL;


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
void draw_cube()
{
  int    i;


  glPolygonMode(GL_FRONT, GL_FILL);
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

/*------------------------------------------------------------
 * Procedure to draw a floor of black-and-white tiles.
  */
void draw_floor()
{
	int   i,  j;

	if(polygonMode==LINE)
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(i=0;i<10;i++)
		for(j=0;j<10;j++){
			if((i+j)%2==0) glColor3f(0.9, 0.9, 0.9);
			else glColor3f(0.2, 0.2, 0.2);
			glBegin(GL_POLYGON);
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j+1);
			glVertex3f(i+1, 0.0, j+1);
			glVertex3f(i+1, 0.0, j);
			glEnd();
		}
}


/*------------------------------------------------------------------
 * Procedure to draw a cylinder.
 */
void draw_cylinder(void)
{
   if(cylind==NULL){
    cylind = gluNewQuadric();
   }
   /*--- Draw a cylinder ---*/

    gluCylinder(cylind, 0.3, 0.3, /* radius of top and bottom circle */
	                  4.0,              /* height of the cylinder */
                      12,               /* use 12-side polygon approximating circle*/
	                    4);               /* Divide it into 3 sections */

}

/*----------------------------------------------------------------------
 * Procedure to draw a coordinate system, RGB bars = XYZ axes
 * White ball = origin.
 */
void draw_coord_sys(void)
{
	
	//draw coord. sys. in line or fill mode.
	if(polygonMode==LINE)
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Draw a sphere to show the origin
     if(sphere==NULL){
       sphere = gluNewQuadric();
     }
	 glColor3f(1.0, 1.0, 1.0);
     gluSphere(sphere, 0.5,   /* radius=0.5 */
	                              12,            /* composing of 12 slices*/
	                              12);           /* composing of 12 stacks */
	 //Draw the x-axis in RED.
	 glColor3f(1.0, 0.0, 0.0);
     glPushMatrix();
     glRotatef(90.0, 0, 1, 0);   // The x-axis
     draw_cylinder();
     glPopMatrix();
	 //Draw the Y-axis in GREEN.
	 glColor3f(0.0, 1.0, 0.0);
     glPushMatrix();  //The y-axis
     glRotatef(-90.0, 1, 0, 0);   
     draw_cylinder();
     glPopMatrix();	 
	 //Draw the z-axis in BLUE.
     glColor3f(0.0, 0.0, 1.0);
     draw_cylinder();   //The z-axis
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

  /*-------Draw the floor & coordinate system----*/
  draw_floor();
  draw_coord_sys();
  /*-------Draw the 1st cube ----*/
  //move to the base position
  glTranslatef(pos[0],pos[1], pos[2]);

  glPushMatrix();
 // glScalef(1.0, 4.0, 1.0);
  draw_cube();
  glPopMatrix();
  glutSwapBuffers();
  return;

  /*-------Draw another cube---*/
  glPushMatrix();
  glTranslatef(0.0, 0.0, 3.0);
  glTranslatef(0.5, 0.50, 2.0);
  glRotatef(angle, 0.0, 1.0, 0.0);
  glTranslatef(-0.5, -0.50, -2.0);
  glScalef(1.0, 1.0, 4.0);
  draw_cube();
  glPopMatrix();
  /*-------Draw the third cube ---*/
 
  glTranslatef(3.0, 0.0, 0.0);
  glScalef(4.0, 1.0, 1.0);
  glTranslatef(0.5, 0.5, 0.5);
  glRotatef(angle, 1.0, 0.0, 0.0);
  glTranslatef(-0.5, -0.5, -0.5);
  draw_cube();


  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
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
  if(key=='Z'){
	  if(pos[2]<9.0) pos[2] += 1.0;
  }else if(key=='z'){
	  if(pos[2]>0.0) pos[2] -= 1.0;
  }
  if(key=='x'){
	if(pos[0]>0.0) pos[0] -= 1.0;  
  }else if(key=='X' && pos[0]<9.0) pos[0] += 1.0;
   else if(key=='R'||key=='r') angle += 3.0;
  else if(key=='L'||key=='l') angle -= 3.0;
  if(key=='1') polygonMode = FILL;
  else if(key=='2') polygonMode = LINE;

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

