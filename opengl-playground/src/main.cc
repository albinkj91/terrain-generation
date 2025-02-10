#define MAIN
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "stb_image.h"
#include "LittleOBJLoader.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define window_width 800
#define window_height 800
#define triangle_count 18

GLuint shader_program;
GLuint vbo;
Model* m;

//glm::vec3 camera{0.0f, 1.0f, 1.0f};

string load_shader(string const& filename)
{
	string shader{};
	string tmp{};
	ifstream ifs{filename};

	while(getline(ifs, tmp, '\n'))
	{
		shader.append(tmp)
			.append("\n");
	}
	return shader;
}

const string strVertexShader{load_shader("shaders/shader.vert")};
const string strFragmentShader{load_shader("shaders/shader.frag")};

GLuint CreateShader(GLenum eShaderType, const string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t i = 0; i < shaderList.size(); i++)
		glAttachShader(program, shaderList[i]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t i = 0; i < shaderList.size(); i++)
		glDetachShader(program, shaderList[i]);

	return program;
}

void InitializeProgram()
{
	vector<GLuint> shaderList{};

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	shader_program = CreateProgram(shaderList);
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void InitializeVertexBuffer()
{
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*m), m, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	InitializeProgram();
	glUseProgram(shader_program);
	InitializeVertexBuffer();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Create texture reference
	unsigned int texture{};
	glGenTextures(1, &texture);

	// Load image
	int width{}, height{}, nrChannels{};
	unsigned char *data = stbi_load("assets/gripen.jpg", &width, &height, &nrChannels, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program);

	DrawModel(m, shader_program, "position", "normal", NULL);
}

int main()
{
	if (!glfwInit())
	{
		cerr << "GLFW init failed" << endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(window_width,
			window_height, "Playground", NULL, NULL);

	glfwSetWindowAspectRatio(window, window_width, window_height);

	if(!window)
	{
		glfwTerminate();
		cerr << "GLFW window creation failed" << endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	init();
	m = LoadModel("models/sportsCar.obj");

	float rot_angle{0.01f};
	float aspect_ratio{static_cast<float>(window_width) / static_cast<float>(window_height)};
	glm::mat4 proj{glm::perspective(glm::radians(45.0f),
			aspect_ratio,
			0.1f,
			100.0f
	)};

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 rot_y{glm::rotate(glm::mat4{1.0f}, rot_angle, glm::vec3{0.0, 1.0, 0.0})};
		glm::mat4 translate{glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -1.0f, -6.5f))};

		glm::mat4 mvp_matrix{proj * translate * rot_y};
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_mvp"),
				1, GL_FALSE, glm::value_ptr(mvp_matrix));

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
		rot_angle += 0.01f;
	}
}
