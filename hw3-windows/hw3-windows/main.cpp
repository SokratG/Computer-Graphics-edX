#include "FreeImage.h"
#include "variables.h"
#include "readfile.h"
#define TEST


void render(BYTE* pixels)
{
	// TODO
	// find intersections and set pixels values
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

	BYTE* pixels = new BYTE[width * height * 3];
	render(pixels);


	FreeImage_Initialise();
	// TODO 
	// Save image
	FreeImage_DeInitialise();
	
	delete[] pixels;

	return 0;
}