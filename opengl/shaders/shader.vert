#version 330
in vec3 position;
in vec3 normal;
out vec3 normalOut;
out vec3 positionOut;

uniform mat4 mat_mvp;
uniform mat4 mat_proj;

void main()
{
	gl_Position = mat_proj * mat_mvp * vec4(position, 1.0f);
	positionOut = position;
	normalOut = mat3(mat_mvp) * normal;
}
