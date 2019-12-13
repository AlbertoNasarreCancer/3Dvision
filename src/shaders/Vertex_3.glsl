attribute vec3 Position;
attribute vec2 Texture;

uniform mat4 mvp;
uniform mat4 transform;

varying vec2 textCoord;

void main()

{
	
            textCoord = Texture;

            vec4 v = vec4( Position, 1.0 );
	
            gl_Position = mvp * transform * v;



}
