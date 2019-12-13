varying vec4 normal_frag;

void main()
{	
	gl_FragColor = (normal_frag + 1.0)/2.0; 
}
