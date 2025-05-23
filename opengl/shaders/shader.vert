#version 330
in vec3 position;
in vec3 normal;
out vec3 normalOut;
out vec3 positionOut;
out vec4 colorOut;

uniform mat4 mat_mvp;
uniform mat4 mat_proj;

void main()
{
	gl_Position = mat_proj * mat_mvp * vec4(position, 1.0f);
	positionOut = position;
	normalOut = mat3(mat_mvp) * normal;

	if(positionOut.y > 110)
		colorOut = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	else
		colorOut = vec4(0.0f, 0.4f, 0.3f, 1.0f);
}
