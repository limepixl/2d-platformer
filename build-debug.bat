clang -g3 -Wall -Wextra .\src\main.cpp .\thirdparty\glad\src\glad.c .\src\AssetManagement\*.cpp .\src\Display\*.cpp .\src\Sprite\*.cpp -I .\thirdparty\glad\include\ -I .\thirdparty\GLFW\include\ -I .\thirdparty\glm\ -I .\thirdparty\ -D_CRT_SECURE_NO_WARNINGS -L .\thirdparty\GLFW\lib\ -lglfw3-clang -lgdi32 -luser32 -lshell32 -lmsvcrt