#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

// We gave this code in one of the tutorials, so leaving it here too
void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom) {
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setNormals(cpuGeom.normals);
	gpuGeom.setTexCoords(cpuGeom.texCoords);
}

// EXAMPLE CALLBACKS
class Assignment4 : public CallbackInterface {

public:
	Assignment4() : camera(0.0, 0.0, 2.0), aspect(1.0f) {
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
	}
	virtual void mouseButtonCallback(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				rightMouseDown = true;
			} else if (action == GLFW_RELEASE) {
				rightMouseDown = false;
			}
		}
	}
	virtual void cursorPosCallback(double xpos, double ypos) {
		if (rightMouseDown) {
			double dx = xpos - mouseOldX;
			double dy = ypos - mouseOldY;
			camera.incrementTheta(dy);
			camera.incrementPhi(dx);
		}
		mouseOldX = xpos;
		mouseOldY = ypos;
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
		camera.incrementR(yoffset);
	}
	virtual void windowSizeCallback(int width, int height) {
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width,  height);
		aspect = float(width)/float(height);
	}

	void viewPipeline(ShaderProgram &sp) {
		glm::mat4 M = glm::mat4(1.0);
		glm::mat4 V = camera.getView();
		glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);

		GLint location = glGetUniformLocation(sp, "light");
		glm::vec3 light = camera.getPos();
		glUniform3fv(location, 1, glm::value_ptr(light));

		GLint uniMat = glGetUniformLocation(sp, "M");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(M));
		uniMat = glGetUniformLocation(sp, "V");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(V));
		uniMat = glGetUniformLocation(sp, "P");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(P));
	}

	Camera camera;

private:

	bool rightMouseDown = false;
	float aspect;
	double mouseOldX;
	double mouseOldY;

};

void colouredTriangles(CPU_Geometry &geom) {
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.cols.push_back(glm::vec3(1.0, 0.0, 0.0));
}

void positiveZFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.5));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
}

void positiveXFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
}

void negativeZFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
}

void negativeXFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
}

void positiveYFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
}

void negativeYFace(std::vector<glm::vec3> const &originQuad, CPU_Geometry &geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
	for(auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
}

void drawSphere(float r, CPU_Geometry& geom) {

	std::vector<std::vector<glm::vec3 >> quads;
	int i = 0;

	for (float theta = 0; theta <= M_PI; theta += (M_PI / 32.0f)) {
		for (float phi = 0; phi <= M_PI * 2.0f; phi += ((M_PI) / 32.0f)) {
			quads.push_back(std::vector<glm::vec3>());
			quads[i].push_back(glm::vec3(
				r * sin(phi) * sin(theta),
				r * cos(phi) * sin(theta),
				r * cos(theta)
			));
		}
		i++;
	}
	float sec = quads.size() / quads[1].size();

	for (int j = 0; j < sec - 1; j++) {
		float a = quads[i].size();
		for (int k = 0; k < a; k++) {
			geom.verts.push_back(quads[j][k]);
			geom.verts.push_back(quads[j+1][k]);
			geom.verts.push_back(quads[j][k+1]);
			geom.verts.push_back(quads[j+1][k]);
			geom.verts.push_back(quads[j+1][k+1]);
			geom.verts.push_back(quads[j][k+1]);

			geom.normals.push_back(glm::vec3{ quads[j][k].x / r, quads[j][k].y / r, quads[j][k].z / r });
			geom.normals.push_back(glm::vec3{ quads[j+1][k].x / r, quads[j+1][k].y / r, quads[j+1][k].z / r });
			geom.normals.push_back(glm::vec3{ quads[j][k+1].x / r, quads[j][k+1].y / r, quads[j][k+1].z / r });
			geom.normals.push_back(glm::vec3{ quads[j+1][k].x / r, quads[j+1][k].y / r, quads[j+1][k].z / r });
			geom.normals.push_back(glm::vec3{ quads[j+1][k+1].x / r, quads[j+1][k + 1].y / r, quads[j + 1][k + 1].z / r });
			geom.normals.push_back(glm::vec3{ quads[j][k + 1].x / r, quads[j][k + 1].y / r, quads[j][k + 1].z / r });
		}
	}
}

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired


	GLDebug::enable();

	// CALLBACKS
	auto a4 = std::make_shared<Assignment4>();
	window.setCallbacks(a4);


	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// The current CPU_Geometry and GPU_Geometry classes are defined in
	// Geometry.h/Geometry.cpp They will work for this assignment, but for some of
	// the bonuses you may have to modify them.

	std::vector<glm::vec3> originQuad;
	originQuad.push_back(glm::vec3{-0.5, 0.5, 0}); // top-left
	originQuad.push_back(glm::vec3{-0.5, -0.5, 0}); // bottom-left
	originQuad.push_back(glm::vec3{0.5, 0.5, 0}); // top-right

	originQuad.push_back(glm::vec3{-0.5, -0.5, 0}); // bottom-left
	originQuad.push_back(glm::vec3{0.5, -0.5, 0}); // bottom-right
	originQuad.push_back(glm::vec3{0.5, 0.5, 0}); // top-right

	CPU_Geometry square;
	positiveZFace(originQuad, square);
	positiveXFace(originQuad, square);
	negativeZFace(originQuad, square);
	negativeXFace(originQuad, square);
	positiveYFace(originQuad, square);
	negativeYFace(originQuad, square);

	//square.cols.resize(square.verts.size(), glm::vec3{1.0, 0.0, 0.0});
	colouredTriangles(square);
	colouredTriangles(square);
	colouredTriangles(square);
	colouredTriangles(square);
	colouredTriangles(square);
	colouredTriangles(square);


	for(auto i = square.verts.begin(); i < square.verts.end(); ++i) {
		std::cout << *i << std::endl;
	}

	GPU_Geometry quads;
	updateGPUGeometry(quads, square);

	CPU_Geometry earth;
	GPU_Geometry earthGeom;
	Texture eT("textures\earth.jpg", GL_NEAREST);
	drawSphere(0.3f, earth);
	updateGPUGeometry(earthGeom, earth);

	CPU_Geometry moon;
	GPU_Geometry moonGeom;
	Texture mT("textures\moon.jpg", GL_NEAREST);
	drawSphere(0.08f, moon);
	updateGPUGeometry(moonGeom, moon);

	CPU_Geometry sun;
	GPU_Geometry sunGeom;
	Texture sT("textures\sun.jpg", GL_NEAREST);
	drawSphere(1.f, sun);
	updateGPUGeometry(sunGeom, sun);

	CPU_Geometry stars;
	GPU_Geometry starsGeom;
	Texture bT("textures\stars.jpg", GL_NEAREST);
	drawSphere(200.f, stars);
	updateGPUGeometry(starsGeom, stars);



	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader.use();

		a4->viewPipeline(shader);

		earthGeom.bind();
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(earth.verts.size()));


		moonGeom.bind();
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(moon.verts.size()));

		sunGeom.bind();
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(sun.verts.size()));

		starsGeom.bind();
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(stars.verts.size()));


		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
