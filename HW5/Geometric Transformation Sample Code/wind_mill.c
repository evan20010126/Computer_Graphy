/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#include <math.h>
#include <stdio.h>

#include <GL/glut.h>
#include <time.h>
#define Step 0.2
#define ANGLE 1.0
#define PI 3.14159265359
/*-------------hw5_texture------------------*/
float fog_color[] = {0.15, 0.20, 0.20, 0.50};
int change_flog = -1;
int flog_modify_state = 0;
float FOG_DENSITY = 0.01;
float FOG_END = 100.0;
/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
#define TSIZE 64
unsigned char copy_for_airplane[TSIZE][TSIZE][4];
unsigned char checkboard[TSIZE][TSIZE][4];  /* checkboard textures */
unsigned char checkboard2[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char brick[TSIZE][TSIZE][4];       /* brick wall textures */
unsigned char land[64][64][4];
unsigned int textName[5]; /* declare two texture maps*/
void make_check() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkboard[i][j][0] = c;
            checkboard[i][j][1] = c;
            checkboard[i][j][2] = c;
            checkboard[i][j][3] = 255;
        }
}

/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
unsigned char star[64][64][4];
void Create_Texture_Star() {
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++) {
            star[x][y][0] = 255;
            star[x][y][1] = 255;
            star[x][y][2] = 255;
            star[x][y][3] = 255;
        }

    x = 31;
    y = 60;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 100;
            star[x][i][1] = 100;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 3;
        x--;
    }
    x = 31;
    y = 20;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 255;
            star[x][i][1] = 255;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 1;
        x--;
    }
    x = 4;
    y = 40;
    while (y >= 18) {
        for (i = x; i < 32; i++) {
            star[i][y][0] = 100;
            star[i][y][1] = 100;
            star[i][y][2] = 255;
            star[i][y][3] = 255;
        }
        y -= 1;
        if ((y % 2) == 1)
            x += 2;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; i++)
                star[x][y][i] = star[63 - x][y][i];
}
void make_check2() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            checkboard2[i][j][0] = c / 8;
            checkboard2[i][j][1] = c / 2;
            checkboard2[i][j][2] = c / 4;
            if (c == 255)
                checkboard2[i][j][3] = 255;
            else
                checkboard2[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            checkboard2[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            checkboard2[i][j][3] = 0;
    }
}

