#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <glut.h>

#pragma comment(lib, "glew32.lib") // ugh

/* Globals */
GLuint program;
GLint attribute_coord2d;
unsigned int keys = 0;
bool update = false;
double scale = 1.0;
double x_pos = 0.0;
double y_pos = 0.0;
int max_iteration = 512;
double x_max = 1.0;
double x_min = -2.5;
double y_max = 1.0;
double y_min = -1.0;

int frame = 0;
int time = 0;
int timebase = 0;
 
int init_resources(void)
{
  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
 
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const char *vs_source = 
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"  // OpenGL ES 2.0
#else
    "#version 430\n"  // OpenGL 2.1
#endif
    "attribute vec2 coord2d;                  "
    "void main(void) {                        "
    "  gl_Position = vec4(coord2d, 0.0, 0.0); "
    "}";
  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (0 == compile_ok)
  {
    fprintf(stderr, "Error in vertex shader\n");
    return 0;
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs_source =
    "#version 430           \n                                             "
	"uniform int Scale;                                                    "
	"uniform double x_max;"
	"uniform double x_min;"
	"uniform double y_max;"
	"uniform double y_min;"
    "void main(void) {                                                     "
	"  double x0 = ((double(gl_FragCoord.x) * (x_max - x_min)) / 1920.0) + x_min;  "
	"  double y0 = ((double(gl_FragCoord.y) * (y_max - y_min)) / 1080.0) + y_min;  "
	"  double hello = 0.0;                                                  "
	"  double x = 0.0; "
	"  double y = 0.0;                                                      "
	"  double xtemp = 0.0;                                                  "
	"  int iteration = 0;                                            "
	"  int max_iteration = Scale;                                      "
	"  vec2 z = vec2(0.0, 0.0);                                         "
	"  while (z.x + z.y < 65536.0 && iteration < max_iteration)          "
	"  {                                                             "
	"     xtemp = z.x - z.y + x0;                                    "
	"     y = 2.0*x*y + y0;                                            "
	"     x = xtemp;                                                 "
	"     iteration = iteration + 1;                                 "
	"     z = vec2(x*x, y*y);                                         "
	"  }                                                             "
	"  double d = 1.0 - (double(iteration) / double(max_iteration));    "
	"  gl_FragColor = vec4(d, d, d, 1.0); "
    "}";
  glShaderSource(fs, 1, &fs_source, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in fragment shader\n");
    return 0;
  }

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    return 0;
  }

  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  return 1;
}
 
void onDisplay()
{
	

  /* Clear the background as white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
 
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord2d);
  GLfloat triangle_vertices[] = {
     0.0,  0.8,
    -0.8, -0.8,
     0.8, -0.8,
  };
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glVertexAttribPointer(
    attribute_coord2d, // attribute
    2,                 // number of elements per vertex, here (x,y)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    triangle_vertices  // pointer to the C array
  );
 
  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(attribute_coord2d);

  GLint loc = glGetUniformLocation(program, "x_max");
  if (loc != -1)
  {
	  glUniform1d(loc, x_max);
  }
  loc = glGetUniformLocation(program, "x_min");
  if (loc != -1)
  {
	  glUniform1d(loc, x_min);
  }
  loc = glGetUniformLocation(program, "y_max");
  if (loc != -1)
  {
	  glUniform1d(loc, y_max);
  }
  loc = glGetUniformLocation(program, "y_min");
  if (loc != -1)
  {
	  glUniform1d(loc, y_min);
  }
  loc = glGetUniformLocation(program, "Scale");
  if (loc != -1)
  {
	  glUniform1i(loc, max_iteration);
  }
  else
  {
	  //std::cout << "error" << std::endl;
  }
 
  /* Display the result */
  glutSwapBuffers();

  frame++;
  time = glutGet(GLUT_ELAPSED_TIME);

  if (time - timebase > 1000) 
  {
	  double fps = (double)frame*(1000.0/(double)(time-timebase));
	  timebase = time;
	  frame = 0;
	  std::cout << fps << std::endl;
  }
}
 
void free_resources()
{
  glDeleteProgram(program);
}

static void special(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_LEFT:
		keys |= 1;
		break;
	case GLUT_KEY_RIGHT:
		keys |= 2;
		break;
	case GLUT_KEY_UP:
		keys |= 4;
		break;
	case GLUT_KEY_DOWN:
		keys |= 8;
		break;
	case GLUT_KEY_PAGE_UP:
		keys |= 16;
		break;
	case GLUT_KEY_PAGE_DOWN:
		keys |= 32;
		break;
	case GLUT_KEY_HOME:
		keys |= 64;
		break;
	case GLUT_KEY_END:
		keys |= 128;
		break;
	}
	update = true;
}

void myUpdate(int te)
{
	if (update) 
	{
		if (keys & 1)
			x_pos = x_pos - 0.5 * scale;
		if (keys & 2)
			x_pos = x_pos + 0.5 * scale;
		if (keys & 4)
			y_pos = y_pos + 0.5 * scale;
		if (keys & 8)
			y_pos = y_pos - 0.5 * scale;
		if (keys & 16)
			max_iteration = max_iteration + 100;
		if (keys & 32)
			if (max_iteration > 100)
				max_iteration = max_iteration - 100;
		if (keys & 64)
			scale = scale * 2.0;
		if (keys & 128)
			scale = scale / 2.0;

	}

	keys = 0;

	x_max = 1.0 * scale + x_pos;
	x_min = -2.5 * scale + x_pos;
	y_max = 1.0 * scale + y_pos;
	y_min = -1.0 * scale + y_pos;

	update = false;
	glutPostRedisplay();
	glutTimerFunc(5, myUpdate, 1);
}
 
int main(int argc, char* argv[])
{
  /* Glut-related initialising functions */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("My First Triangle");
 
  /* Extension wrangler initialising */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return EXIT_FAILURE;
  }
 
  /* When all init functions run without errors,
  the program can initialise the resources */
  if (1 == init_resources())
  {
    /* We can display it if everything goes OK */
    glutDisplayFunc(onDisplay);
	glutSpecialFunc(special);
	glutTimerFunc(5, myUpdate, 1);
    glutMainLoop();
  }
 
  /* If the program exits in the usual way,
  free resources and exit with a success */
  free_resources();

  char wait;
  std::cin >> wait;

  return EXIT_SUCCESS;
}