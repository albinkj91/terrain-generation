#version 330

in vec3 normalOut;
in vec3 positionOut;
in vec4 colorOut;
out vec4 outputColor;

void main()
{
	float ambient = 0.4f;
	float light = ambient + max(dot(vec3(0.58f, 0.58f, 0.58f), normalOut), 0.0f);
	if(positionOut.y < 90.5f)
		outputColor = light * vec4(0.0f, 0.5f, 0.7f, 1.0f);
	else if(positionOut.y < 93)
		outputColor = light * vec4(0.7f, 0.6f, 0.5f, 1.0f);
	else
		outputColor = light * colorOut;
}
