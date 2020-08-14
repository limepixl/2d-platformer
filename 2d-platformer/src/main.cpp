#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Display/display.hpp"
#include "AssetManagement/assetmanagement.hpp"
#include <glm/gtc/matrix_transform.hpp>

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Display display = CreateDisplay(WIDTH, HEIGHT, "2D Platformer - limepixl");
	Shader shader = LoadShadersFromFiles("res/shaders/normal/normal.vs", "res/shaders/normal/normal.fs");
	glUseProgram(shader.ID);

	float vertices[]
	{
		100.0f, 200.0f, 
		300.0f, 200.0f,
		300.0f, 400.0f,
		300.0f, 400.0f,
		100.0f, 400.0f,
		100.0f, 200.0f
	};

	float colors[]
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	GLuint colorVBO;
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glm::mat4 model(1.0);
	glUniformMatrix4fv(shader.uniforms["model"], 1, GL_FALSE, &model[0][0]);

	glm::mat4 projection = glm::ortho(0.0f, (float)display.width, 0.0f, (float)display.height, -1.0f, 1.0f);
	glUniformMatrix4fv(shader.uniforms["projection"], 1, GL_FALSE, &projection[0][0]);

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}