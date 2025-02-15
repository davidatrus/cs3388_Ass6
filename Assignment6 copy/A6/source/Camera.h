#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>

extern std::map<int, bool> keys;

struct Camera {

	// Spherical coordinate camera
	float theta; // x/z plane angle (degrees)
	float phi; // y/(x/z) plane angle (degrees)
	float radius;

	glm::vec3 eye;
	glm::vec3 target;

	Camera(float theta, float phi, float radius, glm::vec3 target): 
		theta(theta), phi(phi), radius(radius), target(target), eye({ 0, 0, 0 }) {}

	void update(float deltaTime) {

		if (keys[GLFW_KEY_DOWN])
			radius = std::min(radius + 8 * deltaTime, 100.0f);
		if (keys[GLFW_KEY_UP])
			radius = std::max(radius - 8 * deltaTime, 0.1f);

		// Euler angle calculations for camera.
		eye.x = radius * sin(glm::radians(theta)) * sin(glm::radians(phi));
		eye.y = radius * cos(glm::radians(phi));
		eye.z = radius * cos(glm::radians(theta)) * sin(glm::radians(phi));
		eye += target;
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(eye, target, glm::vec3(0, 1, 0));
	}

	//  Mouse funcs
	double oldMx = -1, oldMy = -1;
	#define FIRST_MOVE -1
	void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
		//  set oldMx and oldMy to FIRST_MOVE
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			oldMx = FIRST_MOVE; oldMy = FIRST_MOVE;
			return;
		}

		float dx = xpos - oldMx;
		float dy = ypos - oldMy;

		// check for if first move
		if (oldMx == FIRST_MOVE || oldMy == FIRST_MOVE) {
			dx = 0;
			dy = 0;
		}

		float sensitivity = 0.15f;
		theta -= dx * sensitivity;
		phi -= dy * sensitivity;

		if (phi > 179.5f)
			phi = 179.5f;
		if (phi < 0.1f)
			phi = 0.1f;

		oldMx = xpos; oldMy = ypos;
	}
};

#endif
