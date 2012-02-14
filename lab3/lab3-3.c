// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"
#include "loadobj.h"
#include "math.h"
#include "LoadTGA.c"
#include "VectorUtils2.h"

// Globals
// Data would normally be read from files

// These are generally in math.h, but the compiler must be pleased...
# define M_PI           3.14159265358979323846  /* pi */
# define M_PI_2         1.57079632679489661923  /* pi/2 */


#define NUMBER_OF_WINDMILLS 10
#define MILL_RADIUS 20
#define MILL_SCALE 0.8

#define near 1.0
#define far 50.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };

GLfloat camMatrix[16];
GLfloat work[2][16];

Point3D position;
Point3D look_at;
float windspeed;
int mouse_click[2];

#define GROUND_SIZE 100.0f
GLfloat groundGlyphPosition[] = {-GROUND_SIZE, 0.0f, GROUND_SIZE,
                                GROUND_SIZE, 0.0f, GROUND_SIZE,
                                GROUND_SIZE, 0.0f, -GROUND_SIZE,
                                -GROUND_SIZE, 0.0f, -GROUND_SIZE};

GLuint groundGlyphIndices[] = {0, 1, 2, 2, 3, 0};

GLfloat groundTexturePos[] = {0, 0,
                              100, 0,
                              100, 100,
                              0, 100};
GLuint groundVertexArrayObjectID;
Model* skybox;

enum programs_t {
    WINDMILL_PROGRAM,
    GROUND_PROGRAM,
    SKYBOX_PROGRAM,
    NUMBER_OF_PROGRAMS
};
GLuint programs[NUMBER_OF_PROGRAMS];

typedef struct {
    Model* windmill[4];
    Model* blades[4];

    GLfloat windmillMDLMatrix[4][16];
    GLfloat bladecenterMatrix[16];
    GLfloat bladerotationMatrix[16];
    GLfloat bladeMDLMatrix[4][16];

    float bladeangle;
} windmill_t;

enum windmill_parts_t {
    WALLS,
    BALCONY,
    ROOF,
    WINDMILL_BASE,
    NUMBER_OF_WINDMILL_PARTS
};

enum textures {
    GROUND_TEXTURE,
    SKYBOX_TEXTURE,
    NUMBER_OF_TEXTURES
};

GLuint billboards[NUMBER_OF_TEXTURES];

void init_windmill(windmill_t* w, int n)
{
    programs[WINDMILL_PROGRAM] = loadShaders("lab3-3.vert", "lab3-3.frag");

    w->bladeangle = 0;
    w->windmill[WALLS] = LoadModelPlus("windmill/windmill-walls.obj", programs[WINDMILL_PROGRAM], "inPosition", "inNormal", "inTexCoord");
    w->windmill[BALCONY] = LoadModelPlus("windmill/windmill-balcony.obj", programs[WINDMILL_PROGRAM], "inPosition", "inNormal", "inTexCoord");
    w->windmill[ROOF] = LoadModelPlus("windmill/windmill-roof.obj", programs[WINDMILL_PROGRAM], "inPosition", "inNormal", "inTexCoord");

    T(0.05,9.15,4.5,work[1]);
    Ry(-M_PI_2,work[0]);
    Mult(work[0], work[1], w->bladecenterMatrix);

    {
        int i = 0;
        for(i = 0; i < 4; ++i) {
            T(0,0,0,w->bladeMDLMatrix[i]);
            Ry(100*M_PI/180,work[0]);
            Mult(work[0], w->bladeMDLMatrix[i], work[1]);
            Rz(M_PI_2*i,work[0]);
            Mult(work[0], work[1], w->bladeMDLMatrix[i]);
            w->blades[i] = LoadModelPlus("windmill/blade.obj", programs[WINDMILL_PROGRAM], "inPosition", "inNormal", "inTexCoord");
        }
    }

    T((MILL_RADIUS + MILL_RADIUS/(n+1.0))*cos(2*M_PI/(double)NUMBER_OF_WINDMILLS*n),0,(MILL_RADIUS + MILL_RADIUS/(n+1.0))*sin(2*M_PI/(double)NUMBER_OF_WINDMILLS*n),work[0]);
    double scale = 10*MILL_SCALE/(n+10.0);
    S(scale, scale, scale, work[1]);
    Mult(work[0], work[1], w->windmillMDLMatrix[WINDMILL_BASE]);
    T(0,0,0,w->windmillMDLMatrix[WALLS]);
    T(0,0,0,w->windmillMDLMatrix[BALCONY]);
    T(0,0,0,w->windmillMDLMatrix[ROOF]);
	printError("init windmill");
}

