#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RayTrace.h"


using namespace std;
using namespace glm;

Sphere::Sphere(vec3 c, float r, int ID){
	centre = c;
	radius = r;
	id = ID;
}
//------------------------------------------------------------------------------
// This is part 2 of your assignment. At the moment, the spheres are not showing
// up. Implement this method to make them show up.
//
// Make sure you set all of the appropriate fields in the Intersection object.
//------------------------------------------------------------------------------
//referenced https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

bool solveQuadratic(const float& a, const float& b, const float& c, float& t0, float& t1)
{
	float q;
	float discr = b * b - 4 * a * c;
	if (discr < 0) {
		return false;
	}
	else if (discr == 0) {
		t0 = t1 = -0.5 * b / a;
	}
	else {
		if (b > 0) {
			q = -0.5f * (b + sqrt(discr));
		}
		else {
			q = -0.5 * (b - sqrt(discr));
		}
		t0 = q / a;
		t1 = c / q;
	}

	if (t0 > t1) {
		std::swap(t0, t1);
	}
	return true;
}

Intersection Sphere::getIntersection(Ray ray){
	Intersection i{};
	float t0, t1;
	glm::vec3 L = ray.origin - centre;
	float a = dot(ray.direction, ray.direction);
	float b = 2 * dot(ray.direction, L);
	float c = dot(L, L) - (radius * radius);

	if (!solveQuadratic(a, b, c, t0, t1)) {
		i.numberOfIntersections = 0;
		return i;
	}

	if (t0 > t1) {
		std::swap(t0, t1);
	}
	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) {
			i.numberOfIntersections = 0;
			return i;
		}
	}
	i.id = id;
	i.material = material;
	i.point = ray.origin + ray.direction * t0;
	i.normal = normalize(i.point - centre);
	i.numberOfIntersections = 1;

	return i;

	// You are required to implement this intersection.
	//
	// NOTE: You _must_ set these values appropriately for each case:
	//
	// No Intersection:
	// i.numberOfIntersections = 0;
	//
	// Intersection:
	// i.normal = **the normal at the point of intersection **
	// i.point = **the point of intersection**
	// i.numberOfIntersections = 1; // for a single intersection
	//
	// If you get fancy and implement things like refraction, you may actually
	// want to track more than one intersection. You'll need to change
	// The intersection struct in that case.
}

Plane::Plane(vec3 p, vec3 n, int ID){
	point = p;
	normal = n;
	id = ID;
}


float dot_normalized(vec3 v1, vec3 v2){
	return glm::dot(glm::normalize(v1), glm::normalize(v2));
}

void debug(char* str, vec3 a){
	cout << "debug:" << str << ": " << a.x <<", " << a.y <<", " << a.z << endl;
}
// --------------------------------------------------------------------------
void Triangles::initTriangles(int num, vec3 * t, int ID){
	id = ID;
	for(int i = 0; i< num; i++){
		triangles.push_back(Triangle(*t, *(t+1), *(t+2)));
		t+=3;
	}
}

Intersection Triangles::intersectTriangle(Ray ray, Triangle triangle){
	// From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	const float EPSILON = 0.0000001;
	auto vertex0 = triangle.p1;
	auto vertex1 = triangle.p2;
	auto vertex2 = triangle.p3;

	glm::vec3 edge1, edge2, h, s, q;
	float a,f,u,v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(ray.direction, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) {
		return Intersection{}; // no intersection
	}
	f = 1.0/a;
	s = ray.origin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0) {
		return Intersection{}; // no intersection
	}
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.direction, q);
	if (v < 0.0 || u + v > 1.0) {
		return Intersection{}; // no intersection
	}
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	// ray intersection
	if (t > EPSILON) {
		Intersection p;
		p.point = ray.origin + ray.direction * t;
		p.normal = glm::normalize(glm::cross(edge1, edge2));
		p.material = material;
		p.numberOfIntersections = 1;
		p.id = id;
		return p;
	} else {
		// This means that there is a line intersection but not a ray intersection.
		return Intersection{}; // no intersection
	}
}


Intersection Triangles::getIntersection(Ray ray){
	Intersection result{};
	result.material = material;
	result.id = id;
	float min = 9999;
	result = intersectTriangle(ray, triangles.at(0));
	if(result.numberOfIntersections!=0)min = glm::distance(result.point, ray.origin);
	for(int i = 1; i<triangles.size() ;i++){
		Intersection p = intersectTriangle(ray, triangles.at(i));
		if(p.numberOfIntersections !=0 && glm::distance(p.point, ray.origin) < min){
			min = glm::distance(p.point, ray.origin);
			result = p;
		}
	}

	result.material = material;
	result.id = id;
	return result;
}

Intersection Plane::getIntersection(Ray ray){
	Intersection result;
	result.material = material;
	result.id = id;
	result.normal = normal;
	if(dot(normal, ray.direction)>=0)return result;
	float s = dot(point - ray.origin, normal)/dot(ray.direction, normal);
	//if(s<0.00001)return result;
	result.numberOfIntersections = 1;
	result.point = ray.origin + s*ray.direction;
	return result;
}


