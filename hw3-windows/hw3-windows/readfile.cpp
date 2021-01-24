/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  


using namespace std;
#include "readfile.h"
#include "variables.h"

// You may not need to use the following two functions, but it is provided
// here for convenience


void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename) 
{
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        numused = 0;
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd;
                float values[10]; // Position and color for light, colors for others
                                  // Up to 10 params for cameras.  
                bool validinput; // Validity of input 


                // Process the light, add it to database.
                // Vertex Command
                if (cmd == "vertex") {
                    validinput = readvals(s, 3, values); // Position/color for lts.
                    if (validinput) {
                        vertices.emplace_back(glm::vec4(values[0], values[1], values[2], 1.f));
                    }
                }
                else if (cmd == "tri") {
                    validinput = readvals(s, 3, values); // Position/color for lts.
                    if (validinput) {
                        glm::vec4 v0 = vertices[(int)values[0]];
                        glm::vec4 v1 = vertices[(int)values[1]];
                        glm::vec4 v2 = vertices[(int)values[2]];

                        Triangle tri_ = Triangle(v0, v1, v2);
                        tri_.ambient = ambient;
                        tri_.specular = specular;
                        tri_.emission = emission;
                        tri_.diffuse = diffuse;
                        tri_.shininess = shininess;
                        tri_.setTransfTri(transfstack.top());
                        triangles.push_back(tri_);
                    }
                }
                else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values); // Position/color for lts.
                    if (validinput) {
                        Sphere sph_(glm::vec3(values[0], values[1], values[2]), values[3]);
                        sph_.ambient = ambient;
                        sph_.specular = specular;
                        sph_.emission = emission;
                        sph_.diffuse = diffuse;
                        sph_.shininess = shininess;
                        sph_.transform = transfstack.top();
                        spheres.push_back(sph_);
                    }
                }
                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        ambient = glm::vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        diffuse = glm::vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        specular = glm::vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        emission = glm::vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        attenuation = glm::vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "output") {
                    s >> OutFilename;
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    if (validinput) {
                        width = (int)values[0]; height = (int)values[1];
                    }
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxVerts = values[0];
                    }
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxDepth = values[0];
                    }
                }
                else if (cmd == "maxvertsnorm") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxVertsNorm = values[0];
                    }
                }
                else if (cmd == "camera") {
                    validinput = readvals(s, 10, values); // 10 values eye cen up fov
                    if (validinput) {

                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly. 
                        // Set eyeinit upinit center fovy in variables.h 

                        eyeinit.x = values[0]; // lookfromx;
                        eyeinit.y = values[1]; // lookfromy;
                        eyeinit.z = values[2]; // lookfromz;

                        center.x = values[3]; // lookatx 
                        center.y = values[4]; // lookaty 
                        center.z = values[5]; // lookatz 

                        upinit.x = values[6]; // upx 
                        upinit.y = values[7]; // upy
                        upinit.z = values[8]; // upz 
                        upinit = Transform::upvector(upinit, eyeinit);

                        fovy = values[9];
                    }
                }
                else if (cmd == "translate") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        const glm::mat4 M = Transform::translate(values[0], values[1], values[2]);
                        rightmultiply(M, transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        const glm::mat4 M = Transform::scale(values[0], values[1], values[2]);
                        rightmultiply(M, transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        const glm::vec3 axis_ = glm::normalize(glm::vec3(values[0], values[1], values[2]));
                        const float angle = values[3]; // degrees
                        glm::mat4 M = glm::transpose(glm::mat4(Transform::rotate(angle, axis_)));

                        rightmultiply(M, transfstack);
                    }
                }
                else if (cmd == "directional" || cmd == "point") {
                    if (numused >= numLights) {
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                        continue;
                    }
                    else {
                        validinput = readvals(s, 6, values);
                        if (validinput) {
                            // light position:
                            lightposn[4 * numused] = values[0]; // x
                            lightposn[4 * numused + 1] = values[1]; // y
                            lightposn[4 * numused + 2] = values[2]; // z
                            if (cmd == "point")
                                lightposn[4 * numused + 3] = 1.f; // w
                            else
                                lightposn[4 * numused + 3] = 0.f; // w

                            // light color
                            lightcolor[4 * numused] = values[4]; // r
                            lightcolor[4 * numused + 1] = values[5]; // g
                            lightcolor[4 * numused + 2] = values[6]; // b
                            lightcolor[4 * numused + 3] = 1.f; // a

                            ++numused;
                        }

                    }
                }
                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                    }
                }
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
                }
                getline(in, str);
            }

            // Set up initial position for eye, up and amount
            // As well as booleans 

            eye = eyeinit;
            up = upinit;
            sx = sy = 1.0;  // keyboard controlled scales in x and y 
            tx = ty = 0.0;  // keyboard controllled translation in x and y  


        }
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
