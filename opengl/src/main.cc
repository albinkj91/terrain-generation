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

#define byte unsigned char
#define window_width 800
#define window_height 800
#define perlin_image_width 512

GLuint shader_program;
GLuint vbo_pos;
GLuint vbo_norm;
GLuint vao;

vector<glm::vec3> vertices{};
vector<glm::vec3> normals{};

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

vector<vector<float>> read_ppm(string const& file)
{
	vector<vector<float>> data{};
	fstream input{file, ios_base::in};
	string temp{};
	getline(input, temp);
	getline(input, temp);
	getline(input, temp);

	float in{};
	for(int i{}; i < perlin_image_width; ++i)
	{
		data.push_back(vector<float>{});
		for(int j{}; j < perlin_image_width; ++j)
		{
			input >> in;
			data.at(i).push_back(in);
		}
	}
	return data;
}

glm::vec3 calc_normal(int x, int y, vector<vector<glm::vec3>> const& vertices)
{
	glm::vec3 v_up{};
	glm::vec3 v_down{};
	glm::vec3 v_right{};
	glm::vec3 v_left{};

	if(y == 0)
		v_up = vertices.at(y).at(x);
	else
		v_up = vertices.at(y-1).at(x);

	if(x == 0)
		v_left = vertices.at(y).at(x);
	else
		v_left = vertices.at(y).at(x-1);

	if(y == perlin_image_width - 1)
		v_down = vertices.at(y).at(x);
	else
		v_down = vertices.at(y+1).at(x);

	if(x == perlin_image_width - 1)
		v_right = vertices.at(y).at(x);
	else
		v_right = vertices.at(y).at(x+1);

	glm::vec3 v_up_down{v_up - v_down};
	glm::vec3 v_right_left{v_right - v_left};
	glm::vec3 normal{normalize(glm::cross(v_right_left, v_up_down))};
	return normal;
}

void generate_terrain()
{
	vector<vector<float>> height_map{read_ppm("../perlin/perlin.ppm")};

	float scale{100.0};
	vector<vector<glm::vec3>> temp{};
	for(int i{}; i < perlin_image_width; ++i)
	{
		temp.push_back(vector<glm::vec3>(perlin_image_width));
	}
	
	// Generate vertices
	for(int i{}; i < perlin_image_width - 1; ++i)
	{
		float z{i - (perlin_image_width)/2.0f};
		for(int j{}; j < perlin_image_width - 1; ++j)
		{
			// Offset values so that center of the terrain is at the origin (0, 0, 0) in model space
			float x{j - (perlin_image_width)/2.0f};

			glm::vec3 vertex1{x, height_map.at(i).at(j) * scale, z};
			glm::vec3 vertex2{x, height_map.at(i+1).at(j) * scale, z+1};
			glm::vec3 vertex3{x+1, height_map.at(i+1).at(j+1) * scale, z+1};

			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);

			temp.at(i).at(j) = vertex1;
			temp.at(i).at(j+1) = vertex2;
			temp.at(i+1).at(j+1) = vertex3;

			glm::vec3 vertex4{x+1, height_map.at(i).at(j+1) * scale, z};

			vertices.push_back(vertex1);
			vertices.push_back(vertex3);
			vertices.push_back(vertex4);
			temp.at(i+1).at(j) = vertex4;
		}
	}


	// Generate normals
	for(int i{}; i < perlin_image_width - 1; ++i)
	{
		for(int j{}; j < perlin_image_width - 1; ++j)
		{
			glm::vec3 normal1{calc_normal(j, i, temp)};
			normals.push_back(normal1);
			glm::vec3 normal2{calc_normal(j, i+1, temp)};
			normals.push_back(normal2);
			glm::vec3 normal3{calc_normal(j+1, i+1, temp)};
			normals.push_back(normal3);

			glm::vec3 normal4{calc_normal(j+1, i, temp)};
			normals.push_back(normal1);
			normals.push_back(normal3);
			normals.push_back(normal4);
		}
	}
}

void InitializeVertexBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_pos);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "position"));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InitializeNormalBuffer()
{
	glGenBuffers(1, &vbo_norm);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "normal"));

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

	generate_terrain();
	InitializeVertexBuffer();
	InitializeNormalBuffer();
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	glm::mat4 translate{glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -800.0f))};
	glm::mat4 rot_x{glm::rotate(glm::mat4{1.0f}, 0.8f, glm::vec3{1.0f, 0.0f, 0.0f})};

	glm::mat4 mvp_matrix{translate * rot_x};

	glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_proj"),
			1, GL_FALSE, glm::value_ptr(proj));

	glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_mvp"),
			1, GL_FALSE, glm::value_ptr(mvp_matrix));

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
