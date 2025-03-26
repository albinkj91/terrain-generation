#version 330

in vec3 normalOut;
out vec4 outputColor;

void main()
{
	float ambient = 0.4f;
	float light = ambient + max(dot(vec3(0.5f, 0.5f, 0.5f), normalOut), 0.0f);
	outputColor = vec4(0, light*0.3, 0, 1.0f);// * texture(sampleTex, texOut);
	//outputColor = vec4(normalOut, 1.0f);
}
