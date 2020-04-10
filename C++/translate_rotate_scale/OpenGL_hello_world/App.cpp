#include "App_h.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description)
{
	std::cout << "Error occured number: " << error << " - " << description << std::endl;
}

App* App::instance = NULL;
unsigned short int App::textureCount = 0;

void App::initAndPrepareContext(int width, int height, const char* title)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) 
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!this->window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(this->window);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, width, height);

}

App& App::getInstance()
{
	if (!instance) {
		instance = new App();
	}
	return *instance;
}

GLuint App::load2DTexture(const char* path, int rgb)
{
    unsigned int texture = 0;
	int width = 0, height = 0, nrChanells = 0;
	unsigned char* data;

	glActiveTexture(GL_TEXTURE0 + this->textureCount);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load(path, &width, &height, &nrChanells, 0);
	if (rgb == 0) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture);

	this->textureCount++;
	return texture;
}

void App::bindVAO() 
{
	float vertices[] = {
		// positions          // colors          // texture coords                  //// texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,                 //2.0f, 2.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,                 //2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,                 //0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f                 //0.0f, 2.0f    // top left 
	};

	unsigned int indices[] = {
		2,3,0,
		0,1,2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void App::startRenderingCycle(Shader shader)
{
	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");

	while (!glfwWindowShouldClose(this->window))
	{
		processInput(shader);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//glUseProgram(shaderProgram);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glm::mat4 trans2 = glm::mat4(1.0f);
		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans2 = glm::scale(trans2, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime())));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		////glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

void App::processInput(Shader shader)
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		GLfloat* params = new float[1];
		glGetUniformfv(shader.ID, glGetUniformLocation(shader.ID, "grade"), params);
		glUniform1f(glGetUniformLocation(shader.ID, "grade"), *(params)-0.001f);
		if (*(params) < 0.0f)
		{
			glUniform1f(glGetUniformLocation(shader.ID, "grade"), 0.0f);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		GLfloat* params = new float[1];
		glGetUniformfv(shader.ID, glGetUniformLocation(shader.ID, "grade"), params);
		glUniform1f(glGetUniformLocation(shader.ID, "grade"), *(params)+0.001f);
		if (*(params) > 1.0f)
		{
			glUniform1f(glGetUniformLocation(shader.ID, "grade"), 1.0f);
		}
	}
}

App::App()
{
}