void brick_pattern() {
    int y, x, i;

    for (y = 0; y < 12; y++) { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                brick[y][x][i] = brick[y - 32][x][i];
}
void Create_Texture_Land() {
    int x, y, i;

    for (y = 0; y < 32; y++)
        for (x = 0; x < 32; x++) {
            land[x][y][0] = 240;
            land[x][y][1] = 240;
            land[x][y][2] = 240;
            land[x][y][3] = 255;
        }

    for (x = 24; x < 32; x++) {
        land[x][0][0] = 255;
        land[x][0][1] = 200;
        land[x][0][2] = 200;
        land[x][0][3] = 255;

        land[0][x][0] = 255;
        land[0][x][1] = 200;
        land[0][x][2] = 200;
        land[0][x][3] = 255;
    }
    for (x = 26; x < 32; x++) {
        land[x][1][0] = 255;
        land[x][1][1] = 200;
        land[x][1][2] = 200;
        land[x][1][3] = 255;

        land[1][x][0] = 255;
        land[1][x][1] = 200;
        land[1][x][2] = 200;
        land[1][x][3] = 255;
    }
    for (x = 27; x < 32; x++) {
        for (y = 2; y < 4; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 28; x < 32; x++) {
        for (y = 4; y < 7; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 29; x < 32; x++) {
        for (y = 7; y < 9; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 30; x < 32; x++) {
        for (y = 9; y < 12; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 31; x < 32; x++) {
        for (y = 12; y < 16; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }

    for (y = 16; y < 32; y++)
        for (x = 16; x < 32; x++) {
            land[x][y][0] = 200;
            land[x][y][1] = 160;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }
    for (y = 20; y < 32; y++)
        for (x = 20; x < 32; x++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }

    for (x = 63; x > 31; x--) {
        for (y = 0; y < 32; y++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[63 - x][y][i];
    }

    /* Create the rest half part */
    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[x][63 - y][i];
}
/*------------------------------------------*/

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
float light_color[3][4] = {
    {0.8, 0.4, 0.4, 1.0},
    {0.7, 0.7, 0.0, 1.0},
    {0.0, 0.4, 0.8, 1.0}};  //????????????R or G or B (????????????diffuse)
/*----Define light source properties -------*/
float lit_position[] = {10, -4.0, 4.0, 1.0};
float lit_direction[] = {-1.0, -1.0, 1.0, 0.0};
float lit_diffuse[] = {0.8, 0.4, 0.4, 1.0};
float lit_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit_cutoff = 60.0;
float lit_exponent = 8.0;
float lit0_angle = 30;

//..lit1???sun(directional light)
float lit1_position[] = {20, 20.0, -2.0, 1.0};
float lit1_direction[] = {-1.0, -1.0, 1.0, 0.0};
float lit1_diffuse[] = {0.7, 0.7, 0.0, 1.0};
float lit1_specular[] = {0.7, 0.7, 0.7, 1.0};
float rotate_dirction_lit1 = 0;

//..lit2????????????(spot light)
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
/*---- the axes of billboard ----*/
float a[3], b[3];
float mtx[16]; /* the modelview matrix */
float mat_ambient[] = {0.8, 0.8, 0.8, 1.0};
float mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
void compute_ab_axes(void) {
    float w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0;
    b[1] = 1.0;
    b[2] = 0.0;
    a[0] = w2 / len;
    a[1] = 0.0;
    a[2] = -w0 / len;
}
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * a[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void draw_view_volumn() {
    float far_plane_length = (50.0 * zoom) / 2.5;
    float zoom_points[][3] = {{-zoom, -zoom, -2.5},
                              {zoom, -zoom, -2.5},
                              {zoom, zoom, -2.5},
                              {-zoom, zoom, -2.5},
                              {-far_plane_length, -far_plane_length, -50.0},
                              {far_plane_length, -far_plane_length, -50.0},
                              {far_plane_length, far_plane_length, -50.0},
                              {-far_plane_length, far_plane_length, -50.0}};
    int zoom_face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                          {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
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

    for (i = 0; i < 6; i++) {
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

void my_view(int P_style) {
    // glMatrixMode(GL_MODELVIEW);
    switch (P_style) {
        case Orthographical_x:
            gluLookAt(15.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
            draw_view_volumn();  //????????????????????????????????????
            break;
        case Orthographical_y:
            gluLookAt(0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);  // WHY?
            draw_view_volumn();  //????????????????????????????????????
            break;
        case Orthographical_z:
            gluLookAt(0.0, 2.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
            draw_view_volumn();  //????????????????????????????????????
            break;
        case Perspective:
            glRotatef(theta[0], 1.0, 0.0, 0.0);
            glRotatef(theta[1], 0.0, 1.0, 0.0);
            glRotatef(theta[2], 0.0, 0.0, -1.0);
            glTranslated(delta[0], delta[1], delta[2]);
            gluLookAt(eye[0], eye[1], eye[2], 10.0, 5.0, -10.0, 0.0, 1.0, 0.0);
            ////????????????
            // draw_view_volumn(); //????????????????????????????????????
            break;
    }
}
void make_projection(int x) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == Perspective) {
        glFrustum(-zoom, zoom, -zoom, zoom, 2.5,
                  50.0);  //(near??????, near??????, near??????, near??????, near??????,
                          //far??????)
        // gluPerspective(90.0, (double) width/ (double) height, 1.5, 50.0);
    } else {
        if (width > height)
            glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
                    40.0 * (float)height / (float)width, -0.0, 100.0);
        else
            glOrtho(-40.0 * (float)width / (float)height,
                    40.0 * (float)width / (float)height, -40.0, 40.0, -0.0,
                    100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

//

int state = 0;
// state = 1 ?????????
int count = 0;
int is_line = 0;

/*-----Define a unit box--------*/
/* Vertices of the box */
float points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5},
                     {-0.5, 0.5, -0.5},  {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                     {0.5, 0.5, 0.5},    {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                 {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
/* indices of the box faces */
float points_ass[][3] = {{3.0, -1.5, -1.5},   {3.0, -1.5, 1.5},
                         {3.0, 1.5, 1.5},     {3.0, 1.5, -1.5},
                         {5.0, -0.75, -0.75}, {5.0, -0.75, 0.75},
                         {5.0, 0.75, 0.75},   {5.0, 0.75, -0.75}};
int face_ass[][4] = {{0, 1, 2, 3}, {0, 1, 5, 4}, {0, 4, 7, 3},
                     {1, 5, 6, 2}, {3, 7, 6, 2}, {4, 5, 6, 7}};
int cube[6] = {0, 1, 2, 3, 4, 5};

float points_twelve[][3] = {
    {1, 0, 0},         {0.866, 0, 0.5},  {0.5, 0, 0.866},  {0, 0, 1},
    {-0.5, 0, 0.866},  {-0.866, 0, 0.5}, {-1, 0, 0},       {-0.866, 0, -0.5},
    {-0.5, 0, -0.866}, {0, 0, -1},       {0.5, 0, -0.866}, {0.866, 0, -0.5},
    {1, 1, 0},         {0.866, 1, 0.5},  {0.5, 1, 0.866},  {0, 1, 1},
    {-0.5, 1, 0.866},  {-0.866, 1, 0.5}, {-1, 1, 0},       {-0.866, 1, -0.5},
    {-0.5, 1, -0.866}, {0, 1, -1},       {0.5, 1, -0.866}, {0.866, 1, -0.5}};
float mo_angle = 0.0;

float normal[][4] = {{0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0},  {0.0, 1.0, 0.0},  {-1.0, 0.0, 0.0}};

/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj *sphere = NULL, *sphere_ass = NULL;
GLUquadricObj *cylind = NULL, *driverWindow = NULL;
GLUquadricObj *airplane_ass1 = NULL, *airplane_ass2 = NULL;
GLUquadricObj *leg1 = NULL, *leg2 = NULL, *leg3 = NULL;
GLUquadricObj *wheel_A_cylinder = NULL, *wheel_B_cylinder = NULL,
              *wheel_C_cylinder = NULL;
GLUquadricObj *wheel_A_cycle_1 = NULL, *wheel_B_cycle_1 = NULL,
              *wheel_C_cycle_1 = NULL;
GLUquadricObj *wheel_A_cycle_2 = NULL, *wheel_B_cycle_2 = NULL,
              *wheel_C_cycle_2 = NULL;

GLUquadricObj *mo_arm1 = NULL, *mo_arm2 = NULL, *mo_arm3 = NULL,
              *mo_arm4 = NULL;

GLUquadricObj* world_coordinate_origin = NULL;
GLUquadricObj* world_axes[3] = {NULL};

// Global variables recording rotation angles of H-arm and blades.
float arm_ang = 45.0, bld_ang = 10.0;
float leg_len = 2.0;

//?????????????????????
float position[3] = {10.0, 0.0, 6.0};
//???????????????
float head_forward = 0.0f;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void draw_floor() {
    int i, j;

    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
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
            glTexCoord2f(0.0, 0.0);
            glVertex3f((i - 5.0) * 10.0, -4.5, (j - 5.0) * 10.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f((i - 5.0) * 10.0, -4.5, (j - 4.0) * 10.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f((i - 4.0) * 10.0, -4.5, (j - 4.0) * 10.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f((i - 4.0) * 10.0, -4.5, (j - 5.0) * 10.0);
            glEnd();
        }
}
void myinit() {
    printf("light0 is point light (red ball)\n");
    printf("light1 is directional light (sun)\n");
    printf("light2 is spot light (helicopter's light)\n");

    printf("control light %d\n", control_light_num);

    glClearColor(0.0, 0.0, 0.0, 1.0); /*set the background color BLACK */
                                      /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT |
            GL_ACCUM_BUFFER_BIT);

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

    /*-----Create Texture -----*/
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(6, textName);
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, checkboard);

    /*-----Create another texture ----*/
    brick_pattern();
    glBindTexture(GL_TEXTURE_2D, textName[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, brick);
    /*------Create other texture-----*/
    Create_Texture_Land();

    glBindTexture(GL_TEXTURE_2D, textName[2]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, land);

    /*------Create other texture-----*/
    make_check2();

    glBindTexture(GL_TEXTURE_2D, textName[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, checkboard2);
    /*------Create other texture-----*/
    Create_Texture_Star();

    glBindTexture(GL_TEXTURE_2D, textName[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, star);
    /*Create*/
    glBindTexture(GL_TEXTURE_2D, textName[5]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, copy_for_airplane);
    ///*-----Enable face culling -----*/
    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);
    /*----Set up fog conditions ---*/
    glDisable(GL_FOG);              /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR); /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, FOG_DENSITY); /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);           /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, FOG_END);
    glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */

    glFlush(); /*Enforce window system display the results*/
}

/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade() {
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

void draw_ass1() {
    int i;

    // glColor3f(1.0, 1.0, 40.0 / 255.0);
    for (i = 0; i < 6; i++) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[2]);
        // glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse); /*diffuse color */
        // glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
        // glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
        // glMaterialf(GL_FRONT, GL_SHININESS, body_shininess);
        // glNormal3fv(normal[i]);
        glBegin(GL_POLYGON); /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points_ass[face_ass[i][0]]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(points_ass[face_ass[i][1]]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(points_ass[face_ass[i][2]]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(points_ass[face_ass[i][3]]);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
float anggg = 0;
int test1;
int air_body_on = 0;
void draw_cube() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if (test1 == 0) {
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();
        glScalef(10.0, 10.0, 10.0);
        glRotatef(anggg, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, textName[4]);
    } else {
        if (air_body_on == 0)
            glDisable(GL_TEXTURE);
        else {
            glEnable(GL_TEXTURE);
            glBindTexture(GL_TEXTURE_2D, textName[5]);
        }
        // glBindTexture(GL_TEXTURE_2D, textName[1]);
    }
    int i;
    // glColor3f(1.0, 1.0, 102.0 / 255.0);
    for (i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);                              /* Draw the face */
        glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse); /*diffuse color */
        glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, body_shininess);
        glNormal3fv(normal[i]);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_TEXTURE_2D);
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

void draw_plastic(char _color) {
    float temp_diffuse[3];
    float temp_specular[3];
    float temp_ambient[3];
    float temp_shininess;
    switch (_color) {
        case 'c':
            for (int i = 0; i < 3; i++) {
                temp_diffuse[i] = cyan_plastic_diffuse[i];
                temp_specular[i] = cyan_plastic_specular[i];
                temp_ambient[i] = cyan_plastic_ambient[i];
            }
            temp_shininess = cyan_plastic_shininess;
            break;
        case 'g':
            for (int i = 0; i < 3; i++) {
                temp_diffuse[i] = green_plastic_diffuse[i];
                temp_specular[i] = green_plastic_specular[i];
                temp_ambient[i] = green_plastic_ambient[i];
            }
            temp_shininess = green_plastic_shininess;
            break;
        case 'r':
            for (int i = 0; i < 3; i++) {
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

void draw_twelve() {
    int i;
    draw_plastic('r');
    glBegin(GL_POLYGON); /* Draw the face */
    for (i = 0; i < 12; i++) {
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
    for (i = 0; i <= 10; i++) {
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
    for (i = 12; i < 23; i++) {
        glVertex3fv(points_twelve[i]);
    }
    glEnd();
}

void draw_window() {
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    compute_ab_axes();
    /*-------Draw the billboard ----*/
    glEnable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glBindTexture(GL_TEXTURE_2D, textName[3]);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            glBindTexture(GL_TEXTURE_2D, textName[(i + j) % 4]);
            draw_billboard(-10.0 + i * 20, -3.0 - j * 8, 5.0, 10.0);
        }
    }
    // draw_billboard(5.0, 3.0, 5.0, 8.0);
    // draw_billboard(6.0, 5.0, 5.0, 8.0);
    // draw_billboard(3.0, 6.0, 5.0, 8.0);
    // draw_billboard(2.0, 7.0, 5.0, 8.0);
    // draw_billboard(7.0, 2.0, 5.0, 8.0);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    test1 = 0;
    glScalef(80.0, 50.0, 50.0);
    draw_cube();
    test1 = 1;
    glPopMatrix();
    glEnable(GL_LIGHTING);
    ////////////////////////for lit1 (is directional
    ///light)/////////////////////////////////////////////////////////////////
    glPushMatrix();
    /*----position light1 (fixed) in eye coord sys ---*/
    glPushMatrix();
    glRotatef(rotate_dirction_lit1, 0, 1, 0);
    glLightfv(GL_LIGHT1, GL_POSITION,
              lit1_direction); /*fixed position in eye space---*/
    glPopMatrix();
    /*---draw the light source ---*/
    glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    if (turn_on_off_light[1] % 2 == 0) {
        glDisable(GL_LIGHTING); /* Turn on lighting */
    }
    // glDisable(GL_CULL_FACE);
    glColor3f(light_color[1][0], light_color[1][1], light_color[1][2]);
    glutSolidSphere(3, 12, 12);
    glPopMatrix();
    // glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    ///////////////////////for lit0 (is a point
    ///light)/////////////////////////////////////////////////////////////////////
    /*-------Rotate light position----*/
    if (turn_on_off_light[0] % 2 == 0) {
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

    if (world_coordinate_origin == NULL) {
        world_coordinate_origin = gluNewQuadric();
        gluQuadricDrawStyle(world_coordinate_origin, GLU_FILL);
    }
    if (world_axes[0] == NULL) {
        world_axes[0] = gluNewQuadric();
        gluQuadricDrawStyle(world_axes[0], GL_FILL);
    }
    if (world_axes[1] == NULL) {
        world_axes[1] = gluNewQuadric();
        gluQuadricDrawStyle(world_axes[1], GL_FILL);
    }
    if (world_axes[2] == NULL) {
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
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    /* Draw Y axis */
    glPushMatrix();
    draw_plastic('g');
    glRotatef(-90.0, 1.0, 0.0, 0.0); /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(world_axes[1], 0.25, 0.25, /* radius of top and bottom circle */
                10.0,                      /* height of the cylinder */
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    draw_plastic('r');
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0); /*Rotate about y  by 90', x becomes z */
    gluCylinder(world_axes[2], 0.25, 0.25, /* radius of top and bottom circle */
                10.0,                      /* height of the cylinder */
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    glPopMatrix();

    // gluLookAt(10.0, 5.0, 15.0, 10.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    /*-------Draw the base of the windmill which is a cube----*/

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[2]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse); /*diffuse color */
    glMaterialfv(GL_FRONT, GL_SPECULAR, flr_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
    draw_floor();
    glDisable(GL_TEXTURE_2D);

    glRotatef(30.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslatef(1, 5, -5);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    draw_plastic('c');
    if (mo_arm1 == NULL) { /* allocate a quadric object, if necessary */
        mo_arm1 = gluNewQuadric();
        gluQuadricDrawStyle(mo_arm1, GLU_FILL);
        gluQuadricNormals(mo_arm1, GLU_SMOOTH);
    }
    if (mo_arm2 == NULL) { /* allocate a quadric object, if necessary */
        mo_arm2 = gluNewQuadric();
        gluQuadricDrawStyle(mo_arm2, GLU_FILL);
        gluQuadricNormals(mo_arm2, GLU_SMOOTH);
    }
    if (mo_arm3 == NULL) { /* allocate a quadric object, if necessary */
        mo_arm3 = gluNewQuadric();
        gluQuadricDrawStyle(mo_arm3, GLU_FILL);
        gluQuadricNormals(mo_arm3, GLU_SMOOTH);
    }
    if (mo_arm4 == NULL) { /* allocate a quadric object, if necessary */
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

    if (state == 1) {
        glRotatef(10.0, 0.0, 0.0, 1.0);
        count++;
        if (count == 1500) {
            count = 0;
            state = 0;
        }
    } else if (state == 2) {
        glRotatef(-10.0, 0.0, 0.0, 1.0);
        count++;
        if (count == 1500) {
            count = 0;
            state = 0;
        }
    } else if (state == 3) {
        glRotatef(-10.0, -1.0 / sqrt(2), 0.0, -1.0 / sqrt(2));
        count++;
        if (count == 1500) {
            count = 0;
            state = 0;
        }
    } else if (state == 4) {
        glRotatef(10.0, -1.0 / sqrt(2), 0.0, 1.0 / sqrt(2));
        count++;
        if (count == 1500) {
            count = 0;
            state = 0;
        }
    }
    glPushMatrix();          /* Save M1 coord. sys */
    glScalef(6.0, 3.0, 3.0); /* Scale up the axes by 6 times */
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    draw_cube();   /* Draw a unit cube in the new coord. sys. */
    glPopMatrix(); /* Get M1 back */

    //??????????????????
    glPushMatrix();
    glTranslatef(0.0, -1.5, 0.0);
    if (leg1 == NULL) { /* allocate a quadric object, if necessary */
        leg1 = gluNewQuadric();
        gluQuadricDrawStyle(leg1, GLU_FILL);
        gluQuadricNormals(leg1, GLU_SMOOTH);
    }
    if (leg2 == NULL) { /* allocate a quadric object, if necessary */
        leg2 = gluNewQuadric();
        gluQuadricDrawStyle(leg2, GLU_FILL);
        gluQuadricNormals(leg2, GLU_SMOOTH);
    }
    if (leg3 == NULL) { /* allocate a quadric object, if necessary */
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
    // draw_wheel??????
    glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
    glTranslatef(0.0, -0.5, leg_len + 0.5);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    if (wheel_A_cylinder ==
        NULL) { /* allocate a quadric object, if necessary */
        wheel_A_cylinder = gluNewQuadric();
        gluQuadricDrawStyle(wheel_A_cylinder, GLU_FILL);
        gluQuadricNormals(wheel_A_cylinder, GLU_SMOOTH);
    }
    gluCylinder(wheel_A_cylinder, 0.5,
                0.5, /* radius of top and bottom circle */
                1.0, /* height of the cylinder */
                12,  /* use 12-side polygon approximating circle*/
                3);  /* Divide it into 3 sections */
    glTranslatef(0.0, 0.0, 1.0);
    if (wheel_A_cycle_1 == NULL) {
        wheel_A_cycle_1 = gluNewQuadric();
    }

    glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
    gluDisk(wheel_A_cycle_1, 0, 0.5, 12, 3);
    glTranslatef(0.0, 0.0, -1);
    if (wheel_A_cycle_2 == NULL) {
        wheel_A_cycle_2 = gluNewQuadric();
    }
    gluDisk(wheel_A_cycle_2, 0, 0.5, 12, 3);
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
    // draw_wheel??????
    glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
    glTranslatef(0.0, -0.5, leg_len + 0.5);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    if (wheel_B_cylinder ==
        NULL) { /* allocate a quadric object, if necessary */
        wheel_B_cylinder = gluNewQuadric();
        gluQuadricDrawStyle(wheel_B_cylinder, GLU_FILL);
        gluQuadricNormals(wheel_B_cylinder, GLU_SMOOTH);
    }
    gluCylinder(wheel_B_cylinder, 0.5,
                0.5, /* radius of top and bottom circle */
                1.0, /* height of the cylinder */
                12,  /* use 12-side polygon approximating circle*/
                3);  /* Divide it into 3 sections */
    glTranslatef(0.0, 0.0, 1.0);
    if (wheel_B_cycle_1 == NULL) {
        wheel_B_cycle_1 = gluNewQuadric();
    }

    glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
    gluDisk(wheel_B_cycle_1, 0, 0.5, 12, 3);
    glTranslatef(0.0, 0.0, -1.0);
    if (wheel_B_cycle_2 == NULL) {
        wheel_B_cycle_2 = gluNewQuadric();
    }
    gluDisk(wheel_B_cycle_2, 0, 0.5, 12, 3);

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
    // draw_wheel??????
    glColor3f(255.0 / 255.0, 0.0 / 255.0, 0.0);
    glTranslatef(0.0, -0.5, leg_len + 0.5);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    if (wheel_C_cylinder ==
        NULL) { /* allocate a quadric object, if necessary */
        wheel_C_cylinder = gluNewQuadric();
        gluQuadricDrawStyle(wheel_C_cylinder, GLU_FILL);
        gluQuadricNormals(wheel_C_cylinder, GLU_SMOOTH);
    }
    gluCylinder(wheel_C_cylinder, 0.5,
                0.5, /* radius of top and bottom circle */
                1.0, /* height of the cylinder */
                12,  /* use 12-side polygon approximating circle*/
                3);  /* Divide it into 3 sections */
    glTranslatef(0.0, 0.0, 1.0);
    if (wheel_C_cycle_1 == NULL) {
        wheel_C_cycle_1 = gluNewQuadric();
    }

    glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0);
    gluDisk(wheel_C_cycle_1, 0, 0.5, 12, 3);
    glTranslatef(0.0, 0.0, -1.0);
    if (wheel_C_cycle_2 == NULL) {
        wheel_C_cycle_2 = gluNewQuadric();
    }
    gluDisk(wheel_C_cycle_2, 0, 0.5, 12, 3);
    //
    glPopMatrix();
    glPopMatrix();

    // driverWindow<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    glColor3f(0.2, 0.2, 1.0); /* Gray colored */

    if (driverWindow == NULL) { /* allocate a quadric object, if necessary */
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
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    glPopMatrix();  /* Get M2 back */

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
    if (turn_on_off_light[2] % 2 == 0) {
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
    if (airplane_ass1 == NULL) { /* allocate a quadric object, if necessary */
        airplane_ass1 = gluNewQuadric();
        gluQuadricDrawStyle(airplane_ass1, GLU_FILL);
        gluQuadricNormals(airplane_ass1, GLU_SMOOTH);
    }
    if (airplane_ass2 == NULL) { /* allocate a quadric object, if necessary */
        airplane_ass2 = gluNewQuadric();
        gluQuadricDrawStyle(airplane_ass2, GLU_FILL);
        gluQuadricNormals(airplane_ass2, GLU_SMOOTH);
    }
    glPushMatrix();
    glTranslatef(5.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    gluCylinder(airplane_ass1, 0.75, 0.0, /* radius of top and bottom circle */
                6.0,                      /* height of the cylinder */
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    glTranslatef(0.0, 0.0, 5.0);
    gluCylinder(airplane_ass2, 0.0, 0.55, /* radius of top and bottom circle */
                2.0,                      /* height of the cylinder */
                12, /* use 12-side polygon approximating circle*/
                3); /* Divide it into 3 sections */
    //????????????
    glTranslatef(-0.5, 0.0, 0.0);
    if (sphere_ass == NULL) {
        sphere_ass = gluNewQuadric();
        gluQuadricDrawStyle(sphere_ass, GLU_FILL);
    }
    glColor3f(0.95, 0.2, 0.2);
    gluSphere(sphere_ass, 0.5, /* radius=2.0 */
              12,              /* composing of 12 slices*/
              12);             /* composing of 12 stacks */
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(90, 0.0, 1.0, 0.0);
    if (position[1] > 0) {
        glRotatef(bld_ang, 0.0, 0.0,
                  1.0); /* Rotate about x axis, M5 coord. sys. */
    }
    glScalef(0.15, 0.15, 0.15);
    glNormal3fv(normal[5]);
    draw_blade(); /* draw the first blade */  //??????y??????

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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (cylind == NULL) { /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    gluQuadricTexture(cylind, GL_TRUE);

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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----- Draw a unit sphere ---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }
    gluQuadricTexture(sphere, GL_TRUE);
    glColor3f(0.95, 0.2, 0.2);
    gluSphere(sphere, 1.0, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    glDisable(GL_TEXTURE_2D);
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

    if (position[1] > 0) {
        glRotatef(bld_ang, 0.0, 0.0,
                  1.0); /* Rotate about x axis, M5 coord. sys. */
    }
    draw_blade(); /* draw the first blade */  //??????y??????

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
void display() {
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (show_case) {
        case Orthographical_x:
            make_projection(Orthographical_x);
            glViewport(0, 0, width, height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_x);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();
            break;
        case Orthographical_y:
            make_projection(Orthographical_y);
            glViewport(0, 0, width, height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_y);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();
            break;
        case Orthographical_z:
            make_projection(Orthographical_z);
            glViewport(0, 0, width, height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_z);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();
            break;
        case Perspective:
            make_projection(Perspective);
            glViewport(0, 0, width, height);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();      /* Initialize modelview matrix */
            my_view(Perspective);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();
            break;
        case show_All:
            make_projection(Perspective);
            glViewport(width / 2, 0, width / 2, height / 2);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();      /* Initialize modelview matrix */
            my_view(Perspective);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();

            make_projection(Orthographical_x);
            glViewport(0, height / 2, width / 2, height / 2);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_x);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();

            make_projection(Orthographical_y);
            glViewport(width / 2, height / 2, width / 2, height / 2);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_y);  //<<<<<<<<<<<<<<<<<<<<<<
            draw_window();

            make_projection(Orthographical_z);
            glViewport(0, 0, width / 2, height / 2);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();           /* Initialize modelview matrix */
            my_view(Orthographical_z);  //<<<<<<<<<<<<<<<<<<<<<<
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
void my_reshape(int w, int h) {
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-15.0, 15.0, -15.0 * (float)h / (float)w,
                15.0 * (float)h / (float)w, -10.5, 30.0);
    else
        glOrtho(-15.0 * (float)w / (float)h, 15.0 * (float)w / (float)h, -15.0,
                15.0, -10.5, 30.0);
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void SpecialKey(GLint key, GLint x, GLint y) {
    //????????????
    if (key == GLUT_KEY_UP) {
        position[1] += Step;
        if (leg_len > 0) {
            leg_len -= 0.2;
        }
    }
    if (key == GLUT_KEY_DOWN) {
        if (position[1] > 0) {
            position[1] -= Step;
            if (leg_len < 2) {
                leg_len += 0.2;
            }
        }
    }
    if (key == GLUT_KEY_LEFT) {
        head_forward += 8.0;
    }
    if (key == GLUT_KEY_RIGHT) {
        head_forward -= 8.0;
    }
    display();
}
void my_keyboard(unsigned char key, int x, int y) {
    // if(key=='Q'||key=='q') exit(0);
    //  if(key=='r'){
    //    arm_ang += 5.0;
    //    if(arm_ang>360.0) arm_ang -= 360.0;
    //  }else if(key=='c'){
    //    bld_ang += 5.0;
    //    if(bld_ang>360.0) bld_ang -=360.0;
    //  }
    if (key == 'w') {
        position[0] -= Step * cos(head_forward * PI / 180.0);
        position[2] += Step * sin(head_forward * PI / 180.0);
        // position[0] -= Step;
        state = 1;
    }
    if (key == 's') {
        position[0] += Step * cos(head_forward * PI / 180.0);
        position[2] -= Step * sin(head_forward * PI / 180.0);
        // position[0] += Step
        state = 2;
    }
    if (key == 'q') {
        head_forward += 10;
        position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
        position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
        state = 3;
    }
    if (key == 'e') {
        head_forward -= 10;
        position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
        position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
        state = 4;
    }
    if (key == 'f') {
        int i = 0;
        for (i = 0; i < 20; i++) {
            head_forward += 10;
            position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
            position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
            state = 3;
            Sleep(30);
            display();
        }
        for (i = 0; i < 30; i++) {
            head_forward -= 10;
            position[0] -= 3 * Step * cos((head_forward)*PI / 180.0);
            position[2] += 3 * Step * sin((head_forward)*PI / 180.0);
            state = 4;
            Sleep(30);
            display();
        }
    }
    switch (key) {
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
    if (key == 'n') {
        zoom += 0.5;
    } else if (key == 'm') {
        zoom -= 0.5;
    } else if (key == '/') {
        lit0_angle += 3;
    }

    if (key == 'c' || key == 'C') {
        control_light_num++;
        control_light_num %= 3;
        printf("control light %d\n", control_light_num);
    } else if (key == 'v' || key == 'V') {
        if (control_light_num == 0) {
            if (turn_on_off_light[0]++ % 2 == 0)
                glDisable(GL_LIGHT0);
            else
                glEnable(GL_LIGHT0);
        } else if (control_light_num == 1) {
            if (turn_on_off_light[1]++ % 2 == 0)
                glDisable(GL_LIGHT1);
            else
                glEnable(GL_LIGHT1);
        } else if (control_light_num == 2) {
            if (turn_on_off_light[2]++ % 2 == 0)
                glDisable(GL_LIGHT2);
            else
                glEnable(GL_LIGHT2);
        }
        printf("Turn off/on control light %d\n", control_light_num);
    }
    switch (key)  // control_light_num = 0 : red point ligth
    {             // small:decrease large:increase
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
            for (int i = 0; i < 3; i++) {
                light_color[control_light_num][i] *= 0.9;
            }
            break;
        case 'Z':
            for (int i = 0; i < 3; i++) {
                light_color[control_light_num][i] *= 1.1;
            }
            break;
        default:
            break;
    }
    /*-----Define light0 ---------*/
    // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    if (control_light_num == 0) {
        /*Define light0*/
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[0]);
        // printf("R = %f; G = %f; B = %f\n", light_color[0][0],
        // light_color[0][1], light_color[0][2]); glLightfv(GL_LIGHT0,
        // GL_SPECULAR, lit_specular); glLightf(GL_LIGHT0, GL_SPOT_EXPONENT,
        // lit_exponent);
    } else if (control_light_num == 1) {
        /*-----Define light1 ---------*/
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color[1]);
        // printf("R = %f; G = %f; B = %f\n", light_color[1][0],
        // light_color[1][1], light_color[1][2]); glLightfv(GL_LIGHT1,
        // GL_SPECULAR, lit1_specular);
    } else if (control_light_num == 2) {
        /*----Define light2 -----*/
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color[2]);
        // printf("R = %f; G = %f; B = %f\n", light_color[2][0],
        // light_color[2][1], light_color[2][2]); glLightfv(GL_LIGHT2,
        // GL_SPECULAR, lit2_specular); glLightf(GL_LIGHT2, GL_SPOT_CUTOFF,
        // lit2_cutoff); glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    }

    switch (key) {
        case '!':
            modify_state = 0;  // sun light direction
            printf("Now can modify sun light direction\n");
            break;
        case '@':
            modify_state = 1;  // helicopter direction
            printf("Now can modify helicopter light direction\n");
            break;
        case '#':
            modify_state = 2;  // helicopter exponent
            printf("Now can modify helicopter light exponenet\n");
            break;
        case '$':
            modify_state = 3;  // helicopter cutoff angle
            printf("Now can modify helicopter light cutoff angle\n");
            break;
        default:
            break;
    }

    if (key == '+' && modify_state == 0) {
        rotate_dirction_lit1 += 2;
    } else if (key == '_' && modify_state == 0) {
        rotate_dirction_lit1 -= 2;
    }

    if (key == '+' && modify_state == 1) {
        rotate_dirction_lit2 += 2;
    } else if (key == '_' && modify_state == 1) {
        rotate_dirction_lit2 -= 2;
    }

    if (key == '+' && modify_state == 2) {
        lit2_exponent *= 2;
    } else if (key == '_' && modify_state == 2) {
        lit2_exponent /= 2;
    }

    if (key == '+' && modify_state == 3) {
        lit2_cutoff += 2;
    } else if (key == '_' && modify_state == 3) {
        lit2_cutoff -= 2;
    }

    if (key == '8') {
        sun_auto++;
    } else if (key == '9') {
        redball_auto++;
    } else if (key == '0') {
        spot_auto++;
    }
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);

    if (key == '`') {  //???flog??????
        change_flog++;
        if (change_flog % 4 == 0) {
            printf("Now fog is exp.\n");
            glEnable(GL_FOG); /*enable fog fade */
            glFogi(GL_FOG_MODE,
                   GL_EXP); /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
        } else if (change_flog % 4 == 1) {
            printf("Now fog is exp2.\n");
            glEnable(GL_FOG); /*enable fog fade */
            glFogi(GL_FOG_MODE,
                   GL_EXP2); /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
        } else if (change_flog % 4 == 2) {
            printf("Now fog is linear.\n");
            glEnable(GL_FOG); /*enable fog fade */
            glFogi(GL_FOG_MODE,
                   GL_LINEAR); /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
        } else if (change_flog % 4 == 3) {
            printf("Now no fog.\n");
            glDisable(GL_FOG); /*enable fog fade */
        }
    } else if (key == '6') {  //??????flog?????????
        fog_color[0] += 0.01;
        fog_color[2] += 0.01;
        glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */
    } else if (key == '^') {
        fog_color[0] -= 0.01;
        fog_color[2] -= 0.01;
        glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */
    } else if (key == '7') {
        FOG_DENSITY += 0.01;
        FOG_END -= 5;
        glFogf(GL_FOG_DENSITY, FOG_DENSITY); /*fog opacity(density)= 0.25*/
        glFogf(GL_FOG_END, FOG_END);
    } else if (key == '&') {
        FOG_DENSITY -= 0.01;
        FOG_END += 5;
        glFogf(GL_FOG_DENSITY, FOG_DENSITY); /*fog opacity(density)= 0.25*/
        glFogf(GL_FOG_END, FOG_END);
    } else if (key == '~') {
        glReadBuffer(GL_FRONT);
        glBindTexture(GL_TEXTURE_2D, textName[5]);
        air_body_on = 1;
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    }
}

/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func() {
    bld_ang += 5.0;
    mo_angle += 0.3;
    anggg += 0.01;
    if (bld_ang > 360.0) {
        bld_ang -= 360.0;
        arm_ang += 3.0;
        if (arm_ang > 360.0)
            arm_ang -= 360.0;
    }

    if (sun_auto % 2 == 0) {
    } else {
        sun_auto_color++;
        if (sun_auto_color % 100 == 33) {
            light_color[0][0] = 1.0;
            light_color[0][1] = 1.0;
            light_color[0][2] = 1.0;
        } else if (sun_auto_color % 100 == 66) {
            light_color[0][0] = 1.0;
            light_color[0][1] = 0.0;
            light_color[0][2] = 1.0;
        } else if (sun_auto_color % 100 == 99) {
            light_color[0][0] = 0.0;
            light_color[0][1] = 1.0;
            light_color[0][2] = 1.0;
        }
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[0]);
    }

    if (redball_auto % 2 == 0) {
    } else {
        redball_auto_color++;
        if (redball_auto_color % 100 == 33) {
            light_color[1][0] = 1.0;
            light_color[1][1] = 0.0;
            light_color[1][2] = 0.0;
        } else if (redball_auto_color % 100 == 66) {
            light_color[1][0] = 0.0;
            light_color[1][1] = 1.0;
            light_color[1][2] = 0.0;
        } else if (redball_auto_color % 100 == 99) {
            light_color[1][0] = 0.0;
            light_color[1][1] = 0.0;
            light_color[1][2] = 1.0;
        }
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color[1]);
    }

    if (spot_auto % 2 == 0) {
    } else {
        spot_auto_color++;
        if (spot_auto_color % 100 == 33) {
            light_color[2][0] = 1.0;
            light_color[2][1] = 0.0;
            light_color[2][2] = 1.0;
        } else if (spot_auto_color % 100 == 66) {
            light_color[2][0] = 1.0;
            light_color[2][1] = 1.0;
            light_color[2][2] = 0.0;
        } else if (spot_auto_color % 100 == 99) {
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
void main(int argc, char** argv) {
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
