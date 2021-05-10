#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"
#include <cmath>

struct point {
	float x;
	float y;
};

struct State {
	int segments = 1;
	int scene = 1;
	bool operator == (State const& other) const {
		return segments == other.segments && scene == other.scene;
	}
};

class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			if (key == GLFW_KEY_R) {
				shader.recompile();
			}
			if (key == GLFW_KEY_LEFT) {
				if (state.segments > 0) {
					state.segments--;
				}
			}
			if (key == GLFW_KEY_RIGHT) {
				if (state.segments < 8) {
					state.segments++;
				}
			}
			if (key == GLFW_KEY_1) {
				state.scene = 1;
			}
			if (key == GLFW_KEY_2) {
				state.scene = 2;
			}
			if (key == GLFW_KEY_3) {
				state.scene = 3;
			}
			if (key == GLFW_KEY_4) {
				state.scene = 4;
			}

		}
	}
	State getState() {
		return state;
	}

private:
	ShaderProgram& shader;
	State state;
};

void drawTriangle(CPU_Geometry &cpuGeom, point a, point b, point c) {
	// vertices
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(c.x, c.y, 0.f));

	// colours (these should be in linear space)
	cpuGeom.cols.push_back(glm::vec3((a.x + 1.0f) / 2.0f, (b.x + 1.0f) / 2.0f, (c.y + 1.0f) / 2.0f));
	cpuGeom.cols.push_back(glm::vec3((a.x + 1.0f) / 2.0f, (b.x + 1.0f) / 2.0f, (c.y + 1.0f) / 2.0f));
	cpuGeom.cols.push_back(glm::vec3((a.x + 1.0f) / 2.0f, (b.x + 1.0f) / 2.0f, (c.y + 1.0f) / 2.0f));
};

void Serpinski(point a, point b, point c, int i, CPU_Geometry &cpuGeom) {
	point d, e, f;
	if (i > 0) {
		d.x = (a.x + b.x) * 0.5f;
		d.y = (a.y + b.y) * 0.5f;

		e.x = (b.x + c.x) * 0.5f;
		e.y = (b.y + c.y) * 0.5f;

		f.x = (c.x + a.x) * 0.5f;
		f.y = (c.y + a.y) * 0.5f;

		Serpinski(d, b, e, i - 1, cpuGeom);
		Serpinski(a, d, f, i - 1, cpuGeom);
		Serpinski(f, e, c, i - 1, cpuGeom);
	}

		else {
			drawTriangle(cpuGeom, a, b, c);

	};
};

void initTriangle(point& a, point& b, point& c) {
	a.x = -1.0f;
	a.y = -1.0f;

	b.x = 0.0f;
	b.y = 1.0f;

	c.x = 1.0f;
	c.y = -1.0f;
};

void initSquare(point& a, point& b, point& c, point& d) {
	a.x = -0.5f;
	a.y = -0.5f;

	b.x = -0.5f;
	b.y = 0.5f;

	c.x = 0.5f;
	c.y = 0.5f;

	d.x = 0.5f;
	d.y = -0.5f;
};

void drawSquare(CPU_Geometry& cpuGeom, point a, point b, point c, point d, int i) {
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(c.x, c.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(c.x, c.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(d.x, d.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(d.x, d.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));

		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
		cpuGeom.cols.push_back(glm::vec3(0.0f, 1.0f/i, 0.f));
	
};

void drawSquare2(CPU_Geometry& cpuGeom, point a, point b, point c, point d, int i) {
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(c.x, c.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(c.x, c.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(d.x, d.y, 0.f));

	cpuGeom.verts.push_back(glm::vec3(d.x, d.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));

	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f / i, 0.0f, 0.f));

};


void nestSquares(CPU_Geometry& cpuGeom, point a, point b, point c, point d, int i) {
	point e, f, g, h;
	if (i >= 0) {

		e.x = (a.x + b.x) * 0.5f;
		e.y = (a.y + b.y) * 0.5f;

		f.x = (b.x + c.x) * 0.5f;
		f.y = (b.y + c.y) * 0.5f;

		g.x = (c.x + d.x) * 0.5f;
		g.y = (c.y + d.y) * 0.5f;

		h.x = (d.x + a.x) * 0.5f;
		h.y = (d.y + a.y) * 0.5f;


		drawSquare(cpuGeom, a, b, c, d, i);
		drawSquare2(cpuGeom, e, f, g, h, i);

		a.x = (e.x + f.x) * 0.5f;
		a.y = (e.y + f.y) * 0.5f;

		b.x = (f.x + g.x) * 0.5f;
		b.y = (f.y + g.y) * 0.5f;

		c.x = (g.x + h.x) * 0.5f;
		c.y = (g.y + h.y) * 0.5f;

		d.x = (h.x + e.x) * 0.5f;
		d.y = (h.y + e.y) * 0.5f;

		nestSquares(cpuGeom, a, b, c, d, i - 1);
	}
};

void drawLine(CPU_Geometry& cpuGeom, point a, point b) {
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));

	cpuGeom.cols.push_back(glm::vec3(0.0f, a.x, 1.0f));
	cpuGeom.cols.push_back(glm::vec3(0.0f, b.y, 1.0f));

};

void drawLine2(CPU_Geometry& cpuGeom, point a, point b) {
	cpuGeom.verts.push_back(glm::vec3(a.x, a.y, 0.f));
	cpuGeom.verts.push_back(glm::vec3(b.x, b.y, 0.f));

	cpuGeom.cols.push_back(glm::vec3(1.0f, 0.f, 0.f));
	cpuGeom.cols.push_back(glm::vec3(1.0f, 0.f, 0.f));

};

