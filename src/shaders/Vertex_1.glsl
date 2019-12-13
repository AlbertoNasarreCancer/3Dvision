attribute vec3 Position;
attribute vec3 Normal;

uniform mat4 mvp;
uniform mat4 transform;

varying vec4 normal_frag;

void main()
{
	vec4 v = vec4( Position, 1.0 );
	normal_frag = vec4( Normal, 1.0 );

	gl_Position = mvp * transform * v;
}
