/******************************************************************
 * This program illustrates the fundamental instructions for handling 
 * mouse and keyboeard events as well as menu buttons.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/freeglut.h>

#define    SIZEX   800
#define    SIZEY   800

#define    MY_QUIT -1
#define    MY_CLEAR -2
#define    MY_SAVE  -3
#define    MY_BLEND -4
#define    MY_LOAD  -5

#define    WHITE   1
#define    RED     2
#define    GREEN   3
#define    BLUE    4

#define    GRID    0
#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    CURVE   5
#define    TRIANGLE 6
#define    RECTANGLE 7

#define    FILL 1
#define    NON_FILL 0

int is_typing = 0;
int eraser = 0;

typedef    int   menu_t;
menu_t     top_m, color_m, file_m, type_m, fill_m, grid_m, grid_sub_m, back_color_m, eraser_m;
int fill_mode = 0; 

int        height=512, width=512;
int        Grid_Width = 25;
unsigned char  image[SIZEX*SIZEY][4];  /* Image data in main memory */

int        pos_x = -1, pos_y = -1;
float      myColor[3]={0.0,0.0,0.0};
float      backgroundColor[3] = {0.0, 0.0, 0.0};

int        obj_type = -1;
int        first=0;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side=0;         /*num of sides of polygon */
float      pnt_size=3.0;

/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
void display_func(void)
{
  /* define window background color */
  //glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}


/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h)
{
  height = new_h;
  width = new_w;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width,height);
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}


/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */

void keyboard(unsigned char key, int x, int y)
{
    if (is_typing) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, key);
        glFlush(); //glFlush�A�O�ҫe����OpenGL�R�O�ߧY����]�Ӥ��O�����̦b�w�İϤ����ݡ^
        //https://iter01.com/190020.html
    }
    else {
        if(key == 'Q' || key == 'q') exit(0);
        if (key == '=') pnt_size++;
        if (key == '-' && (pnt_size-1) > 0) pnt_size--;
        if (key == 'r' && myColor[0] <= 1) {
            myColor[0] += (0.1);
        }
        else if (key == 'g' && myColor[1] <= 1) {
            myColor[1] += (0.1);
        }
        else if (key == 'b' && myColor[2] <= 1) {
            myColor[2] += (0.1);
        }
        glColor3f(myColor[0], myColor[1], myColor[2]);
    }
}


/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon()
{
  int  i;
  if (fill_mode == FILL) glPolygonMode(GL_FRONT, GL_FILL);
  else {
      glPolygonMode(GL_FRONT, GL_LINE);
  }
  glBegin(GL_POLYGON);
    for(i=0;i<side;i++)
      glVertex2f(vertex[i][0], height-vertex[i][1]);
  glEnd();
  glFinish();
  side = 0;    /* set side=0 for next polygon */
}



/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle(float after_x, float after_y)
{
   
  static GLUquadricObj *mycircle=NULL;

  if(mycircle==NULL){
    mycircle = gluNewQuadric();
    gluQuadricDrawStyle(mycircle,GLU_FILL);
  }
  float radius = sqrt((after_x - pos_x) * (after_x - pos_x) + (after_y - pos_y) * (after_y - pos_y));
  glPushMatrix();
  glTranslatef(pos_x, height-pos_y, 0.0);
  gluDisk(mycircle,
      (fill_mode == FILL ? 0.0 : radius - pnt_size),           /* inner radius=0.0 */
	  radius,          /* outer radius=10.0 */
	  16,            /* 16-side polygon */
	   3);
  glPopMatrix();
}


