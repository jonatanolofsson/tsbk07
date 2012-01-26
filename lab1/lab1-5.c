// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"
#include "math.h"

// Globals
// Data would normally be read from files
GLfloat vertices[8][3] = {	{-0.5f,	-0.5f,	-0.5f},
				{0.5f,	-0.5f,	-0.5f},
				{0.5f,	0.5f,	-0.5f},
				{-0.5f,	0.5f,	-0.5f},
			
				{-0.5f,	-0.5f,	0.5f},
				{0.5f,	-0.5f,	0.5f},
				{0.5f,	0.5f,	0.5f},
				{-0.5f,	0.5f,	0.5f}};

GLuint indices[] = { 
0,3,2, 
0,2,1,

2,3,7,
2,7,6,

0,4,7,
0,7,3,

1,2,6,
1,6,5,

4,5,6,
4,6,7,

0,1,5,
0,5,4
};


GLfloat colors[] = {	1.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,
			0.0f,1.0f,1.0f,
			1.0f,0.0f,1.0f,
			
			1.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,
			1.0f,0.0f,1.0f,
			0.0f,1.0f,1.0f};

GLfloat myMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

// vertex array object
unsigned int vertexArrayObjID;
// Color array 
unsigned int colorArrayObjID;
// Reference to shader program
GLuint program;


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int indexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// Upload color data
	glGenBuffers(1, &colorBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

	// Upload color data
	glGenBuffers(1, &indexBufferObjID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// End of upload of geometry
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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

	double t = glutGet(GLUT_ELAPSED_TIME)/500.0;
	//myMatrix[5] = myMatrix[10] = cos(t);
	//myMatrix[6] = -sin(t);
	//myMatrix[9] = sin(t);
	myMatrix[0] = myMatrix[10] = cos(t);
	myMatrix[8] = -sin(t);
	myMatrix[2] = sin(t);
	
	//myMatrix[3] += dir*0.01;
	
	// Send in additional params
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_FALSE, myMatrix);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}
