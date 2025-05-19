#version 330

in vec3 normalOut;
in vec3 positionOut;
out vec4 outputColor;

void main()
{
	float ambient = 0.4f;
	float light = ambient + max(dot(vec3(0.58f, 0.58f, 0.58f), normalOut), 0.0f);
	if(positionOut.y < 136.0f)
		outputColor = light * vec4(0.0f, 0.3f, 0.5f, 1.0f);
	else
		outputColor = light*vec4(0.0f, 0.2f, 0.1f, 1.0f);
}
