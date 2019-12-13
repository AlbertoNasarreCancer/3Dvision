
attribute vec3 Position;
attribute vec2 Texture;
attribute vec3 Normal;

uniform mat4 mvp;
uniform mat4 transform;

varying vec2 out_textCoord;
varying vec3 out_normal;

void main()
{
	out_textCoord = Texture;
	out_normal = Normal;
	vec4 v = vec4( Position, 1.0 );
	gl_Position = mvp * transform * v;
}