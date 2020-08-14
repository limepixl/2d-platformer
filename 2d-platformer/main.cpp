#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Display/display.h"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Display display = CreateDisplay(WIDTH, HEIGHT, "2D Platformer - limepixl");

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