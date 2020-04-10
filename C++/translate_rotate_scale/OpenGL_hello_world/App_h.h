#ifndef App_h
#define App_h

#include "Headers_h.h"

class App 
{
	private:
			static App* instance;
			static unsigned short int textureCount;
			unsigned int VBO, VAO, EBO;
			App();
			GLFWwindow* window;
	public:
			static App& getInstance();
			void initAndPrepareContext(int width = 800, int height = 600, const char *title = "Window");
			GLuint load2DTexture(const char* path, int rgb);
			void bindVAO();
			void startRenderingCycle(Shader shader);
			void processInput(Shader shader);
};

#endif