void draw_windmill(windmill_t* w, float dt)
{
    glUseProgram(programs[WINDMILL_PROGRAM]);

	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "camMatrix"), 1, GL_TRUE, camMatrix);


    w->bladeangle += dt*windspeed/3;

	Rz(w->bladeangle, w->bladerotationMatrix);
    GLfloat bladeBaseMatrix[16];
    Mult(w->windmillMDLMatrix[WINDMILL_BASE], w->bladecenterMatrix, work[0]);
    Mult(work[0], w->bladerotationMatrix, bladeBaseMatrix);

    {
        glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "baseMatrix"), 1, GL_TRUE, bladeBaseMatrix);
        int i = 0;
        for(i = 0; i < 4; ++i) {
            glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "mdlMatrix"), 1, GL_TRUE, w->bladeMDLMatrix[i]);
            DrawModel(w->blades[i]);
        }
    }

    {
        glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "baseMatrix"), 1, GL_TRUE, w->windmillMDLMatrix[WINDMILL_BASE]);
        int i = 0;
        for(i = 0; i < 3; ++i) {
            glUniformMatrix4fv(glGetUniformLocation(programs[WINDMILL_PROGRAM], "mdlMatrix"), 1, GL_TRUE, w->windmillMDLMatrix[i]);
            DrawModel(w->windmill[i]);
        }
    }

	printError("display windmill");
}
windmill_t my_windmills[NUMBER_OF_WINDMILLS];



void init_skybox(void)
{
    skybox = LoadModelPlus("skybox.obj", programs[GROUND_PROGRAM], "inPosition", "inNormal", "inTexCoord");
    LoadTGATextureSimple("SkyBox512.tga", &billboards[SKYBOX_TEXTURE]);
}

