#version 330

in vec3 normalOut;
out vec4 outputColor;

void main()
{
	float ambient = 0.2f;
	float light = ambient + max(dot(normalOut, vec3(0.5f, 0.5f, 0.5f)), 0.0f);
	outputColor = light*vec4(0, 0.3, 0, 1.0f);
}
