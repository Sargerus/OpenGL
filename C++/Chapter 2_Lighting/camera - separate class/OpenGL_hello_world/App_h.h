#ifndef App_h
#define App_h

#include "Headers_h.h"

class App 
{
	private:
			float lastFrame = 0.0f;
			float deltaTime = 0.0f;
			float currentFrame;
			glm::vec3 direction;
			static App* instance;
			static unsigned short int textureCount;
			unsigned int VBO, VAO, EBO, lightVAO;
			glm::vec3 cameraPos;
			//glm::vec3 cameraFront;
			glm::vec3 cameraUp;
			App();
			GLFWwindow* window;
			glm::vec3 lockOnXZPlane(glm::vec3);
	public:
			static App& getInstance();
			void initAndPrepareContext(int width = 800, int height = 600, const char *title = "Window");
			GLuint load2DTexture(const char* path, int rgb);
			void bindVAO();
			void startRenderingCycle(Shader shader, Shader lightShader);
			void processInput(Shader shader);
};

#endif