void snowFlakeLine(CPU_Geometry& cpuGeom, point a, point b, int i) {
	point c, d, e, f, g;

	if (i > 0) {
		f.x = b.x - a.x;
		f.y = b.y - a.y;
		g.x = a.y - b.y;
		g.y = b.x - a.x;

		c.x = a.x * 2.0f / 3.0f + b.x * 1.0f / 3.0f;
		c.y = a.y * 2.0f / 3.0f + b.y * 1.0f / 3.0f;

		d.x = a.x * 1.0f / 3.0f + b.x * 2.0f / 3.0f;
		d.y = a.y * 1.0f / 3.0f + b.y * 2.0f / 3.0f;

		e.x = a.x + 1.0f / 2.0f * f.x + sqrt(3.0f) / 6.0f * g.x;
		e.y = a.y + 1.0f / 2.0f * f.y + sqrt(3.0f) / 6.0f * g.y;

		snowFlakeLine(cpuGeom, a, c, i - 1);
		snowFlakeLine(cpuGeom, c, e, i - 1);
		snowFlakeLine(cpuGeom, e, d, i - 1);
		snowFlakeLine(cpuGeom, d, b, i - 1);
	}
	else {
		drawLine(cpuGeom,a, b);
	}
};

void snowFlakeLine2(CPU_Geometry& cpuGeom, point a, point b, int i) {
	point c, d, e, f, g;

	if (i > 0) {
		f.x = b.x - a.x;
		f.y = b.y - a.y;
		g.x = a.y - b.y;
		g.y = b.x - a.x;

		c.x = a.x * 2.0f / 3.0f + b.x * 1.0f / 3.0f;
		c.y = a.y * 2.0f / 3.0f + b.y * 1.0f / 3.0f;

		d.x = a.x * 1.0f / 3.0f + b.x * 2.0f / 3.0f;
		d.y = a.y * 1.0f / 3.0f + b.y * 2.0f / 3.0f;

		e.x = a.x + 1.0f / 2.0f * f.x + sqrt(3.0f) / 6.0f * g.x;
		e.y = a.y + 1.0f / 2.0f * f.y + sqrt(3.0f) / 6.0f * g.y;

		snowFlakeLine2(cpuGeom, a, c, i - 1);
		snowFlakeLine2(cpuGeom, c, e, i - 1);
		snowFlakeLine2(cpuGeom, e, d, i - 1);
		snowFlakeLine2(cpuGeom, d, b, i - 1);
	}
	else {
		drawLine2(cpuGeom, a, b);
	}
};


void snowFlake(CPU_Geometry& cpuGeom, point a, point b, point c, int i) {
	snowFlakeLine(cpuGeom, a, b, i);
	snowFlakeLine(cpuGeom, b, c, i);
	snowFlakeLine(cpuGeom, c, a, i);

};

void snowFlake2(CPU_Geometry& cpuGeom, point a, point b, point c, int i) {
	snowFlakeLine2(cpuGeom, a, b, i);
	snowFlakeLine2(cpuGeom, b, c, i);
	snowFlakeLine2(cpuGeom, c, a, i);

};

void initSnowFlake(point& a, point& b, point& c) {
	a.x = -0.5f;
	a.y = -0.5f;
	b.x = 0.f;
	b.y = 0.5f;
	c.x = 0.5f;
	c.y = -0.5f;
};

void dragoncurve(CPU_Geometry& cpuGeom, point a, point b, int i) {
	point o, c;
	if (i > 0) {
		o.x = (a.x + b.x) * 0.5f;
		o.y = (a.y + b.y) * 0.5f;
		c.x = ((a.x-o.x) * float(cos(M_PI*0.5)) - (a.y - o.y)* float(sin(M_PI * 0.5)) + o.x);
		c.y = ((a.x - o.x) * float(sin(M_PI * 0.5)) +  (a.y - o.y)* float(cos(M_PI * 0.5))  + o.y);

		dragoncurve(cpuGeom, a, c, i - 1);
		dragoncurve(cpuGeom, b, c, i - 1);
	}
	else drawLine(cpuGeom, a, b);
};


int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453 Assignment 1"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	std::shared_ptr<MyCallbacks> callbacks = std::make_shared<MyCallbacks>(shader); // can also update callbacks to new ones
	window.setCallbacks(callbacks);

	// GEOMETRY
	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

	State state;

	point a, b, c;
	initTriangle(a, b, c);
	Serpinski(a, b, c, state.segments, cpuGeom);

	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();
		if (!(state == callbacks->getState())) {
			state = callbacks->getState();
			cpuGeom.verts.clear();
			cpuGeom.cols.clear();
			if (state.scene == 1) {
				point a, b, c;
				initTriangle(a, b, c);
				Serpinski(a, b, c, state.segments, cpuGeom);
			}
			if (state.scene == 2) {
				point a, b, c, d;
				initSquare(a, b, c, d);
				nestSquares(cpuGeom, a, b, c, d, state.segments+1);
			}
			if (state.scene == 3) {
				point a, b, c;
				initSnowFlake(a, b, c);
				snowFlake2(cpuGeom, a, b, c, (state.segments - 1));
				snowFlake(cpuGeom, a, b, c, state.segments);
			}
			if (state.scene == 4) {
				point a, b;
				a.x = -0.5f;
				a.y = 0.f;
				b.x = 0.5f;
				b.y = 0.f;
				dragoncurve(cpuGeom, a, b, state.segments);
				
			}
			gpuGeom.setVerts(cpuGeom.verts);
			gpuGeom.setCols(cpuGeom.cols);
			
		}



		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (state.scene == 1) {
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(cpuGeom.verts.size()));
		}

		if (state.scene == 2|| state.scene == 3 || state.scene == 4) {
			glDrawArrays(GL_LINES, 0, GLsizei(cpuGeom.verts.size()));
		}
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
