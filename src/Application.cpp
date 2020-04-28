//#include<iostream>
//#include <GLFW/glfw3.h>
//
//#include "Utils\Vec2D.h"
//
//int main(void)
//{
//    Vec2D vec(2, 5.6f);
//    std::cout << vec << std::endl;
//
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}

#include<iostream>
#include<sstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* APP_TITLE = "OpenGL Window";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gFullScreen = false;
GLFWwindow* gWindow = NULL;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"

"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"	frag_color = vec4(0.35f, 0.96f, 0.3f, 1.0f);"

"}";



void glfw_onkey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
	if (!initOpenGL()) {
		std::cerr << "GLFW initialization failed :(" << std::endl;
		return -1;
	}

	GLfloat vertices[] = {
		 0.0f,  0.5f, 0.0f, //top
		 0.5f, -0.5f, 0.0f, //right
		-0.5f, -0.5f, 0.0f // left
	};

	GLuint vbo, vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error vertex shader failed to compile" << infoLog << std::endl;

	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);


	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error fragment shader failed to compile" << infoLog << std::endl;

	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error shader program linker failure" << infoLog << std::endl;

	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	// main loop
	while (!glfwWindowShouldClose(gWindow)) {

		showFPS(gWindow);

		glfwPollEvents();

		glClearColor(0.23f, 0.38f, 0.47f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(gWindow);
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}

bool initOpenGL()
{
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed :(" << std::endl;
		return false;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/*GLFWwindow* pWindow = NULL;*/

	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL) {
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else
	{
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}


	if (gWindow == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, glfw_onkey);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLFW initialization failed :(" << std::endl;
		return false;
	}
	return true;
}

void glfw_onkey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void showFPS(GLFWwindow* window)
{
	static double previousSecons = 0.0;
	static int frameCount = 0;

	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns	number of seconds since GLFW started as a double

	elapsedSeconds = currentSeconds - previousSecons;

	if (elapsedSeconds > 0.25) {
		previousSecons = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msFrame = 1000.0 / fps;

		std::ostringstream outs;

		outs.precision(3);
		outs << std::fixed
			<< APP_TITLE << "   "
			<< "FPS: " << fps << "   "
			<< "Frame Time " << msFrame << " (ms)";

		glfwSetWindowTitle(window, outs.str().c_str());
		frameCount = 0;
	}

	frameCount++;
}
