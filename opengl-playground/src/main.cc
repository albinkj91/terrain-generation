#define MAIN
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "stb_image.h"
using namespace std;

#define window_width 800
#define window_height 800

GLuint shader_program;
GLuint vbo;
GLuint vao;

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

GLuint create_shader(GLenum eShaderType, const string &strShaderFile)
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

GLuint create_program(const vector<GLuint> &shaderList)
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

	shaderList.push_back(create_shader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(create_shader(GL_FRAGMENT_SHADER, strFragmentShader));

	shader_program = create_program(shaderList);
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

vector<float> generate_terrain(unsigned char* data)
{
	vector<float> vertices{
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.0, 0.5, -0.5
	};
	return vertices;
}

void InitializeVertexBuffer(vector<float> const& vertices)
{
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	InitializeProgram();
	glUseProgram(shader_program);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Create texture reference
	unsigned int texture{};
	glGenTextures(1, &texture);

	// Load image
	int width{}, height{}, nrChannels{};
	unsigned char *data = stbi_load("perlin/perlin.ppm", &width, &height, &nrChannels, 0);
	vector<float> vertices{generate_terrain(data)};
	InitializeVertexBuffer(vertices);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cout << "here1" << endl;
	cout << "here2" << endl;

	glDrawArrays(GL_TRIANGLES, 0, 3);
	cout << "here3" << endl;
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

	float aspect_ratio{static_cast<float>(window_width) / static_cast<float>(window_height)};
	glm::mat4 proj{glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f)};

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 mvp_matrix{proj};
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_mvp"),
				1, GL_FALSE, glm::value_ptr(mvp_matrix));

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
