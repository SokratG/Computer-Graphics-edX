#pragma once
#include <glm/glm.hpp>
#define EXTERN extern 


int amount; // The amount of rotation for each arrow press
glm::vec3 eye; // The (regularly updated) vector coordinates of the eye 
glm::vec3 up;  // The (regularly updated) vector coordinates of the up 

glm::vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
glm::vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
glm::vec3 center(0.0, 0.0, 0.0); // Center look at point 


int amountinit;
int w, h;
float fovy;

float sx, sy; // the scale in x and y 
float tx, ty; // the translation in x and y

mat4 projection, modelview; // The mvp matrices

static enum { view, translate, scale } transop; // which operation to transform 
enum shape { sphere, triangle };