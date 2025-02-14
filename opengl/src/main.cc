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
#include <sstream>
#include <iterator>
#include "stb_image.h"
using namespace std;

#define byte unsigned char
#define window_width 800
#define window_height 800

GLuint shader_program;
GLuint vbo;
GLuint vao;

vector<float> vertices{};

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

vector<float> read_ppm(string const& file)
{
	vector<float> data{};
	fstream input{file, ios_base::in};
	string temp{};
	getline(input, temp);
	getline(input, temp);
	getline(input, temp);

	while(input >> temp)
	{
		byte in{};
		istringstream iss{temp};
		while(iss >> in)
			data.push_back(static_cast<float>(in));
	}
	return data;
}

vector<float> generate_terrain()
{
	vector<float> height_map{read_ppm("../perlin/perlin.ppm")};
	vector<float> vert{};
	int width{512};
	for(int i{}; i < width - 1; ++i)
	{
		for(int j{}; j < width - 1; ++j)
		{
			float x_offset{j - (width-1)/2.0f};
			float z_offset{i - (width-1)/2.0f};

			float y1{height_map.at((i+1) * width + j+1)};
			float x1{0.5f + x_offset};
			float z1{0.5f + z_offset};
			vert.push_back(x1);
			vert.push_back(y1);
			vert.push_back(z1);

			float y2{height_map.at(i * width + j+1)};
			float x2{0.5f + x_offset};
			float z2{-0.5f + z_offset};
			vert.push_back(x2);
			vert.push_back(y2);
			vert.push_back(z2);

			float y3{height_map.at(i * width + j)};
			float x3{-0.5f + x_offset};
			float z3{-0.5f + z_offset};
			vert.push_back(x3);
			vert.push_back(y3);
			vert.push_back(z3);

			vert.push_back(x1);
			vert.push_back(y1);
			vert.push_back(z1);

			vert.push_back(-0.5f + x_offset);
			vert.push_back(y3);
			vert.push_back(-0.5f + z_offset);

			float y4{height_map.at((i+1) * width + j)};
			vert.push_back(-0.5f + x_offset);
			vert.push_back(y4);
			vert.push_back(0.5f + z_offset);

			//float cross1{glm::vec3{x1, y, z1
		}
	}
	return vert;
}

void InitializeVertexBuffer(vector<float> const& vertices)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "position"));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void load_image(string const& file)
{
	// Create texture reference
	unsigned int texture{};
	glGenTextures(1, &texture);

	//// Load image
	int width{}, height{}, nrChannels{};
	byte *data = stbi_load(file.data(), &width, &height, &nrChannels, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

void init()
{
	InitializeProgram();
	glUseProgram(shader_program);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	vertices = generate_terrain();
	InitializeVertexBuffer(vertices);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
	glm::mat4 proj{glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 1600.0f)};
	glm::mat4 translate{glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -300.0f, -1000.0f))};

	glm::mat4 mvp_matrix{proj * translate};
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_mvp"),
			1, GL_FALSE, glm::value_ptr(mvp_matrix));

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
