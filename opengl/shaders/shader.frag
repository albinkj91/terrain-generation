#version 330

//in vec3 normalOut;
out vec4 outputColor;

void main()
{
	//float ambient = 0.1f;
	//float light = ambient + max(dot(vec3(0.58f, 0.58f, 0.58f), normalize(normalOut)), 0);
	//outputColor = vec4(light, light, light, 1.0f);// * texture(sampleTex, texOut);
	outputColor = vec4(1.0, 0.5, 0.2, 1.0);
}
