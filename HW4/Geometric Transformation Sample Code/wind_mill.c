/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <time.h>
#define Step 0.2
#define ANGLE 1.0
#define PI 3.14159265359

/*-----Define window size----*/
int width = 512, height = 512;

int control_light_num = 0;
int sun_auto = 0;
int sun_auto_color = 0;
int redball_auto = 0;
int redball_auto_color = 0;
int spot_auto = 0;
int spot_auto_color = 0;
int turn_on_off_light[3] = {0, 0, 0};
int modify_state = 0;
float light_color[3][4] = {{0.8, 0.4, 0.4, 1.0}, {0.7, 0.7, 0.0, 1.0}, {0.0, 0.4, 0.8, 1.0}}; //第幾盞的R or G or B (都是控制diffuse)
/*----Define light source properties -------*/
float lit_position[] = {10, -4.0, 4.0, 1.0};
float lit_direction[] = {-1.0, -1.0, 1.0, 0.0};
float lit_diffuse[] = {0.8, 0.4, 0.4, 1.0};
float lit_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit_cutoff = 60.0;
float lit_exponent = 8.0;
float lit0_angle = 30;

//..lit1為sun(directional light)
float lit1_position[] = {20, 20.0, -2.0, 1.0};
float lit1_direction[] = {-1.0, -1.0, 1.0, 0.0};
float lit1_diffuse[] = {0.7, 0.7, 0.0, 1.0};
float lit1_specular[] = {0.7, 0.7, 0.7, 1.0};
float rotate_dirction_lit1 = 0;

//..lit2為探照燈(spot light)
float lit2_position[] = {0, 0, 0, 1.0};
float lit2_direction[] = {-1, 0, 0, 0};
float lit2_diffuse[] = {0.0, 0.4, 0.8, 1.0};
float lit2_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit2_cutoff = 50.0;
float lit2_exponent = 8.0;
float rotate_dirction_lit2 = 0;

float flr_diffuse[] = {0.4, 0.4, 0.4, 1};
float flr_specular[] = {0.0, 0.0, 0.0, 1};
float flr_ambient[] = {0.25, 0.25, 0.25, 1};
float body_diffuse[] = {0.50754, 0.50754, 0.50754};
float body_specular[] = {0.508273, 0.508273, 0.508273};
float body_ambient[] = {0.19225, 0.19225, 0.19225};
float body_shininess = 0.4;

float global_ambient[] = {0.2, 0.2, 0.2, 1.0};
void draw_plastic(char);

// projection & view
#define Orthographical_x 0
#define Orthographical_y 1
#define Orthographical_z 2
#define Perspective 3
#define show_All 4

int show_case = show_All;

float eye[3] = {10.0, 5.0, 15.0};
float delta[3] = {0.0, 0.0, 0.0};
float theta[3] = {0.0, 0.0, 0.0};
float zoom = 2.5;

