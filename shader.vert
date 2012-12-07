#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 color;
uniform vec3 inPosition;
uniform float pointSize;

smooth out vec4 theColor;

void main()
{
	gl_Position = projectionMatrix*viewMatrix*vec4(inPosition, 1.0);
  gl_PointSize = pointSize;
	theColor = color;
}
