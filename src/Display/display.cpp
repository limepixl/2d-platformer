#include "display.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../Sprite/sprite.h"

Display CreateDisplay(int width, int height, const char* title)
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "2D Platformer", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD!\n");
		glfwTerminate();
	}
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Display result { "", window, width, height, 0.0f, 0.0f, 0, 0.0f };
	strcpy(result.title, title);
	return result;
}

void DeltaTimeCalc(Display& display)
{
	float currentTime = (float)glfwGetTime();
	display.deltaTime = currentTime - display.lastTime;
	display.lastTime = currentTime;
	display.numFrames++;

	// One second has passed
	if(currentTime - display.lastFPSTime >= 1.0f)
	{
		double frameTime = 1000.0 / (double)display.numFrames;
		double fps = (double)display.numFrames / (double)(currentTime - display.lastFPSTime);

		char buffer[1024];
		sprintf(buffer,"%s | FPS: %.2lf | Frame Time: %.3lf", display.title, fps, frameTime);
		glfwSetWindowTitle(display.window, buffer);

		display.numFrames = 0;
		display.lastFPSTime = currentTime;
	}
}

void ProcessInput(const Display& display, Player& player)
{
	player.sprite.position += player.velocity;
	player.velocity = glm::vec2(0.0, 0.0);

	float gravity = 3.0f * display.deltaTime;
	if(!player.onGround)
		player.acceleration -= gravity * glm::vec2(0.0, 1.0);

	float jumpHeight = 4.0f * display.deltaTime;
	float playerSpeed = 6.0f * display.deltaTime;
	if(glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS)
		player.velocity -= playerSpeed * glm::vec2(1.0, 0.0);
	if(glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS)
		player.velocity += playerSpeed * glm::vec2(1.0, 0.0);
	if(glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS && player.jumpTime < player.allowedJumpTime)
	{
		player.onGround = false;
		player.jumpTime++;
		player.acceleration += jumpHeight * glm::vec2(0.0, 1.0);
	}
	if(glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		player.jumpTime = 1000;
	
	player.velocity += player.acceleration;

	// Clamp velocity
	if(player.velocity.x > playerSpeed)
		player.velocity.x = playerSpeed;
	else if(player.velocity.x < -playerSpeed)
		player.velocity.x = -playerSpeed;

	if(player.velocity.y < -4.0f * gravity)
		player.velocity.y = -4.0f * gravity;
	else if(player.velocity.y > 4.0f * jumpHeight)
		player.velocity.y = 4.0f * jumpHeight;
}
