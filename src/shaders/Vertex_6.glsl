
attribute vec3 Position;
attribute vec2 Texture;
attribute vec3 Normal;
attribute vec3 eye;

uniform mat4 mvp;
uniform mat4 transform;

varying vec2 out_textCoord;
varying vec3 out_normal;
varying vec4 out_position;
varying vec3 out_eye;

void main()
{
	out_textCoord = Texture;
	out_normal = Normal;
	out_eye = eye;
	vec4 v = vec4( Position, 1.0 );
	gl_Position = mvp * transform * v;
	out_position = gl_Position; 
}
