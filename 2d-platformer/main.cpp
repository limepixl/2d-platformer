#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/Display/display.hpp"
#include "src/AssetManagement/assetmanagement.hpp"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Display display = CreateDisplay(WIDTH, HEIGHT, "2D Platformer - limepixl");
	Shader shader = LoadShadersFromFiles("res/shaders/normal.vs", "res/shaders/normal.fs");

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);

		glClear(GL_COLOR_BUFFER_BIT);

		// render

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}