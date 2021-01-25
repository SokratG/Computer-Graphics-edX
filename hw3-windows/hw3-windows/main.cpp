#include <limits>
#include "FreeImage.h"
#include "variables.h"
#include "readfile.h"
#define TEST

float fovy_r = 0.f;
float fovx_r = 0.f;
float aspect = 0.f;

#define CALCATTEN(d) return 1.0/(attenuation[0] + attenuation[1] * d + attenuation[2] * pow(d,2)); 

glm::vec3 RayThruPixel(int x, int y)
{
	// Canonical viewing geometry
	glm::vec3 posn = glm::normalize(eye - up);
	glm::vec3 u = glm::normalize(glm::cross(up, posn));
	glm::vec3 v = glm::cross(posn, u);
	
	float a = glm::tan((fovy_r / 2) * aspect * ((y + 0.5f * width/2)/(float)width/2));
	float b = glm::tan((fovy_r / 2) * ((height/2 - (x+0.5f))/(float)height/2));

	glm::vec3 res = a * u + b * v + posn;
	return res;
}

IntersectionInfo findIntersect(glm::vec3 p0, float maxDist, glm::vec3 dir, bool noIter=false, bool useNormal=false)
{
	TypeShape* obj = nullptr;
	float md = std::numeric_limits<float>::max(); // minimal distance
	bool isFound = false;
	IntersectionInfo res;
	for (auto& tri : triangles) {
		IntersectionInfo ii = tri.getIntersect(p0, dir);
		if (ii.isIntersect) {
			if ((ii.dist > 0) && (ii.dist < md) && (ii.dist < maxDist)) {
				md = ii.dist;
				obj = &tri;
				res.dist = md;
				res.shape_ = obj;
				res.isIntersect = true;
				res.vec_intersec = ii.vec_intersec;
				isFound = true;
				if (noIter)
					break;
			}
		}
	}
	if (!noIter && !isFound) {
		for (auto& sph : spheres) {
			IntersectionInfo ii = sph.getIntersect(p0, dir);
			if (ii.isIntersect) {
				if ((ii.dist > 0) && (ii.dist < maxDist) && (ii.dist < md)) {
					md = ii.dist;
					obj = &sph;
					res.dist = md;
					res.shape_ = obj;
					res.isIntersect = true;
					res.vec_intersec = ii.vec_intersec;
					isFound = true;
					if (noIter)
						break;
				}
			}
		}
	}
	if (isFound && res.isIntersect) {
		if (useNormal)
			if (res.shape_->typeshape == shape::triangle)
				res.normal = res.shape_->getNormal(res.vec_intersec);
			else
				res.normal = res.shape_->getNormal(res.vec_intersec);
	}

	return res;
}

void RGBPixel(BYTE col[3], const glm::vec3& lightcolor)
{
	col[0] = static_cast<BYTE>(lightcolor[2] * 255);
	col[1] = static_cast<BYTE>(lightcolor[1] * 255);
	col[2] = static_cast<BYTE>(lightcolor[0] * 255);
}



glm::vec3 computelight(const IntersectionInfo& iinfo, const glm::vec3& cam, int depthlevel)
{
	auto calcSpecLight = [=](glm::vec3 ray, glm::vec3 n) {
		glm::vec3 specdir = 2 * glm::dot(-ray, n) * n - (-ray);
		return glm::normalize(specdir);
	};
	
	// TODO
	if (depthlevel > 0) {
		for (size_t i = 0; i < numused; ++i) {

		}
	}



	return glm::vec3();
}

void render(BYTE* pixels)
{
	aspect = (float)width / (float)height;
	fovy_r = glm::radians(fovy);
	fovx_r = aspect * fovy_r;
	// TODO: add some padding

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			BYTE col[3]{ 0.f, 0.f, 0.f };
			size_t offset = 3 * (x * width + y);
			glm::vec3 ray = RayThruPixel(x, y);
			IntersectionInfo ii = findIntersect(eye, std::numeric_limits<float>::max(), ray, false, true);
			if (ii.isIntersect) {
				glm::vec3 lightcol = computelight(ii, eye, maxDepth);
				RGBPixel(col, lightcol);
				pixels[offset] = col[0];
				pixels[offset + 1] = col[1];
				pixels[offset + 2] = col[2];
			}
			
		}
	}
}


int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Ray Tracing - Error: incorrect number of arguments\n";
		exit(EXIT_FAILURE);
	}
	
	readfile(argv[1]);
#ifdef TEST
	width = 160;
	height = 120;
	OutFilename = "TestRayTracing.png";
#endif
	
	BYTE* pixelsbuffer = new BYTE[width * height * 3];
	render(pixelsbuffer);


	FreeImage_Initialise();
	// TODO 
	// Save image
	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixelsbuffer, width, height, 3 * width, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	//FreeImage_Save(FIF_PNG, img, OutFilename.c_str(), 0);
	FreeImage_DeInitialise();
	
	delete[] pixelsbuffer;

	return 0;
}