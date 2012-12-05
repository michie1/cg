 	#version 330

	smooth in vec3 theColor;
	out vec4 outputColor;
	float rand() { 
		return mod(gl_FragCoord.x, 1) / mod(gl_FragCoord.y, 1) * mod(gl_FragCoord.z, 1);
	}

	void main()
	{
		   outputColor = vec4(theColor, 1.0);
		   //outputColor = vec4(1.0, rand(), rand(), 1.0);
	}
