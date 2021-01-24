#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Geometry.h"
#define EXTERN extern 


static glm::vec3 eye; // The (regularly updated) vector coordinates of the eye 
static glm::vec3 up;  // The (regularly updated) vector coordinates of the up 

static glm::vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
static glm::vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
static glm::vec3 center(0.0, 0.0, 0.0); // Center look at point 

const int numLights = 10;
static size_t numused;
static int amountinit;
static int width, height;
static float fovy;
static size_t maxVerts;
static size_t maxVertsNorm;
static size_t maxDepth = 5;



static float sx, sy; // the scale in x and y 
static float tx, ty; // the translation in x and y

static glm::mat4 projection, modelview; // The mvp matrices
static std::string OutFilename;
static std::vector<Triangle> triangles;
static std::vector<Sphere> spheres;
static std::vector<glm::vec4> vertices;
static float lightposn[numLights * 4];
static float lightcolor[numLights * 4];


static enum { view, translate, scale } transop; // which operation to transform 


static glm::vec3 attenuation = glm::vec3(1.f, 0.f, 0.f);
static glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
static glm::vec3 diffuse;
static glm::vec3 specular;
static glm::vec3 emission;
static float shininess;