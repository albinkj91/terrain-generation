#version 330
in vec3 position;
//in vec3 normal;
//out vec3 normalOut;

uniform mat4 mat_mvp;

void main()
{
	gl_Position = mat_mvp * vec4(position, 1.0f);
	//normalOut = mat3(mat_mvp) * normal;
}