void draw_view_volumn()
{
  float far_plane_length = (50.0 * zoom) / 2.5;
  float zoom_points[][3] = {{-zoom, -zoom, -2.5}, {zoom, -zoom, -2.5}, {zoom, zoom, -2.5}, {-zoom, zoom, -2.5}, {-far_plane_length, -far_plane_length, -50.0}, {far_plane_length, -far_plane_length, -50.0}, {far_plane_length, far_plane_length, -50.0}, {-far_plane_length, far_plane_length, -50.0}};
  int zoom_face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(1);
  glColor3f(0.2, 0.2, 0.2);
  glPushMatrix();
  // glTranslatef(eye[0], eye[1], eye[2]);
  // glTranslatef(delta[0], delta[1], delta[2]);
  // glRotated(180.0, 1, 0, 0);
  // glRotatef(-theta[0], 1.0, 0.0, 0.0);
  // glRotatef(theta[1], 0.0, 1.0, 0.0);
  // glRotatef(-theta[2], 0.0, 0.0, 1.0);

  // glTranslated(-delta[0], -delta[1], -delta[2]);
  // glRotatef(theta[0], 1.0, 0.0, 0.0);
  // glRotatef(-theta[1], 0.0, 1.0, 0.0);
  // glRotatef(-theta[2], 0.0, 0.0, 1.0);
  // glTranslatef(eye[0], eye[1], eye[2]);
  glTranslated(eye[0], eye[1], eye[2]);
  glTranslated(-delta[0], -delta[1], -delta[2]);
  glRotatef(theta[0], -1.0, 0.0, 0.0);
  glRotatef(theta[1], 0.0, -1.0, 0.0);
  glRotatef(theta[2], 0.0, 0.0, 1.0);

  for (i = 0; i < 6; i++)
  {
    glBegin(GL_POLYGON); /* Draw the face */
    glVertex3fv(zoom_points[zoom_face[i][0]]);
    glVertex3fv(zoom_points[zoom_face[i][1]]);
    glVertex3fv(zoom_points[zoom_face[i][2]]);
    glVertex3fv(zoom_points[zoom_face[i][3]]);
    glEnd();
  }
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void my_view(int P_style)
{
  // glMatrixMode(GL_MODELVIEW);
  switch (P_style)
  {
  case Orthographical_x:
    gluLookAt(15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
    draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  case Orthographical_y:
    gluLookAt(0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0); // WHY?
    draw_view_volumn();                                      //轉到對的位子後要先畫出來
    break;
  case Orthographical_z:
    gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
    draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  case Perspective:
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, -1.0);
    glTranslated(delta[0], delta[1], delta[2]);
    gluLookAt(eye[0], eye[1], eye[2], 10.0, 5.0, -10.0, 0.0, 1.0, 0.0);
    ////我寫到這
    // draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  }
}
void make_projection(int x)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (x == Perspective)
  {
    glFrustum(-zoom, zoom, -zoom, zoom, 2.5, 50.0); //(near左邊, near右邊, near下面, near上面, near距離, far距離)
    // gluPerspective(90.0, (double) width/ (double) height, 1.5, 50.0);
  }
  else
  {
    if (width > height)
      glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
              40.0 * (float)height / (float)width,
              -0.0, 100.0);
    else
      glOrtho(-40.0 * (float)width / (float)height,
              40.0 * (float)width / (float)height, -40.0, 40.0,
              -0.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
}

//

int state = 0;
// state = 1 向前傾
int count = 0;
int is_line = 0;

/*-----Define a unit box--------*/
/* Vertices of the box */
float points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
/* indices of the box faces */
float points_ass[][3] = {{3.0, -1.5, -1.5}, {3.0, -1.5, 1.5}, {3.0, 1.5, 1.5}, {3.0, 1.5, -1.5}, {5.0, -0.75, -0.75}, {5.0, -0.75, 0.75}, {5.0, 0.75, 0.75}, {5.0, 0.75, -0.75}};
int face_ass[][4] = {{0, 1, 2, 3}, {0, 1, 5, 4}, {0, 4, 7, 3}, {1, 5, 6, 2}, {3, 7, 6, 2}, {4, 5, 6, 7}};
int cube[6] = {0, 1, 2, 3, 4, 5};

float points_twelve[][3] = {{1, 0, 0}, {0.866, 0, 0.5}, {0.5, 0, 0.866}, {0, 0, 1}, {-0.5, 0, 0.866}, {-0.866, 0, 0.5}, {-1, 0, 0}, {-0.866, 0, -0.5}, {-0.5, 0, -0.866}, {0, 0, -1}, {0.5, 0, -0.866}, {0.866, 0, -0.5}, {1, 1, 0}, {0.866, 1, 0.5}, {0.5, 1, 0.866}, {0, 1, 1}, {-0.5, 1, 0.866}, {-0.866, 1, 0.5}, {-1, 1, 0}, {-0.866, 1, -0.5}, {-0.5, 1, -0.866}, {0, 1, -1}, {0.5, 1, -0.866}, {0.866, 1, -0.5}};
float mo_angle = 0.0;

float normal[][4] = {{0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}};

/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj *sphere = NULL, *sphere_ass = NULL;
GLUquadricObj *cylind = NULL, *driverWindow = NULL;
GLUquadricObj *airplane_ass1 = NULL, *airplane_ass2 = NULL;
GLUquadricObj *leg1 = NULL, *leg2 = NULL, *leg3 = NULL;
GLUquadricObj *wheel_A_cylinder = NULL, *wheel_B_cylinder = NULL, *wheel_C_cylinder = NULL;
GLUquadricObj *wheel_A_cycle_1 = NULL, *wheel_B_cycle_1 = NULL, *wheel_C_cycle_1 = NULL;
GLUquadricObj *wheel_A_cycle_2 = NULL, *wheel_B_cycle_2 = NULL, *wheel_C_cycle_2 = NULL;

GLUquadricObj *mo_arm1 = NULL, *mo_arm2 = NULL, *mo_arm3 = NULL, *mo_arm4 = NULL;

GLUquadricObj *world_coordinate_origin = NULL;
GLUquadricObj *world_axes[3] = {NULL};

// Global variables recording rotation angles of H-arm and blades.
float arm_ang = 45.0, bld_ang = 10.0;
float leg_len = 2.0;

//飛機最中心座標
float position[3] = {10.0, 0.0, 6.0};
//飛機頭朝向
float head_forward = 0.0f;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void draw_floor()
{
  int i, j;

  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++)
    {

      // if ((i + j) % 2 == 0)
      //   glColor3f(64.0 / 255, 224.0 / 255, 208.0 / 255);
      // else
      //   glColor3f(224.0 / 255, 255.0  / 255, 255 / 255);
      glPushMatrix();
      glTranslated((i - 5.0) * 10.0, -4.5, (j - 5.0) * 10.0);
      glutSolidSphere(0.4, 12, 12);
      glPopMatrix();

      glNormal3f(0.0, 1.0, 0.0);
      glBegin(GL_POLYGON);
      glVertex3f((i - 5.0) * 10.0, -4.5, (j - 5.0) * 10.0);
      glVertex3f((i - 5.0) * 10.0, -4.5, (j - 4.0) * 10.0);
      glVertex3f((i - 4.0) * 10.0, -4.5, (j - 4.0) * 10.0);
      glVertex3f((i - 4.0) * 10.0, -4.5, (j - 5.0) * 10.0);
      glEnd();
    }
}
void myinit()
{
  printf("light0 is point light (red ball)\n");
  printf("light1 is directional light (sun)\n");
  printf("light2 is spot light (helicopter's light)\n");

  printf("control light %d\n", control_light_num);

  glClearColor(0.0, 0.0, 0.0, 1.0); /*set the background color BLACK */
                                    /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

  // glViewport(0, 0, width, height);

  /*-----Set a parallel projection mode-----*/
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // glOrtho(-15.0, 15.0 , -15.0, 15.0, -10.5, 30.0);

  // copy paste
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST); /*Enable depth buffer for shading computing */
  glEnable(GL_NORMALIZE);  /*Enable mornalization  */

  glEnable(GL_LIGHTING); /*Enable lighting effects */
  glEnable(GL_LIGHT0);   /*Turn on light0 */
  glEnable(GL_LIGHT1);   /*Turn on light1 */
  glEnable(GL_LIGHT2);
  /*-----Define light0 ---------*/
  // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

  /*-----Define light1 ---------*/
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);

  /*----Define light2 -----*/
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);

  /*-----Define some global lighting status -----*/
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    /* local viewer */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

  ///*-----Enable face culling -----*/
  // glCullFace(GL_BACK);
  // glEnable(GL_CULL_FACE);

  glFlush(); /*Enforce window system display the results*/
}

/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
  glBegin(GL_POLYGON);
  // glNormal3fv(normal[4]);
  // glVertex3f(0.0, 0.0, 0.0);
  // glVertex3f(1.0, 4.0, 0.0);
  // glVertex3f(1.0, 8.0, 0.0);
  // glVertex3f(-1.0, 8.0, 0.0);
  // glVertex3f(-1.0, 4.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 4.0, 0.0);
  glVertex3f(1.0, 13.0, 0.0);
  glVertex3f(-1.0, 13.0, 0.0);
  glVertex3f(-1.0, 4.0, 0.0);
  glEnd();
}