/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
float triangle_pos_x[2];
float triangle_pos_y[2];
void mouse_func(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && (obj_type != LINE)) {
        first = 0;
    }
    //TEXT
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
        pos_x = x;
        pos_y = y;
        if (!is_typing) {
            is_typing = 1;
            glRasterPos2i(pos_x, height - pos_y);
        }
        else {
            is_typing = 0;
        }
        //printf("%d", is_typing);
    }
  //triangle
  if(obj_type == TRIANGLE && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      triangle_pos_x[0] = x;
      triangle_pos_y[0] = y;
  }
  else if(obj_type == TRIANGLE && button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      triangle_pos_x[1] = x;
      triangle_pos_y[1] = y;
      if (fill_mode == FILL) {
          glBegin(GL_TRIANGLES);
          glVertex2f(triangle_pos_x[0], height - triangle_pos_y[0]);
          glVertex2f(triangle_pos_x[1], height - triangle_pos_y[0]);
          glVertex2f(triangle_pos_x[0] + (0.5 * (triangle_pos_x[1] - triangle_pos_x[0])), height - triangle_pos_y[1]);
          glEnd();
      }
      else {
          glLineWidth(pnt_size);
          glBegin(GL_LINES);
            glVertex2f(triangle_pos_x[0], height - triangle_pos_y[0]);
            glVertex2f(triangle_pos_x[1], height - triangle_pos_y[0]);

            glVertex2f(triangle_pos_x[1], height - triangle_pos_y[0]);
            glVertex2f(triangle_pos_x[0] + (0.5 * (triangle_pos_x[1] - triangle_pos_x[0])), height - triangle_pos_y[1]);

            glVertex2f(triangle_pos_x[0] + (0.5 * (triangle_pos_x[1] - triangle_pos_x[0])), height - triangle_pos_y[1]);
            glVertex2f(triangle_pos_x[0], height - triangle_pos_y[0]);
          glEnd();
      }
  }
  //circle
  if (obj_type == CIRCLE && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      pos_x = x; pos_y = y;
  }
  else if (obj_type == CIRCLE && button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      draw_circle(x, y);
  }
  //Rectangle
  if (obj_type == RECTANGLE && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      pos_x = x; pos_y = y;
  }
  else if (obj_type == RECTANGLE && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      if (fill_mode == FILL) {
          glBegin(GL_QUADS);
            glVertex2i(pos_x, height - pos_y);
            glVertex2i(pos_x, height - y);
            glVertex2i(x, height - y);
            glVertex2i(x, height - pos_y);
          glEnd();
      }
      else {
          glLineWidth(pnt_size);     /* Define line width */
          glBegin(GL_LINES);    /* Draw the line */
              glVertex2i(pos_x, height - pos_y);
              glVertex2i(pos_x, height - y);

              glVertex2i(pos_x, height - y);
              glVertex2i(x, height - y);

              glVertex2i(x, height - y);
              glVertex2i(x, height - pos_y);

              glVertex2i(x, height - pos_y);
              glVertex2i(pos_x, height - pos_y);
          glEnd();
      }
  }

  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;

  switch(obj_type){
  case POINT:
    glPointSize(pnt_size);     /*  Define point size */
    glBegin(GL_POINTS);     /*  Draw a point */
       glVertex2f(x, height-y);
    glEnd();
    break;
  case LINE:
    if(first==0){
      first = 1;
      pos_x = x; pos_y = y;
	  glPointSize(pnt_size);
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	    glVertex3f(x, height-y, 0);
      glEnd();
    }else{
      first=0;
      glLineWidth(pnt_size);     /* Define line width */
      glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(pos_x, height - pos_y);
	    glVertex2f(x, height - y);
      glEnd();
    }
    break;
  case POLYGON:  /* Define vertices of poly */
    if(side==0){
      vertex[side][0] = x; vertex[side][1] = y;
      side ++;
    }else{
        if (fabs(vertex[side - 1][0] - x) + fabs(vertex[side - 1][1] - y) < 2)
                draw_polygon();
      else{
	    glBegin(GL_LINES);
          glVertex2f(vertex[side-1][0], height-vertex[side-1][1]);
	      glVertex2f(x, height-y);
	    glEnd();
	    vertex[side][0] = x; 
		vertex[side][1] = y;
	    side ++;
      }
    }
    break;
  //case TRIANGLE:
  //    glBegin(GL_TRIANGLES);
  //      glVertex2f()
  //   glEnd();
  //    break;
  //case CIRCLE:
  //  pos_x = x; pos_y = y;
  //  draw_circle();
  //  break;
  default:
      break;
  }
  glFinish();
}


    
/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
    if (eraser == 1) {
        glLineWidth(pnt_size);
        if (first == 0) {
            first = 1;
            pos_x = x; pos_y = y;
        }
        else {
            glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
            glBegin(GL_LINES);
            glVertex3f(pos_x, height - pos_y, 0.0);
            glVertex3f(x, height - y, 0.0);
            glEnd();
            pos_x = x; pos_y = y;
        }
        glFinish();
        
    }
    else {
        if (obj_type != CURVE) return;
        glLineWidth(pnt_size);
        if (first == 0) {
            first = 1;
            pos_x = x; pos_y = y;
        }
        else {
            glBegin(GL_LINES);
            glVertex3f(pos_x, height - pos_y, 0.0);
            glVertex3f(x, height - y, 0.0);
            glEnd();
            pos_x = x; pos_y = y;
        }
        glFinish();
    }
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
  /*Do nothing else but clear window to black*/

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}


