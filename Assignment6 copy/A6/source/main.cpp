#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Water.h"

#endif

const int WIDTH = 1200, HEIGHT = 1200;
Camera camera(45.0f, 45.0f, 10.0f, glm::vec3(0, 0, 0));
std::map<int, bool> keys;


float ol = 16.0f, il = 16.0f; // For Tessellation

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.cursor_callback(window, xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		keys[GLFW_KEY_UP] = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		keys[GLFW_KEY_UP] = false;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		keys[GLFW_KEY_DOWN] = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		keys[GLFW_KEY_DOWN] = false;


	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		il++;
		ol++;
	}	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		il--;
		ol--;
		if (il < 1.0f)
			il = 1.0f;
		if (ol < 1.0f)
			ol = 1.0f;
	}
}

int main() {

	// Initialize keys
	keys[GLFW_KEY_UP] = false;
	keys[GLFW_KEY_DOWN] = false;
	
	if (!glfwInit()) {
		std::cout << "GLFW failed to init" << std::endl;
		return -1;
	}
    glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
         glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "A6", NULL, NULL);

	if (!window) {
		std::cout << "Window failed to init" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW failed to init" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	ShaderProgram tess_shader("/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/shaders/VerShader.vs", "/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/shaders/FragShader.fs", "/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/shaders/GeoShader.gs",
		"/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/shaders/tessCS.tcs", "/Users/david.olaniyan/Desktop/CS3388/Assignment6/A6/shaders/tessES.tes");
	Water water(-20, 20, 1);

	glm::vec3 lightDir(1, -0.3, 1);
	glm::mat4 projection = glm::perspective(45.0f, WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	double last = glfwGetTime(), current;
	double deltaTime;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// DeltaTime
		current = glfwGetTime();
		deltaTime = current - last;
		last = current;

		// UPDATE
		camera.update(deltaTime);
		glm::mat4 view = camera.getViewMatrix();

		// RENDER
		glClearColor(176/255.0f, 228/255.0f, 251/255.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		tess_shader.setUniform1f("TessLevelInner", ol);
		tess_shader.setUniform1f("TessLevelOuter", il);
		tess_shader.setUniform3fv("lightDir", lightDir);
		tess_shader.setUniform3fv("eyeDir", camera.eye);
		tess_shader.setUniform1f("time", (float)glfwGetTime());
		water.draw(&tess_shader, view, projection);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
