#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Display/display.hpp"
#include "AssetManagement/assetmanagement.hpp"
#include "Sprite/sprite.hpp"
#include <vector>

unsigned int Texture::GlobalTextureIndex = 0;

int main()
{
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	Display display = CreateDisplay(WIDTH, HEIGHT, "2D Platformer - limepixl");
	Shader shader = LoadShadersFromFiles("res/shaders/normal/normal.vs", "res/shaders/normal/normal.fs");
	glUseProgram(shader.ID);

	Texture textureAtlas = LoadTexture2DFromFile("res/images/atlas.png");
	glUniform1i(shader.uniforms["tex"], textureAtlas.index);

	std::vector<float> batchedVertices;
	std::vector<float> batchedUVs;
	BatchSpriteData(1, 0, { 0.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 1.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 2.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 3.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 4.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 5.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 6.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(0, 0, { 6.0f, 3.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(0, 0, { 6.0f, 3.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 7.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 8.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 9.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 10.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 11.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 12.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 13.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 14.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 15.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 16.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(1, 0, { 17.0f, 0.0f }, batchedVertices, batchedUVs);
	BatchSpriteData(0, 0, { 7.0f, 3.0f }, batchedVertices, batchedUVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBOs[2];
	glGenBuffers(2, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glm::mat4 projection = glm::ortho(0.0f, (float)display.width, 0.0f, (float)display.height, -1.0f, 1.0f);
	glUniformMatrix4fv(shader.uniforms["projection"], 1, GL_FALSE, &projection[0][0]);

	glm::vec2 cameraPos(0.0f, 0.0f);

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);
		ProcessInput(display, cameraPos);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batchedVertices.size() * sizeof(float), batchedVertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batchedUVs.size() * sizeof(float), batchedUVs.data());

		glm::mat4 view(1.0);
		view = glm::translate(view, glm::vec3(cameraPos, 0.0f));
		glUniformMatrix4fv(shader.uniforms["view"], 1, GL_FALSE, &view[0][0]);

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, (int)batchedVertices.size());

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}