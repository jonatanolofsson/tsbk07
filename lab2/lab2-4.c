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


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };

GLfloat camMatrix[] = { 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, -2.0,
                        0.0f, 0.0f, 0.0f, 1.0f };


GLfloat mdlMatrix[] = {  1.0f, 0.0f, 0.0f, 2.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mdlMatrix2[] = {  1.0f, 0.0f, 0.0f, -2.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };
GLfloat myMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };
GLfloat myMatrix2[] = { 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, -1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

// vertex array object
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

GLuint bunnyTextureID;
GLuint cubeTextureID;

// Reference to shader program
GLuint program;

Model *m;
Model *m2;

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

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-4.vert", "lab2-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:
    m = LoadModelPlus("bunnyplus.obj", program, "inPosition", "inNormal", "inTexCoord");

    m2 = LoadModelPlus("cubeplus.obj", program, "inPosition", "inNormal", "inTexCoord");


    LoadTGATextureSimple("maskros512.tga", &bunnyTextureID);
    LoadTGATextureSimple("dirt.tga", &cubeTextureID);
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");

}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//static char dir = 1;
	//if(myMatrix[3] > 1) dir = -1;
	//if(myMatrix[3] < -1) dir = 1;

	float t = glutGet(GLUT_ELAPSED_TIME)/500.0;
	//myMatrix[5] = myMatrix[10] = cos(t);
	//myMatrix[6] = -sin(t);
	//myMatrix[9] = sin(t);
	myMatrix[0] = myMatrix[10] = cos(t);
	myMatrix[8] = -sin(t);
	myMatrix[2] = sin(t);



    lookAt(10*cos(t/3.0),0,10*sin(t/3.0),
           0,0,0,
           0,1,0,
           camMatrix);

	//myMatrix[3] += dir*0.01;

	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniform1f(glGetUniformLocation(program, "t"), t);


   	glBindTexture(GL_TEXTURE_2D, bunnyTextureID);
   	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

    DrawModel(m);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix2);

    glBindTexture(GL_TEXTURE_2D, cubeTextureID);
	DrawModel(m2);

	printError("display");

	glutSwapBuffers();
}

void keyboard(unsigned char key,int x, int y) {
    GLfloat camZ = camMatrix[11];
    if(key == 'w')
        camZ += .1;
    else if(key == 's')
        camZ -= .1;



    camMatrix[11] = camZ;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Lab 2 lol");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	init ();
	glutMainLoop();
}
