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


GLfloat windmillMDLMatrix[4][16];
GLfloat bladecenterMatrix[16];
GLfloat bladerotationMatrix[16];
GLfloat bladeMDLMatrix[4][16];

float distance;
float rotspeed;
float windspeed;


// Reference to shader program
GLuint program;

enum windmill_parts_t {
    WALLS = 0,
    BALCONY = 1,
    ROOF = 2,
    WINDMILL_BASE = 3
};

Model* windmill[4];
Model* blades[4];

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
    distance = 25.0;
    rotspeed = 0.0;
    windspeed = 0.0;
    GLfloat work[2][16];

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

	// Upload geometry to the GPU:
    windmill[WALLS] = LoadModelPlus("windmill/windmill-walls.obj", program, "inPosition", "inNormal", "inTexCoord");
    windmill[BALCONY] = LoadModelPlus("windmill/windmill-balcony.obj", program, "inPosition", "inNormal", "inTexCoord");
    windmill[ROOF] = LoadModelPlus("windmill/windmill-roof.obj", program, "inPosition", "inNormal", "inTexCoord");

    T(0,9.1,4.5,work[1]);
    Ry(-M_PI_2,work[0]);
    Mult(work[0], work[1], bladecenterMatrix);

    {
        int i = 0;
        for(i = 0; i < 4; ++i) {
            T(0,0,0,bladeMDLMatrix[i]);
            Ry(100*M_PI/180,work[0]);
            Mult(work[0], bladeMDLMatrix[i], work[1]);
            Rz(M_PI_2*i,work[0]);
            Mult(work[0], work[1], bladeMDLMatrix[i]);
            blades[i] = LoadModelPlus("windmill/blade.obj", program, "inPosition", "inNormal", "inTexCoord");
        }
    }


	printError("init models");
    T(0,0,0,windmillMDLMatrix[WINDMILL_BASE]);
    T(0,0,0,windmillMDLMatrix[WALLS]);
    T(0,0,0,windmillMDLMatrix[BALCONY]);
    T(0,0,0,windmillMDLMatrix[ROOF]);



    //LoadTGATextureSimple("maskros512.tga", &bunnyTextureID);
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

void display(void)
{

	float t = glutGet(GLUT_ELAPSED_TIME)/500.0;

    static float last_t = 0;
    static float angle = 0;
    static float millangle = 0;

    float dt = t - last_t; last_t = t;
    angle += dt*rotspeed/3;
    millangle += dt*windspeed/3;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Rz(millangle, bladerotationMatrix);
    GLfloat bladeBaseMatrix[16];
    Mult(bladecenterMatrix, bladerotationMatrix, bladeBaseMatrix);


    lookAt(distance*cos(angle),10,distance*sin(angle),
           0,5,0,
           0,1,0,
           camMatrix);

	//myMatrix[3] += dir*0.01;

	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);


    {
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_TRUE, bladeBaseMatrix);
        int i = 0;
        for(i = 0; i < 4; ++i) {
            glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladeMDLMatrix[i]);
            DrawModel(blades[i]);
        }
    }

	glUniform1f(glGetUniformLocation(program, "t"), t);


    {
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_TRUE, windmillMDLMatrix[WINDMILL_BASE]);
        int i = 0;
        for(i = 0; i < 3; ++i) {
            glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, windmillMDLMatrix[i]);
            DrawModel(windmill[i]);
        }
    }

   	//glBindTexture(GL_TEXTURE_2D, bunnyTextureID);
   	//glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0


	printError("display");

	glutSwapBuffers();
}

void keyboard(unsigned char key,int x, int y) {
    if(key == 's')
        distance += .1;
    else if(key == 'w')
        distance -= .1;
    else if(key == 'a')
        rotspeed -= .1;
    else if(key == 'd')
        rotspeed += .1;
    else if(key == 'x')
        windspeed -= .05;
    else if(key == 'c')
        windspeed += .05;

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (800, 600);
    glutInitWindowPosition (10, 10);
	glutCreateWindow ("Lab 2 lol");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