void draw_ass1()
{
  int i;

  // glColor3f(1.0, 1.0, 40.0 / 255.0);
  for (i = 0; i < 6; i++)
  {
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse); /*diffuse color */
    // glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
    // glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
    // glMaterialf(GL_FRONT, GL_SHININESS, body_shininess);
    // glNormal3fv(normal[i]);
    glBegin(GL_POLYGON); /* Draw the face */
    glVertex3fv(points_ass[face_ass[i][0]]);
    glVertex3fv(points_ass[face_ass[i][1]]);
    glVertex3fv(points_ass[face_ass[i][2]]);
    glVertex3fv(points_ass[face_ass[i][3]]);
    glEnd();
  }
}

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
  int i;
  // glColor3f(1.0, 1.0, 102.0 / 255.0);
  for (i = 0; i < 6; i++)
  {
    glBegin(GL_POLYGON);                              /* Draw the face */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse); /*diffuse color */
    glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, body_shininess);
    glNormal3fv(normal[i]);
    glVertex3fv(points[face[i][0]]);
    glVertex3fv(points[face[i][1]]);
    glVertex3fv(points[face[i][2]]);
    glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

// plastic attribute
float cyan_plastic_diffuse[] = {0.0, 0.1, 0.06};
float cyan_plastic_specular[] = {0.0, 0.50980392, 0.50980392};
float cyan_plastic_ambient[] = {0.50196078, 0.50196078, 0.50196078};
float cyan_plastic_shininess = 0.25;

float green_plastic_diffuse[] = {0.0, 0.0, 0.0};
float green_plastic_specular[] = {0.1, 0.35, 0.1};
float green_plastic_ambient[] = {0.45, 0.55, 0.45};
float green_plastic_shininess = 0.25;

float red_plastic_diffuse[] = {0.0, 0.0, 0.0};
float red_plastic_specular[] = {0.5, 0.0, 0.0};
float red_plastic_ambient[] = {0.7, 0.6, 0.6};
float red_plastic_shininess = 0.25;

void draw_plastic(char _color)
{
  float temp_diffuse[3];
  float temp_specular[3];
  float temp_ambient[3];
  float temp_shininess;
  switch (_color)
  {
  case 'c':
    for (int i = 0; i < 3; i++)
    {
      temp_diffuse[i] = cyan_plastic_diffuse[i];
      temp_specular[i] = cyan_plastic_specular[i];
      temp_ambient[i] = cyan_plastic_ambient[i];
    }
    temp_shininess = cyan_plastic_shininess;
    break;
  case 'g':
    for (int i = 0; i < 3; i++)
    {
      temp_diffuse[i] = green_plastic_diffuse[i];
      temp_specular[i] = green_plastic_specular[i];
      temp_ambient[i] = green_plastic_ambient[i];
    }
    temp_shininess = green_plastic_shininess;
    break;
  case 'r':
    for (int i = 0; i < 3; i++)
    {
      temp_diffuse[i] = red_plastic_diffuse[i];
      temp_specular[i] = red_plastic_specular[i];
      temp_ambient[i] = red_plastic_ambient[i];
    }
    temp_shininess = red_plastic_shininess;
    break;
  }
  glMaterialfv(GL_FRONT, GL_DIFFUSE, temp_diffuse); /*diffuse color */
  glMaterialfv(GL_FRONT, GL_SPECULAR, temp_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, temp_ambient);
  glMaterialf(GL_FRONT, GL_SHININESS, temp_shininess);
}

void draw_twelve()
{
  int i;
  draw_plastic('r');
  glBegin(GL_POLYGON); /* Draw the face */
  for (i = 0; i < 12; i++)
  {
    // if (i % 2 == 0 && is_line == 1)
    // {
    //   glColor3f(0.2, 0.3, 0.1);
    // }
    // else if (i % 2 == 1 && is_line == 1)
    // {
    //   glColor3f(0.8, 0.8, 0.3);
    // }
    glVertex3fv(points_twelve[i]);
  }
  glEnd();

  draw_plastic('g');
  for (i = 0; i <= 10; i++)
  {
    // if (i % 2 == 0 && is_line == 1)
    // {
    //   glColor3f(0.2, 0.3, 0.1);
    // }
    // else if (i % 2 == 1 && is_line == 1)
    // {
    //   glColor3f(0.8, 0.8, 0.3);
    // }
    glBegin(GL_POLYGON); /* Draw the face */
    glVertex3fv(points_twelve[0 + i]);
    glVertex3fv(points_twelve[12 + i]);
    glVertex3fv(points_twelve[13 + i]);
    glVertex3fv(points_twelve[1 + i]);
    glEnd();
  }
  // if (i % 2 == 0 && is_line == 1)
  // {
  //   glColor3f(0.2, 0.3, 0.1);
  // }
  // else if (i % 2 == 1 && is_line == 1)
  // {
  //   glColor3f(0.8, 0.8, 0.3);
  // }
  glBegin(GL_POLYGON); /* Draw the face */
  glVertex3fv(points_twelve[11]);
  glVertex3fv(points_twelve[23]);
  glVertex3fv(points_twelve[12]);
  glVertex3fv(points_twelve[0]);
  glEnd();

  draw_plastic('r');
  glBegin(GL_POLYGON); /* Draw the face */
  // if (i % 2 == 0 && is_line == 1)
  // {
  //   glColor3f(0.2, 0.3, 0.1);
  // }
  // else if (i % 2 == 1 && is_line == 1)
  // {
  //   glColor3f(0.8, 0.8, 0.3);
  // }
  for (i = 12; i < 23; i++)
  {
    glVertex3fv(points_twelve[i]);
  }
  glEnd();
}