void draw_skybox(void)
{
	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(programs[GROUND_PROGRAM], "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programs[GROUND_PROGRAM], "camMatrix"), 1, GL_TRUE, camMatrix);

    glBindTexture(GL_TEXTURE_2D, billboards[SKYBOX_TEXTURE]);
   	glUniform1i(glGetUniformLocation(programs[GROUND_PROGRAM], "texUnit"), 0); // Texture unit 0
   	glDisable(GL_DEPTH_TEST);
	DrawModel(skybox);
	glEnable(GL_DEPTH_TEST);
}

void init_ground(void)
{
    LoadTGATextureSimple("grass.tga", &billboards[GROUND_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLuint vertexBufferID, indexBufferID, texCoordBufferID;

    glGenVertexArrays(1, &groundVertexArrayObjectID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);
	glGenBuffers(1, &texCoordBufferID);

	glBindVertexArray(groundVertexArrayObjectID);

    // VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundGlyphPosition), groundGlyphPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(programs[GROUND_PROGRAM], "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(programs[GROUND_PROGRAM], "inPosition"));

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexturePos), groundTexturePos, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(programs[GROUND_PROGRAM], "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
	printError("init ground3");
	glEnableVertexAttribArray(glGetAttribLocation(programs[GROUND_PROGRAM], "inTexCoord"));
	printError("init ground4");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundGlyphIndices), groundGlyphIndices, GL_STATIC_DRAW);

	printError("init ground");
}

void draw_ground(void)
{
    glBindTexture(GL_TEXTURE_2D, billboards[GROUND_TEXTURE]);
   	glUniform1i(glGetUniformLocation(programs[GROUND_PROGRAM], "texUnit"), 0); // Texture unit 0

   	glBindVertexArray(groundVertexArrayObjectID);	// Select VAO
	glDrawElements(GL_TRIANGLES, sizeof(groundGlyphIndices)/sizeof(groundGlyphIndices[0]), GL_UNSIGNED_INT, 0L);

	printError("display ground");
}

void init_billboard(void)
{
	programs[GROUND_PROGRAM] = loadShaders("billboard.vert", "billboard.frag");
	printError("load billboard shader");
    init_skybox();
    init_ground();
}
void draw_billboard(void)
{
	glUseProgram(programs[GROUND_PROGRAM]);

	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(programs[GROUND_PROGRAM], "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programs[GROUND_PROGRAM], "camMatrix"), 1, GL_TRUE, camMatrix);

    draw_skybox();
    draw_ground();
}


void lookAt(GLfloat px, GLfloat py, GLfloat pz,
                    GLfloat lx, GLfloat ly, GLfloat lz,
                    GLfloat vx, GLfloat vy, GLfloat vz,
                    GLfloat *m)
{
    Point3D p, l, n, v, u;
    GLfloat r[16], t[16];

    SetVector(px, py, pz, &p);
    SetVector(lx, ly, lz, &l);
    SetVector(vx, vy, vz, &v);

    VectorSub(&p,&l,&n);
    Normalize(&n);

    CrossProduct(&v,&n,&u);
    Normalize(&u);


    CrossProduct(&n,&u,&v);

    m[0] = u.x;
    m[1] = u.y;
    m[2] = u.z;
    m[3] = -DotProduct(&u,&p);
    m[4] = v.x;
    m[5] = v.y;
    m[6] = v.z;
    m[7] = -DotProduct(&v,&p);
    m[8] = n.x;
    m[9] = n.y;
    m[10] = n.z;
    m[11] = -DotProduct(&n,&p);
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
// Place for your code
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
    position.x = 0;position.y = 10;position.z = 10;
    windspeed = 0.0;
    look_at.x = 0.0;look_at.y = 5.0;look_at.z = 0.0;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	printError("GL inits");

	// Load and compile shader
	printError("init shader");

    init_billboard();
    for(int i = 0; i < NUMBER_OF_WINDMILLS; ++i) {
        init_windmill(&my_windmills[i], i);
	}

    //LoadTGATextureSimple("maskros512.tga", &bunnyTextureID);
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

void display(void)
{
	float t = glutGet(GLUT_ELAPSED_TIME)/500.0;
    static float last_t = 0;
    float dt = t - last_t; last_t = t;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    lookAt(position.x,position.y,position.z,
           look_at.x,look_at.y,look_at.z,
           0,1,0,
           camMatrix);

	//myMatrix[3] += dir*0.01;

    draw_billboard();
    for(int i = 0; i < NUMBER_OF_WINDMILLS; ++i) {
        draw_windmill(&my_windmills[i], dt);
	}

   	//glBindTexture(GL_TEXTURE_2D, bunnyTextureID);
   	//glUniform1i(glGetUniformLocation(programs[WINDMILL_PROGRAM], "texUnit"), 0); // Texture unit 0

	glutSwapBuffers();
}

void keyboard(unsigned char key,int x, int y) {
    Point3D diff;
    float scale = 0.4;
    if(key == 'w') {
        VectorSub(&look_at, &position, &diff);
        Normalize(&diff);
        ScalarMult(&diff, scale, &diff);
        VectorAdd(&look_at, &diff, &look_at);
        VectorAdd(&position, &diff, &position);
    } else if(key == 's') {
        VectorSub(&position, &look_at, &diff);
        Normalize(&diff);
        ScalarMult(&diff, scale, &diff);
        VectorAdd(&look_at, &diff, &look_at);
        VectorAdd(&position, &diff, &position);
    } else if(key == 'd') {
        Point3D y; y.x = 0; y.z = 0; y.y = 1;
        VectorSub(&position, &look_at, &diff);
        CrossProduct(&y, &diff, &diff);
        Normalize(&diff);
        ScalarMult(&diff, scale, &diff);
        VectorAdd(&look_at, &diff, &look_at);
        VectorAdd(&position, &diff, &position);
    } else if(key == 'a') {
        Point3D y; y.x = 0; y.z = 0; y.y = 1;
        VectorSub(&position, &look_at, &diff);
        CrossProduct(&diff, &y, &diff);
        Normalize(&diff);
        ScalarMult(&diff, scale, &diff);
        VectorAdd(&look_at, &diff, &look_at);
        VectorAdd(&position, &diff, &position);
    } else if(key == 'x') {
        windspeed -= .05;
    } else if(key == 'c') {
        windspeed += .05;
    }
}

void mousedrag(int x, int y) {
    static const double scale = 10.0;
    static const double fiscale = 10.0;

    Point3D diff;
    diff.x = look_at.x - position.x;
    diff.y = look_at.y - position.y;
    diff.z = look_at.z - position.z;
    Ry((float)(x-mouse_click[0])/fiscale, &work[0]);
    MatrixMultPoint3D(&work[0], &diff, &diff);
    VectorAdd(&position, &diff, &look_at);
    look_at.y -= (float)(y-mouse_click[1])/scale;
    printf("Dragged to (%d,%d) yielding (%f,%f)\n", x, y, look_at.x, look_at.z);
    mouse_click[0] = x;
    mouse_click[1] = y;
}
void mouseclick(int button, int state, int x, int y) {
    mouse_click[0] = x;
    mouse_click[1] = y;
    printf("Clicked at (%d,%d)\n", x, y);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (800, 600);
    glutInitWindowPosition (10, 10);
	glutCreateWindow ("Lab 3 lol");
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousedrag);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
