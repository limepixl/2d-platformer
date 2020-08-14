#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Display/display.hpp"
#include "AssetManagement/assetmanagement.hpp"

unsigned int Texture::GlobalTextureIndex = 0;

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Display display = CreateDisplay(WIDTH, HEIGHT, "2D Platformer - limepixl");
	Shader shader = LoadShadersFromFiles("res/shaders/normal/normal.vs", "res/shaders/normal/normal.fs");
	glUseProgram(shader.ID);

	Texture testTexture = LoadTexture2DFromFile("res/images/test.png");
	glUniform1i(shader.uniforms["tex"], testTexture.index);

	float vertices[]
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		(float)testTexture.width, 0.0f, 1.0f, 0.0f,
		(float)testTexture.width, (float)testTexture.height, 1.0f, 1.0f,
		(float)testTexture.width, (float)testTexture.height, 1.0f, 1.0f,
		0.0f, (float)testTexture.height, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glm::mat4 model(1.0);
	glUniformMatrix4fv(shader.uniforms["model"], 1, GL_FALSE, &model[0][0]);

	glm::mat4 projection = glm::ortho(0.0f, (float)display.width, 0.0f, (float)display.height, -1.0f, 1.0f);
	glUniformMatrix4fv(shader.uniforms["projection"], 1, GL_FALSE, &projection[0][0]);

	glm::vec2 cameraPos(0.0f, 0.0f);

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);
		ProcessInput(display, cameraPos);

		glm::mat4 view(1.0);
		view = glm::translate(view, glm::vec3(cameraPos, 0.0f));
		glUniformMatrix4fv(shader.uniforms["view"], 1, GL_FALSE, &view[0][0]);

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}