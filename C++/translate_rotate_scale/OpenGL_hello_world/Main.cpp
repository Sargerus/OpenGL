#include "Headers_h.h"

int main() {

	App& app = App::getInstance();

	app.initAndPrepareContext(1600, 800, "The window");

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	app.load2DTexture("container.jpg", 0);
	app.load2DTexture("awesomeface.png", 1);
	app.bindVAO();
	
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	app.startRenderingCycle(ourShader);

	return 0;
}