#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Display/display.h"
#include "AssetManagement/assetmanagement.h"
#include "Sprite/sprite.h"

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

	std::vector<Enemy> enemies;

	int playerIndex;
	std::vector<Sprite> level = LoadLevelFromFile("res/levels/level1.txt", playerIndex, enemies);
	Player player{ level[playerIndex], false, 0, 20, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(playerIndex % 50, playerIndex / 50), 0 };
	
	Batch batch = PreallocateBatch();

	glm::mat4 projection = glm::ortho(0.0f, (float)display.width, 0.0f, (float)display.height, -1.0f, 1.0f);
	glUniformMatrix4fv(shader.uniforms["projection"], 1, GL_FALSE, &projection[0][0]);

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);
		ProcessInput(display, player);
		ProcessCollisionsPlayer(player, level);
		ProcessCollisionsEnemies(enemies, level, player, display.deltaTime);

		glm::mat4 view(1.0);
		float cameraX = (player.sprite.position.x > 10.0f) ? (player.sprite.position.x < 40.0f ? -player.sprite.position.x : -40.0f) : -10.0f;
		view = glm::translate(view, glm::vec3(WIDTH * 0.5f + cameraX * 64.0f, 0.0f, 0.0f));

		glm::mat4 PV = projection * view;
		glUniformMatrix4fv(shader.uniforms["PV"], 1, GL_FALSE, &PV[0][0]);

		BatchSpriteData(level, batch, PV);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, (int)batch.indices.size(), GL_UNSIGNED_INT, nullptr);

		batch.vertices.clear();
		batch.uvs.clear();
		batch.indices.clear();

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}