#include "Headers_h.h"

int main() {

	App& app = App::getInstance();

	app.initAndPrepareContext(1920, 1080, "The window");

	Shader ourShader("vertexShader.vert", "fragmentShader.frag");
	Shader lightCubeShader("light_cube.vert", "light_cube.frag");

	app.load2DTexture("container.jpg", 0);
	app.load2DTexture("awesomeface.png", 1);
	app.bindVAO();

	app.startRenderingCycle(ourShader, lightCubeShader);

	return 0;
}