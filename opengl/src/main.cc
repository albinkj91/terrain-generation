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
#define perlin_image_width 512

GLuint shader_program;
GLuint vbo_pos;
GLuint vbo_norm;
GLuint vao;

vector<float> vertices{};
vector<float> normals{};

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

vector<float> generate_terrain()
{
	vector<vector<float>> height_map{read_ppm("../perlin/perlin.ppm")};

	vector<float> vert{};
	float scale{200.0};
	for(int i{}; i < perlin_image_width-1; ++i)
	{
		for(int j{}; j < perlin_image_width-1; ++j)
		{
			// Offset values so that center of the terrain is at the origin (0, 0, 0) in model space
			float x_offset{j - (perlin_image_width)/2.0f};
			float z_offset{i - (perlin_image_width)/2.0f};

			float x1{-0.5f + x_offset};
			float y1{height_map.at(i).at(j) * scale};
			float z1{-0.5f + z_offset};
			vert.push_back(x1);
			vert.push_back(y1);
			vert.push_back(z1);

			float x2{x1};
			float y2{height_map.at(i+1).at(j) * scale};
			float z2{0.5f + z_offset};
			vert.push_back(x2);
			vert.push_back(y2);
			vert.push_back(z2);

			float x3{0.5f + x_offset};
			float y3{height_map.at(i+1).at(j+1) * scale};
			float z3{z2};
			vert.push_back(x3);
			vert.push_back(y3);
			vert.push_back(z3);

			vert.push_back(x1);
			vert.push_back(y1);
			vert.push_back(z1);

			vert.push_back(x3);
			vert.push_back(y3);
			vert.push_back(z3);

			float x4{x3};
			float y4{height_map.at(i).at(j+1) * scale};
			float z4{z1};
			vert.push_back(x4);
			vert.push_back(y4);
			vert.push_back(z4);

			glm::vec3 v1{x1, y1, z1};
			glm::vec3 v2{x2, y2, z2};
			glm::vec3 v3{x3, y3, z3};
			glm::vec3 v4{x4, y4, z4};

			glm::vec3 v3_1{v3 - v1};
			glm::vec3 v2_1{v2 - v1};

			glm::vec3 cross1{glm::cross(v3_1, v2_1)};
			cross1 = normalize(cross1);

			normals.push_back(cross1.x);
			normals.push_back(cross1.y);
			normals.push_back(cross1.z);

			normals.push_back(cross1.x);
			normals.push_back(cross1.y);
			normals.push_back(cross1.z);

			normals.push_back(cross1.x);
			normals.push_back(cross1.y);
			normals.push_back(cross1.z);

			glm::vec3 v4_1{v4 - v1};
			glm::vec3 cross2{glm::cross(v4_1, v3_1)};
			cross2 = normalize(cross2);

			normals.push_back(cross2.x);
			normals.push_back(cross2.y);
			normals.push_back(cross2.z);

			normals.push_back(cross2.x);
			normals.push_back(cross2.y);
			normals.push_back(cross2.z);

			normals.push_back(cross2.x);
			normals.push_back(cross2.y);
			normals.push_back(cross2.z);
		}
	}
	return vert;
}

void InitializeVertexBuffer(vector<float> const& vertices)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_pos);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "position"));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InitializeNormalBuffer(vector<float> const& normals)
{
	glGenBuffers(1, &vbo_norm);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);

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

	vertices = generate_terrain();
	InitializeVertexBuffer(vertices);
	InitializeNormalBuffer(normals);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);
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

	glm::mat4 mvp_matrix{proj * translate * rot_x};
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat_mvp"),
			1, GL_FALSE, glm::value_ptr(mvp_matrix));

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
