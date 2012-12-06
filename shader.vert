#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 color;
uniform vec3 inPosition;
uniform float pointSize;

smooth out vec3 theColor;

void main()
{
	gl_Position = projectionMatrix*viewMatrix*vec4(inPosition, 1.0);
  gl_PointSize = pointSize;
	theColor = color;
}