void draw_window()
{
  ////////////////////////for lit1 (is directional light)/////////////////////////////////////////////////////////////////
  glPushMatrix();
  /*----position light1 (fixed) in eye coord sys ---*/
  glPushMatrix();
  glRotatef(rotate_dirction_lit1, 0, 1, 0);
  glLightfv(GL_LIGHT1, GL_POSITION, lit1_direction); /*fixed position in eye space---*/
  glPopMatrix();
  /*---draw the light source ---*/
  glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
  if (turn_on_off_light[1] % 2 == 0)
  {
    glDisable(GL_LIGHTING); /* Turn on lighting */
  }
  // glDisable(GL_CULL_FACE);
  glColor3f(light_color[1][0], light_color[1][1], light_color[1][2]);
  glutSolidSphere(3, 12, 12);
  glPopMatrix();
  // glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);

  ///////////////////////for lit0 (is a point light)/////////////////////////////////////////////////////////////////////
  /*-------Rotate light position----*/
  if (turn_on_off_light[0] % 2 == 0)
  {
    glDisable(GL_LIGHTING); /* Turn on lighting */
  }
  glPushMatrix();
  glTranslatef(8.0, 10.0, 8.0);         /* Move to [8, 0, 8] */
  glRotatef(lit0_angle, 0.0, 1.0, 0.0); /* Rotate about y-axis */
  /*-------Draw light source as a red cube----*/
  glPushMatrix();
  glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
  glColor3f(light_color[0][0], light_color[0][1], light_color[0][2]);
  glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
  // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
  glutWireSphere(0.5, 8, 8);
  glPopMatrix();

  /*----Redefine position and direction of light0-----*/
  glPopMatrix();
  glEnable(GL_LIGHTING); /* Turn on lighting */
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // World coordinate system origin and 3 axes.

  if (world_coordinate_origin == NULL)
  {
    world_coordinate_origin = gluNewQuadric();
    gluQuadricDrawStyle(world_coordinate_origin, GLU_FILL);
  }
  if (world_axes[0] == NULL)
  {
    world_axes[0] = gluNewQuadric();
    gluQuadricDrawStyle(world_axes[0], GL_FILL);
  }
  if (world_axes[1] == NULL)
  {
    world_axes[1] = gluNewQuadric();
    gluQuadricDrawStyle(world_axes[1], GL_FILL);
  }
  if (world_axes[2] == NULL)
  {
    world_axes[2] = gluNewQuadric();
    gluQuadricDrawStyle(world_axes[2], GL_FILL);
  }

  gluSphere(world_coordinate_origin, 0.5, /* radius=2.0 */
            12,                           /* composing of 12 slices*/
            12);                          /* composing of 12 stacks */
  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
  draw_plastic('c');
  gluCylinder(world_axes[0], 0.25, 0.25, /* radius of top and bottom circle */
              10.0,                      /* height of the cylinder */
              12,                        /* use 12-side polygon approximating circle*/
              3);                        /* Divide it into 3 sections */
  /* Draw Y axis */
  glPushMatrix();
  draw_plastic('g');
  glRotatef(-90.0, 1.0, 0.0, 0.0); /*Rotate about x by -90', z becomes y */
  glColor3f(0.95, 0.0, 0.95);
  gluCylinder(world_axes[1], 0.25, 0.25, /* radius of top and bottom circle */
              10.0,                      /* height of the cylinder */
              12,                        /* use 12-side polygon approximating circle*/
              3);                        /* Divide it into 3 sections */
  glPopMatrix();

  /* Draw X axis */
  draw_plastic('r');
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0);        /*Rotate about y  by 90', x becomes z */
  gluCylinder(world_axes[2], 0.25, 0.25, /* radius of top and bottom circle */
              10.0,                      /* height of the cylinder */
              12,                        /* use 12-side polygon approximating circle*/
              3);                        /* Divide it into 3 sections */
  glPopMatrix();

  // gluLookAt(10.0, 5.0, 15.0, 10.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /*-------Draw the base of the windmill which is a cube----*/

  glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse); /*diffuse color */
  glMaterialfv(GL_FRONT, GL_SPECULAR, flr_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
  draw_floor();

  glRotatef(30.0, 0.0, 1.0, 0.0);

  glPushMatrix();
  glTranslatef(1, 5, -5);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  draw_plastic('c');
  if (mo_arm1 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm1 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm1, GLU_FILL);
    gluQuadricNormals(mo_arm1, GLU_SMOOTH);
  }
  if (mo_arm2 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm2 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm2, GLU_FILL);
    gluQuadricNormals(mo_arm2, GLU_SMOOTH);
  }
  if (mo_arm3 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm3 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm3, GLU_FILL);
    gluQuadricNormals(mo_arm3, GLU_SMOOTH);
  }
  if (mo_arm4 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm4 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm4, GLU_FILL);
    gluQuadricNormals(mo_arm4, GLU_SMOOTH);
  }
  glColor3f(0.5, 0.5, 0.5);
  glPushMatrix();
  glTranslatef(0.0, 3, 0);
  glRotatef(-20.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm1, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glRotatef(40.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm2, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glPopMatrix();
  glPushMatrix();
  // glTranslatef(0.0,3,0);
  glRotatef(-20.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm3, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glRotatef(40.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm4, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glPopMatrix();
  glRotatef(mo_angle, 0.0, 1.0, 0.0);
  glScalef(5.0, 3.0, 5.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(5);
  is_line = 1;
  draw_twelve();
  is_line = 0;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(1, 0, 0);
  draw_twelve();
  glPopMatrix();

  glTranslatef(position[0], position[1], position[2]);
  glRotatef(head_forward, 0.0, 1.0, 0.0);

  if (state == 1)
  {
    glRotatef(10.0, 0.0, 0.0, 1.0);
    count++;
    if (count == 1500)
    {
      count = 0;
      state = 0;
    }
  }
  else if (state == 2)
  {
    glRotatef(-10.0, 0.0, 0.0, 1.0);
    count++;
    if (count == 1500)
    {
      count = 0;
      state = 0;
    }
  }
  else if (state == 3)
  {
    glRotatef(-10.0, -1.0 / sqrt(2), 0.0, -1.0 / sqrt(2));
    count++;
    if (count == 1500)
    {
      count = 0;
      state = 0;
    }
  }
  else if (state == 4)
  {
    glRotatef(10.0, -1.0 / sqrt(2), 0.0, 1.0 / sqrt(2));
    count++;
    if (count == 1500)
    {
      count = 0;
      state = 0;
    }
  }
  glPushMatrix();          /* Save M1 coord. sys */
  glScalef(6.0, 3.0, 3.0); /* Scale up the axes by 6 times */

  draw_cube();   /* Draw a unit cube in the new coord. sys. */
  glPopMatrix(); /* Get M1 back */

  //可伸縮的輪子
  glPushMatrix();
  glTranslatef(0.0, -1.5, 0.0);
  if (leg1 == NULL)
  { /* allocate a quadric object, if necessary */
    leg1 = gluNewQuadric();
    gluQuadricDrawStyle(leg1, GLU_FILL);
    gluQuadricNormals(leg1, GLU_SMOOTH);
  }
  if (leg2 == NULL)
  { /* allocate a quadric object, if necessary */
    leg2 = gluNewQuadric();
    gluQuadricDrawStyle(leg2, GLU_FILL);
    gluQuadricNormals(leg2, GLU_SMOOTH);
  }
  if (leg3 == NULL)
  { /* allocate a quadric object, if necessary */
    leg3 = gluNewQuadric();
    gluQuadricDrawStyle(leg3, GLU_FILL);
    gluQuadricNormals(leg3, GLU_SMOOTH);
  }

  glPushMatrix();
  glColor3f(30.0 / 255.0, 144.0 / 255.0, 1.0);
  glTranslatef(-2.0, 0.0, 0.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glRotatef(-45.0, 0.0, 1.0, 0.0);
  gluCylinder(leg1, 0.2, 0.2, /* radius of top and bottom circle */
              leg_len,        /* height of the cylinder */
              12,             /* use 12-side polygon approximating circle*/
              3);             /* Divide it into 3 sections */
  // draw_wheel輪子
  glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
  glTranslatef(0.0, -0.5, leg_len + 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  if (wheel_A_cylinder == NULL)
  { /* allocate a quadric object, if necessary */
    wheel_A_cylinder = gluNewQuadric();
    gluQuadricDrawStyle(wheel_A_cylinder, GLU_FILL);
    gluQuadricNormals(wheel_A_cylinder, GLU_SMOOTH);
  }
  gluCylinder(wheel_A_cylinder, 0.5, 0.5, /* radius of top and bottom circle */
              1.0,                        /* height of the cylinder */
              12,                         /* use 12-side polygon approximating circle*/
              3);                         /* Divide it into 3 sections */
  glTranslatef(0.0, 0.0, 1.0);
  if (wheel_A_cycle_1 == NULL)
  {
    wheel_A_cycle_1 = gluNewQuadric();
  }

  glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
  gluDisk(wheel_A_cycle_1,
          0,
          0.5,
          12,
          3);
  glTranslatef(0.0, 0.0, -1);
  if (wheel_A_cycle_2 == NULL)
  {
    wheel_A_cycle_2 = gluNewQuadric();
  }
  gluDisk(wheel_A_cycle_2,
          0,
          0.5,
          12,
          3);
  //
  glPopMatrix();
  glPushMatrix();
  glColor3f(30.0 / 255.0, 144.0 / 255.0, 1.0);
  glTranslatef(2.0, 0.0, 1.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glRotatef(45.0, 0.0, 1.0, 0.0);
  gluCylinder(leg2, 0.2, 0.2, /* radius of top and bottom circle */
              leg_len,        /* height of the cylinder */
              12,             /* use 12-side polygon approximating circle*/
              3);             /* Divide it into 3 sections */
  // draw_wheel輪子
  glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
  glTranslatef(0.0, -0.5, leg_len + 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  if (wheel_B_cylinder == NULL)
  { /* allocate a quadric object, if necessary */
    wheel_B_cylinder = gluNewQuadric();
    gluQuadricDrawStyle(wheel_B_cylinder, GLU_FILL);
    gluQuadricNormals(wheel_B_cylinder, GLU_SMOOTH);
  }
  gluCylinder(wheel_B_cylinder, 0.5, 0.5, /* radius of top and bottom circle */
              1.0,                        /* height of the cylinder */
              12,                         /* use 12-side polygon approximating circle*/
              3);                         /* Divide it into 3 sections */
  glTranslatef(0.0, 0.0, 1.0);
  if (wheel_B_cycle_1 == NULL)
  {
    wheel_B_cycle_1 = gluNewQuadric();
  }

  glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
  gluDisk(wheel_B_cycle_1,
          0,
          0.5,
          12,
          3);
  glTranslatef(0.0, 0.0, -1.0);
  if (wheel_B_cycle_2 == NULL)
  {
    wheel_B_cycle_2 = gluNewQuadric();
  }
  gluDisk(wheel_B_cycle_2,
          0,
          0.5,
          12,
          3);

  //
  glPopMatrix();
  glPushMatrix();
  glColor3f(30.0 / 255.0, 144.0 / 255.0, 1.0);
  glTranslatef(2.0, 0.0, -1.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glRotatef(45.0, 0.0, 1.0, 0.0);
  gluCylinder(leg3, 0.2, 0.2, /* radius of top and bottom circle */
              leg_len,        /* height of the cylinder */
              12,             /* use 12-side polygon approximating circle*/
              3);             /* Divide it into 3 sections */
  // draw_wheel輪子
  glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
  glTranslatef(0.0, -0.5, leg_len + 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  if (wheel_C_cylinder == NULL)
  { /* allocate a quadric object, if necessary */
    wheel_C_cylinder = gluNewQuadric();
    gluQuadricDrawStyle(wheel_C_cylinder, GLU_FILL);
    gluQuadricNormals(wheel_C_cylinder, GLU_SMOOTH);
  }
  gluCylinder(wheel_C_cylinder, 0.5, 0.5, /* radius of top and bottom circle */
              1.0,                        /* height of the cylinder */
              12,                         /* use 12-side polygon approximating circle*/
              3);                         /* Divide it into 3 sections */
  glTranslatef(0.0, 0.0, 1.0);
  if (wheel_C_cycle_1 == NULL)
  {
    wheel_C_cycle_1 = gluNewQuadric();
  }

  glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
  gluDisk(wheel_C_cycle_1,
          0,
          0.5,
          12,
          3);
  glTranslatef(0.0, 0.0, -1.0);
  if (wheel_C_cycle_2 == NULL)
  {
    wheel_C_cycle_2 = gluNewQuadric();
  }
  gluDisk(wheel_C_cycle_2,
          0,
          0.5,
          12,
          3);
  //
  glPopMatrix();
  glPopMatrix();

  // driverWindow<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  glColor3f(0.2, 0.2, 1.0); /* Gray colored */

  if (driverWindow == NULL)
  { /* allocate a quadric object, if necessary */
    driverWindow = gluNewQuadric();
    gluQuadricDrawStyle(driverWindow, GLU_FILL);
    gluQuadricNormals(driverWindow, GLU_SMOOTH);
  }

  glPushMatrix(); /* Save M2 system */
  glTranslatef(-3.0, 0.0, 0.0);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  /*--- Draw a cylinder ---*/
  gluCylinder(driverWindow, 1.5, 0.0, /* radius of top and bottom circle */
              1.5,                    /* height of the cylinder */
              12,                     /* use 12-side polygon approximating circle*/
              3);                     /* Divide it into 3 sections */
  glPopMatrix();                      /* Get M2 back */

  glPushMatrix();
  glTranslatef(-3.75, -0.75, 0.0);
  glPushMatrix();
  glScalef(1.5, 1.5, 3.0);
  draw_cube();
  glPopMatrix();
  ////
  /*-------Rotate light position----*/
  glPushMatrix();
  glTranslatef(-1.0, 0, 0.0); /* Move to [8, 0, 8] */
  // glRotatef(lit_angle, 0.0, 1.0, 0.0); /* Rotate about y-axis */
  /*-------Draw light source as a red cube----*/
  if (turn_on_off_light[2] % 2 == 0)
  {
    glDisable(GL_LIGHTING); /* Turn on lighting */
  }
  glPushMatrix();
  glTranslatef(lit2_position[0], lit2_position[1], lit2_position[2]);
  glColor3f(light_color[2][0], light_color[2][1], light_color[2][2]);
  glutWireSphere(0.5, 8, 8);
  glPopMatrix();

  glEnable(GL_LIGHTING); /* Turn on lighting */
  /*----Redefine position and direction of light0-----*/
  glPushMatrix();
  glRotatef(-rotate_dirction_lit2, 0, 0, 1);
  glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);
  glPopMatrix();
  glPopMatrix();
  ////
  glPopMatrix();
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // draw ass
  draw_ass1();
  if (airplane_ass1 == NULL)
  { /* allocate a quadric object, if necessary */
    airplane_ass1 = gluNewQuadric();
    gluQuadricDrawStyle(airplane_ass1, GLU_FILL);
    gluQuadricNormals(airplane_ass1, GLU_SMOOTH);
  }
  if (airplane_ass2 == NULL)
  { /* allocate a quadric object, if necessary */
    airplane_ass2 = gluNewQuadric();
    gluQuadricDrawStyle(airplane_ass2, GLU_FILL);
    gluQuadricNormals(airplane_ass2, GLU_SMOOTH);
  }
  glPushMatrix();
  glTranslatef(5.0, 0.0, 0.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  gluCylinder(airplane_ass1, 0.75, 0.0, /* radius of top and bottom circle */
              6.0,                      /* height of the cylinder */
              12,                       /* use 12-side polygon approximating circle*/
              3);                       /* Divide it into 3 sections */
  glTranslatef(0.0, 0.0, 5.0);
  gluCylinder(airplane_ass2, 0.0, 0.55, /* radius of top and bottom circle */
              2.0,                      /* height of the cylinder */
              12,                       /* use 12-side polygon approximating circle*/
              3);                       /* Divide it into 3 sections */
  //小電風扇
  glTranslatef(-0.5, 0.0, 0.0);
  if (sphere_ass == NULL)
  {
    sphere_ass = gluNewQuadric();
    gluQuadricDrawStyle(sphere_ass, GLU_FILL);
  }
  glColor3f(0.95, 0.2, 0.2);
  gluSphere(sphere_ass, 0.5, /* radius=2.0 */
            12,              /* composing of 12 slices*/
            12);             /* composing of 12 stacks */
  glColor3f(1.0, 1.0, 1.0);

  glRotatef(90, 0.0, 1.0, 0.0);
  if (position[1] > 0)
  {
    glRotatef(bld_ang, 0.0, 0.0, 1.0); /* Rotate about x axis, M5 coord. sys. */
  }
  glScalef(0.15, 0.15, 0.15);
  glNormal3fv(normal[5]);
  draw_blade(); /* draw the first blade */ //為往y軸畫

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree, M6 coord. sys */
  draw_blade();                   /* draw 2nd blade */

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree,M7 coord. sys */
  draw_blade();                   /* draw 3rd blade */

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree,M7 coord. sys */
  draw_blade();                   /* draw 3rd blade */

  glPopMatrix();
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  glTranslatef(0.0, 1.5, 0.0); /* Go up by 2 units, move to M2 coord. sys */

  /*-------Draw the cylinder, arm of the windmill------*/
  glColor3f(0.68, 0.68, 0.68); /* Gray colored */

  if (cylind == NULL)
  { /* allocate a quadric object, if necessary */
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }

  glPushMatrix();                  /* Save M2 system */
  glRotatef(-90.0, 1.0, 0.0, 0.0); /* Roate about x axis, z become y,
                                      and y become -z */
  /*--- Draw a cylinder ---*/
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
              3.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);               /* Divide it into 3 sections */
  glPopMatrix();                /* Get M2 back */

  glTranslatef(0.0, 4.0, 0.0); /* Go up 8 units, become M3 coord. sys*/

  /*----- Draw a unit sphere ---*/
  if (sphere == NULL)
  {
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }
  glColor3f(0.95, 0.2, 0.2);
  gluSphere(sphere, 1.0, /* radius=2.0 */
            12,          /* composing of 12 slices*/
            12);         /* composing of 12 stacks */

  // /*---- Draw forearm of the windmill ---*/
  // glRotatef(arm_ang, 0.0, 1.0, 0.0);

  // glColor3f(0.68, 0.68, 0.68);
  // gluCylinder(cylind, 1.0, 1.0, 4.0, 12, 3);

  // glTranslatef(0.0, 0.0, 5.0); /* goto end of forearm, M4 coord. sys. */
  // glColor3f(0.2, 0.2, 0.95);
  // gluSphere(sphere, 1.5,   /* radius=1.5 */
  //     12,            /* composing of 12 slices*/
  //     12);           /* composing of 12 stacks */

  glRotatef(-90.0, 1.0, 0.0, 0.0);

  /*------Draw three blades ---*/
  glColor3f(1.0, 1.0, 1.0);

  if (position[1] > 0)
  {
    glRotatef(bld_ang, 0.0, 0.0, 1.0); /* Rotate about x axis, M5 coord. sys. */
  }
  draw_blade(); /* draw the first blade */ //為往y軸畫

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree, M6 coord. sys */
  draw_blade();                   /* draw 2nd blade */

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree,M7 coord. sys */
  draw_blade();                   /* draw 3rd blade */

  glRotatef(90.0, 0.0, 0.0, 1.0); /* rotate by 120 degree,M7 coord. sys */
  draw_blade();                   /* draw 3rd blade */

  /*-------Swap the back buffer to the front --------*/
  // glutSwapBuffers();
  // glFlush(); /*--Display the results----*/
}
/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  switch (show_case)
  {
  case Orthographical_x:
    make_projection(Orthographical_x);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_x); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();
    break;
  case Orthographical_y:
    make_projection(Orthographical_y);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_y); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();
    break;
  case Orthographical_z:
    make_projection(Orthographical_z);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_z); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();
    break;
  case Perspective:
    make_projection(Perspective);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();     /* Initialize modelview matrix */
    my_view(Perspective); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();
    break;
  case show_All:
    make_projection(Perspective);
    glViewport(width / 2, 0, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();     /* Initialize modelview matrix */
    my_view(Perspective); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();

    make_projection(Orthographical_x);
    glViewport(0, height / 2, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_x); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();

    make_projection(Orthographical_y);
    glViewport(width / 2, height / 2, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_y); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();

    make_projection(Orthographical_z);
    glViewport(0, 0, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_z); //<<<<<<<<<<<<<<<<<<<<<<
    draw_window();
    break;
  }
  glFlush(); /*--Display the results----*/
  // make_projection(Perspective);
  // /*----Define the current eye position and the eye-coordinate system---*/
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();           /* Initialize modelview matrix */
  // my_view(Project_style); //<<<<<<<<<<<<<<<<<<<<<<
  // draw_window();
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
  if (w > h)
    glOrtho(-15.0, 15.0, -15.0 * (float)h / (float)w, 15.0 * (float)h / (float)w,
            -10.5, 30.0);
  else
    glOrtho(-15.0 * (float)w / (float)h, 15.0 * (float)w / (float)h, -15.0, 15.0,
            -10.5, 30.0);
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void SpecialKey(GLint key, GLint x, GLint y)
{
  //上下左右
  if (key == GLUT_KEY_UP)
  {
    position[1] += Step;
    if (leg_len > 0)
    {
      leg_len -= 0.2;
    }
  }
  if (key == GLUT_KEY_DOWN)
  {
    if (position[1] > 0)
    {
      position[1] -= Step;
      if (leg_len < 2)
      {
        leg_len += 0.2;
      }
    }
  }
  if (key == GLUT_KEY_LEFT)
  {
    head_forward += 8.0;
  }
  if (key == GLUT_KEY_RIGHT)
  {
    head_forward -= 8.0;
  }
  display();
}
void my_keyboard(unsigned char key, int x, int y)
{
  // if(key=='Q'||key=='q') exit(0);
  //  if(key=='r'){
  //    arm_ang += 5.0;
  //    if(arm_ang>360.0) arm_ang -= 360.0;
  //  }else if(key=='c'){
  //    bld_ang += 5.0;
  //    if(bld_ang>360.0) bld_ang -=360.0;
  //  }
  if (key == 'w')
  {
    position[0] -= Step * cos(head_forward * PI / 180.0);
    position[2] += Step * sin(head_forward * PI / 180.0);
    // position[0] -= Step;
    state = 1;
  }
  if (key == 's')
  {
    position[0] += Step * cos(head_forward * PI / 180.0);
    position[2] -= Step * sin(head_forward * PI / 180.0);
    // position[0] += Step
    state = 2;
  }
  if (key == 'q')
  {
    head_forward += 10;
    position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
    position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
    state = 3;
  }
  if (key == 'e')
  {
    head_forward -= 10;
    position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
    position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
    state = 4;
  }
  if (key == 'f')
  {
    int i = 0;
    for (i = 0; i < 20; i++)
    {
      head_forward += 10;
      position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
      position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
      state = 3;
      Sleep(30);
      display();
    }
    for (i = 0; i < 30; i++)
    {
      head_forward -= 10;
      position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
      position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
      state = 4;
      Sleep(30);
      display();
    }
  }
  switch (key)
  {
  case 'u':
    delta[0] += Step;
    break;
  case 'j':
    delta[0] -= Step;
    break;
  case 'i':
    delta[1] -= Step;
    break;
  case 'k':
    delta[1] += Step;
    break;
  case 'o':
    delta[2] -= Step;
    break;
  case 'l':
    delta[2] += Step;
    break;
  case 'U':
    theta[0] += ANGLE;
    break;
  case 'J':
    theta[0] -= ANGLE;
    break;
  case 'I':
    theta[1] += ANGLE;
    break;
  case 'K':
    theta[1] -= ANGLE;
    break;
  case 'O':
    theta[2] -= ANGLE;
    break;
  case 'L':
    theta[2] += ANGLE;
    break;
  case '1':
    show_case = Orthographical_x;
    break;
  case '2':
    show_case = Orthographical_y;
    break;
  case '3':
    show_case = Orthographical_z;
    break;
  case '4':
    show_case = Perspective;
    break;
  case '5':
    show_case = show_All;
    break;
  }
  if (key == 'n')
  {
    zoom += 0.5;
  }
  else if (key == 'm')
  {
    zoom -= 0.5;
  }
  else if (key == '/')
  {
    lit0_angle += 3;
  }

  if (key == 'c' || key == 'C')
  {
    control_light_num++;
    control_light_num %= 3;
    printf("control light %d\n", control_light_num);
  }
  else if (key == 'v' || key == 'V')
  {
    if (control_light_num == 0)
    {
      if (turn_on_off_light[0]++ % 2 == 0)
        glDisable(GL_LIGHT0);
      else
        glEnable(GL_LIGHT0);
    }
    else if (control_light_num == 1)
    {
      if (turn_on_off_light[1]++ % 2 == 0)
        glDisable(GL_LIGHT1);
      else
        glEnable(GL_LIGHT1);
    }
    else if (control_light_num == 2)
    {
      if (turn_on_off_light[2]++ % 2 == 0)
        glDisable(GL_LIGHT2);
      else
        glEnable(GL_LIGHT2);
    }
    printf("Turn off/on control light %d\n", control_light_num);
  }
  switch (key) // control_light_num = 0 : red point ligth
  {            // small:decrease large:increase
  case 'r':
    if (light_color[control_light_num][0] - 0.1 > 0.0001)
      light_color[control_light_num][0] -= 0.1;
    break;
  case 'R':
    if (light_color[control_light_num][0] + 0.1 < 1.0)
      light_color[control_light_num][0] += 0.1;
    break;
  case 'g':
    if (light_color[control_light_num][1] - 0.1 > 0.0001)
      light_color[control_light_num][1] -= 0.1;
    break;
  case 'G':
    if (light_color[control_light_num][1] + 0.1 < 1.0)
      light_color[control_light_num][1] += 0.1;
    break;
  case 'b':
    if (light_color[control_light_num][2] - 0.1 > 0.0001)
      light_color[control_light_num][2] -= 0.1;
    break;
  case 'B':
    if (light_color[control_light_num][2] + 0.1 < 1.0)
      light_color[control_light_num][2] += 0.1;
    break;
  case 'z':
    for (int i = 0; i < 3; i++)
    {
      light_color[control_light_num][i] *= 0.9;
    }
    break;
  case 'Z':
    for (int i = 0; i < 3; i++)
    {
      light_color[control_light_num][i] *= 1.1;
    }
    break;
  default:
    break;
  }
  /*-----Define light0 ---------*/
  // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
  if (control_light_num == 0)
  {
    /*Define light0*/
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[0]);
    // printf("R = %f; G = %f; B = %f\n", light_color[0][0], light_color[0][1], light_color[0][2]);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    // glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
  }
  else if (control_light_num == 1)
  {
    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color[1]);
    // printf("R = %f; G = %f; B = %f\n", light_color[1][0], light_color[1][1], light_color[1][2]);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
  }
  else if (control_light_num == 2)
  {
    /*----Define light2 -----*/
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color[2]);
    // printf("R = %f; G = %f; B = %f\n", light_color[2][0], light_color[2][1], light_color[2][2]);
    // glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    // glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    // glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
  }

  switch (key)
  {
  case '!':
    modify_state = 0; // sun light direction
    printf("Now can modify sun light direction\n");
    break;
  case '@':
    modify_state = 1; // helicopter direction
    printf("Now can modify helicopter light direction\n");
    break;
  case '#':
    modify_state = 2; // helicopter exponent
    printf("Now can modify helicopter light exponenet\n");
    break;
  case '$':
    modify_state = 3; // helicopter cutoff angle
    printf("Now can modify helicopter light cutoff angle\n");
    break;
  default:
    break;
  }

  if (key == '+' && modify_state == 0)
  {
    rotate_dirction_lit1 += 2;
  }
  else if (key == '_' && modify_state == 0)
  {
    rotate_dirction_lit1 -= 2;
  }

  if (key == '+' && modify_state == 1)
  {
    rotate_dirction_lit2 += 2;
  }
  else if (key == '_' && modify_state == 1)
  {
    rotate_dirction_lit2 -= 2;
  }

  if (key == '+' && modify_state == 2)
  {
    lit2_exponent *= 2;
  }
  else if (key == '_' && modify_state == 2)
  {
    lit2_exponent /= 2;
  }

  if (key == '+' && modify_state == 3)
  {
    lit2_cutoff += 2;
  }
  else if (key == '_' && modify_state == 3)
  {
    lit2_cutoff -= 2;
  }

  if (key == '8')
  {
    sun_auto++;
  }
  else if (key == '9')
  {
    redball_auto++;
  }
  else if (key == '0')
  {
    spot_auto++;
  }
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
}

/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
  bld_ang += 5.0;
  mo_angle += 0.3;
  if (bld_ang > 360.0)
  {
    bld_ang -= 360.0;
    arm_ang += 3.0;
    if (arm_ang > 360.0)
      arm_ang -= 360.0;
  }

  if (sun_auto % 2 == 0)
  {
  }
  else
  {
    sun_auto_color++;
    if (sun_auto_color % 100 == 33)
    {
      light_color[0][0] = 1.0;
      light_color[0][1] = 1.0;
      light_color[0][2] = 1.0;
    }
    else if (sun_auto_color % 100 == 66)
    {
      light_color[0][0] = 1.0;
      light_color[0][1] = 0.0;
      light_color[0][2] = 1.0;
    }
    else if (sun_auto_color % 100 == 99)
    {
      light_color[0][0] = 0.0;
      light_color[0][1] = 1.0;
      light_color[0][2] = 1.0;
    }
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[0]);
  }

  if (redball_auto % 2 == 0)
  {
  }
  else
  {
    redball_auto_color++;
    if (redball_auto_color % 100 == 33)
    {
      light_color[1][0] = 1.0;
      light_color[1][1] = 0.0;
      light_color[1][2] = 0.0;
    }
    else if (redball_auto_color % 100 == 66)
    {
      light_color[1][0] = 0.0;
      light_color[1][1] = 1.0;
      light_color[1][2] = 0.0;
    }
    else if (redball_auto_color % 100 == 99)
    {
      light_color[1][0] = 0.0;
      light_color[1][1] = 0.0;
      light_color[1][2] = 1.0;
    }
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color[1]);
  }

  if (spot_auto % 2 == 0)
  {
  }
  else
  {
    spot_auto_color++;
    if (spot_auto_color % 100 == 33)
    {
      light_color[2][0] = 1.0;
      light_color[2][1] = 0.0;
      light_color[2][2] = 1.0;
    }
    else if (spot_auto_color % 100 == 66)
    {
      light_color[2][0] = 1.0;
      light_color[2][1] = 1.0;
      light_color[2][2] = 0.0;
    }
    else if (spot_auto_color % 100 == 99)
    {
      light_color[2][0] = 0.0;
      light_color[2][1] = 1.0;
      light_color[2][2] = 1.0;
    }
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color[2]);
  }
  display(); /* show the scene again */
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
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("windmill");

  myinit(); /*---Initialize other state varibales----*/

  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  glutIdleFunc(idle_func); /* Add idle event */
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_keyboard);
  glutSpecialFunc(SpecialKey);
  glutMainLoop();
}
