varying vec2 textCoord;

uniform sampler2D texture1;

void main()
{	
	vec4 texture_pimpam = texture(texture1, textCoord);
	gl_FragColor = texture_pimpam;
}