/*------------------------------------------------------
 * Procedure to initialize data alighment and other stuff
 */
void init_func()
{   glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*-----------------------------------------------------------------
 * Callback function for color menu
 */
void  color_func(int value)
{
  switch(value){
  case WHITE:
    myColor[0] = myColor[1] = myColor[2] = 1.0;
    break;

  case RED:
    myColor[0] = 1.0;
    myColor[1] = myColor[2] = 0.0;
    break;

  case GREEN:
    myColor[0] = myColor[2] = 0.0;
    myColor[1] = 1.0;
    break;
    
  case BLUE:
    myColor[0] = myColor[1] = 0.0;
    myColor[2] = 1.0;
    break;

  default:
    break;
  }
  glColor3f(myColor[0], myColor[1], myColor[2]);
}


/*------------------------------------------------------------
 * Callback function for top menu.
 */
void file_func(int value)
{ 
  int i, j;

  if(value==MY_QUIT) exit(0);
  else if(value==MY_CLEAR) init_window();
  else if(value==MY_SAVE){ /* Save current window */
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
	for(i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  for(j=0;j<height;j++)
		if(image[i*width+j][0]==0 &&
           image[i*width+j][1]==0 &&
           image[i*width+j][2]==0) image[i*width+j][3] = 0;
		else image[i*width+j][3] = 127; /* Other pixels have A=127*/
  }else if(value==MY_LOAD){ /* Restore the saved image */
	 glRasterPos2i(0, 0);
     glDrawPixels(width, height, 
	       GL_RGBA, GL_UNSIGNED_BYTE, 
	       image);
  }else if(value==MY_BLEND){ /* Blending current image with the saved image */	
	  glEnable(GL_BLEND); 
	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glRasterPos2i(0, 0);
      glDrawPixels(width, height, 
	       GL_RGBA, GL_UNSIGNED_BYTE, 
	       image);
	  glDisable(GL_BLEND);
  }
  glFlush();
}

void size_func(int value)
{
	if(value==1){
	  pnt_size = 5.0;
	}else{
	  pnt_size = 1;
	}
}

/*------------------------------------------------------------
 * Callback function for fill menu.
 */
void fill_func(int value) {
    if (value == 1) { //fill
        fill_mode = FILL;
    }
    else {             //non_fill
        fill_mode = NON_FILL;
    }
}

/*------------------------------------------------------------
 * Callback function for grid menu.
 */
void grid_func(int value) {
    if (value == GRID) {
        int window_height = height; ////for calculate grid
        int window_width = width;
        int loop_i = 0;
        glBegin(GL_LINES);
        while (window_height / 2 - loop_i >= 0) {
            //��u
            glVertex2f(0, window_height / 2 - loop_i * Grid_Width);
            glVertex2f(window_width, window_height / 2 - loop_i * Grid_Width);
            glVertex2f(0, window_height / 2 + loop_i * Grid_Width);
            glVertex2f(window_width, window_height / 2 + loop_i * Grid_Width);
            loop_i++;
        }
        loop_i = 0;
        while (window_height - loop_i >= 0) {
            //���u colunm
            glVertex2f(window_width / 2 - loop_i * Grid_Width, 0);
            glVertex2f(window_width / 2 - loop_i * Grid_Width, window_height);
            glVertex2f(window_width / 2 + loop_i * Grid_Width, 0);
            glVertex2f(window_width / 2 + loop_i * Grid_Width, window_height);
            loop_i++;
        }
        glEnd();
        glFlush(); //�N�w�s���Ϯױq�w�s���X
    }
}
void grid_sub_func(int value) {
    if (value == 1 && Grid_Width <= 40) {
        Grid_Width += 5;
    }
    else if(value == 0 && Grid_Width > 0){
        Grid_Width -= 5;
    }
}

void back_color_func(int value) {
    switch (value)
    {
    case 0:
        backgroundColor[0] = 0;
        backgroundColor[1] = 0;
        backgroundColor[2] = 0;
        break;
    case 1:
        backgroundColor[0] = 1;
        backgroundColor[1] = 1;
        backgroundColor[2] = 1;
        break;
    case 2:
        backgroundColor[0] = 0;
        backgroundColor[1] = 0;
        backgroundColor[2] = 1;
        break;
    default:
        break;
    }
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void eraser_func(int value) {
    if (value == 1) {
        eraser = 1;
    }
    else {
        eraser = 0; 
        glColor3f(myColor[0], myColor[1], myColor[2]);
        printf("eraser close...");
    }
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
    
}


/*-------------------------------------------------------------
 * Callback Func for type_m, define drawing object
 */
void draw_type(int value)
{
  obj_type = value;
  if(value==LINE||value==CURVE)
    first = 0;
  else if(value==POLYGON) side = 0;
}


/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
int main(int argc, char **argv)
{
  int  size_menu;

  glutInit(&argc, argv);    /*---Make connection with server---*/

  glutInitWindowPosition(0,0);  /*---Specify window position ---*/
  glutInitWindowSize(width, height); /*--Define window's height and width--*/

  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA); /*---set display mode---*/
  init_func();

   /* Create parent window */
  glutCreateWindow("Menu"); 

  glutDisplayFunc(display_func); /* Associate display event callback func */
  glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

  glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

  glutMouseFunc(mouse_func);  /* Mouse Button Callback func */

  glutMotionFunc(motion_func);/* Mouse motion event callback func */


  color_m = glutCreateMenu(color_func); /* Create color-menu */
  glutAddMenuEntry("white", WHITE);
  glutAddMenuEntry("red", RED);
  glutAddMenuEntry("green", GREEN);
  glutAddMenuEntry("blue", BLUE);

  file_m = glutCreateMenu(file_func);   /* Create another menu, file-menu */
  glutAddMenuEntry("save", MY_SAVE);
  glutAddMenuEntry("load", MY_LOAD);
  glutAddMenuEntry("blend", MY_BLEND);
  glutAddMenuEntry("clear", MY_CLEAR);
  glutAddMenuEntry("quit", MY_QUIT);

  type_m = glutCreateMenu(draw_type);   /* Create draw-type menu */
  //glutAddMenuEntry("Grid", GRID);
  glutAddMenuEntry("Point", POINT);
  glutAddMenuEntry("Line", LINE);
  glutAddMenuEntry("Poly", POLYGON);
  glutAddMenuEntry("Curve", CURVE);
  glutAddMenuEntry("Circle", CIRCLE);
  glutAddMenuEntry("Triangle", TRIANGLE);
  glutAddMenuEntry("Rectangle", RECTANGLE);

  size_menu = glutCreateMenu(size_func);
  glutAddMenuEntry("Big_printer", 1);
  glutAddMenuEntry("Small_printer",2);


  fill_m = glutCreateMenu(fill_func);
  glutAddMenuEntry("Fill", 1);
  glutAddMenuEntry("Non_fill", 0);

  grid_sub_m = glutCreateMenu(grid_sub_func);
  glutAddMenuEntry("< + >", 1);
  glutAddMenuEntry("< - >", 0);

  grid_m = glutCreateMenu(grid_func);
  glutAddMenuEntry("Draw_Grid", GRID);
  glutAddSubMenu("Grid_width", grid_sub_m);

  back_color_m = glutCreateMenu(back_color_func);
  glutAddMenuEntry("black", 0);
  glutAddMenuEntry("white", 1);
  glutAddMenuEntry("blue", 2);

  eraser_m = glutCreateMenu(eraser_func);
  glutAddMenuEntry("< on >", 1);
  glutAddMenuEntry("< off >", 0);

  top_m = glutCreateMenu(top_menu_func);/* Create top menu */
  glutAddSubMenu("colors", color_m);    /* add color-menu as a sub-menu */
  glutAddSubMenu("printer_width", size_menu);
  glutAddSubMenu("mode", fill_m);
  glutAddSubMenu("type", type_m);
  glutAddSubMenu("grid", grid_m);
  glutAddSubMenu("backColor", back_color_m);
  glutAddSubMenu("Eraser", eraser_m);
  glutAddSubMenu("file", file_m);       /* add file-menu as a sub-menu */
  glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/

  /*---Test whether overlay support is available --*/
  if(glutLayerGet(GLUT_OVERLAY_POSSIBLE)){
    fprintf(stderr,"Overlay is available\n");
  }else{
    fprintf(stderr,"Overlay is NOT available, May encounter problems for menu\n");
  }
  /*---Enter the event loop ----*/
  glutMainLoop();       
}
