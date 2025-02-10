#version 330
in vec3 position;
in vec3 normal;
in vec2 textureCoord;

out vec4 colorOut;
out vec2 textOut;
out vec3 normalOut;

uniform mat4 mat_mvp;

void main()
{
	gl_Position = mat_mvp * vec4(position, 1.0f);
	textOut = textureCoord;
	normalOut = mat3(mat_mvp) * normal;
}
