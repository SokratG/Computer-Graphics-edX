#pragma once
#include <glm/glm.hpp>

enum shape { sphere, triangle };
// using shape = enum shape;

#pragma warning(push)
#pragma warning(disable : 26812)
struct TypeShape {
public:
	shape shape_;
	TypeShape(shape shp_) : shape_(shp_){}
#pragma warning(pop)
};

struct Triangle : public TypeShape {
public:
	glm::vec3 ambient = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 diffuse = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 specular = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 emission = glm::vec3(0.f, 0.f, 0.f);
	float shininess = 0.f;
	glm::mat4 transform = glm::mat4(1.f);
	Triangle(const glm::vec4 p1, const glm::vec4 p2, const glm::vec4 p3) : TypeShape(shape::triangle), P1(p1), P2(p2), P3(p3) { recalcNormal(); }
	glm::vec3 getTriNormal() { return TriNormal; }
	glm::vec4 getP1() const { return P1; }
	glm::vec4 getP2() const { return P2; }
	glm::vec4 getP3() const { return P3; }
	void setP1(const glm::vec4 p) { P1 = p; recalcNormal(); }
	void setP2(const glm::vec4 p) { P2 = p; recalcNormal(); }
	void setP3(const glm::vec4 p) { P3 = p; recalcNormal(); }
	void setTransfTri(const glm::mat4 m, bool apply_=false) {
		transform = m;
		recalcNormal(apply_);
	}
	glm::vec3 getTriNormal() const { return TriNormal; }
private:
	void recalcNormal(bool useTransf=false) {
		if (useTransf) {
			P1 = transform * P1;
			P2 = transform * P2;
			P3 = transform * P3;
		}
		glm::vec3 A = glm::vec3(P1.x, P1.y, P1.z);
		glm::vec3 B = glm::vec3(P2.x, P2.y, P2.z);
		glm::vec3 C = glm::vec3(P3.x, P3.y, P3.z);
		glm::vec3 normal_ = glm::normalize(glm::cross((B - A), (C - A)));
		TriNormal = normal_;
	}
	glm::vec4 P1, P2, P3;
	glm::vec3 TriNormal;
};


struct Sphere : public TypeShape {
public:
	glm::vec3 ambient = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 diffuse = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 specular = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 emission = glm::vec3(0.f, 0.f, 0.f);
	float shininess = 0.f;
	glm::mat4 transform = glm::mat4(1.f);
	Sphere(glm::vec3 c, float r) : TypeShape(shape::sphere), center(c), radius(r) {}

	glm::vec3 getSphNormal(const glm::vec3& P) {
		glm::vec4 P_ = glm::inverse(transform) * glm::vec4(P.x, P.y, P.z, 1.f);
		glm::vec3 dehP = (glm::vec3(P_.x, P.y, P.z) / P_.w); // dehomogeneous 
		glm::vec3 normal = glm::normalize(dehP - center);
		glm::vec4 sphnormal = glm::transpose(glm::inverse(transform)) * glm::vec4(normal.x, normal.y, normal.z, 0);
		normal = glm::normalize(glm::vec3(sphnormal.x, sphnormal.y, sphnormal.z));
		return normal;
	}
	glm::vec3 getCenter() const { return center; }
	float gerRad() const { return radius; }
private:
	glm::vec3 center;
	float radius;
};