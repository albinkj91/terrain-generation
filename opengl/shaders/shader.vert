#version 330
in vec3 position;
in vec3 normal;
out vec3 normalOut;
out vec3 positionOut;
out vec4 colorOut;

uniform mat4 mvp;
uniform mat4 proj;
uniform mat4 cam;

void main()
{
	gl_Position = proj * cam * mvp * vec4(position, 1.0f);
	positionOut = position;
	normalOut = mat3(mvp) * normal;

	if(position.y > 110)
		colorOut = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	else
		colorOut = vec4(0.0f, 0.4f, 0.3f, 1.0f);
}
