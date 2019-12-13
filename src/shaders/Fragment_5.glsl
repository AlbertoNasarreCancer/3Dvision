varying vec2 out_textCoord;
varying vec3 out_normal;
varying vec4 out_position;

uniform sampler2D texture1;
uniform mat4 inv_model_matrix;
uniform vec3 V;

struct LightParameters {
	vec3 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	};

struct MaterialParameters {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform LightParameters Light;
uniform MaterialParameters Material;


void main()
{
	vec4 textureAlgo = texture(texture1, out_textCoord);
	vec3 N = normalize( mat3( inv_model_matrix ) * normalize( out_normal ) );
	vec3 L = normalize(Light.Position-out_position.xyz);	
	vec3 R = normalize(reflect(-L,N));

	//Ambient
	vec4 Ca = vec4(Light.La, 1.0) * vec4(Material.Ka, 1.0);

	//Difuse
	vec4 Cd = max( dot(L, N), 0.0 ) * vec4(Light.Ld, 1.0) * vec4(Material.Kd, 1.0);

	//Specular
	float Cs_float = pow( max ( dot(R, normalize(V)), 0.0 ), Material.Shininess);
	vec4 Cs = Cs_float * vec4(Light.Ls, 1.0) * vec4(Material.Ks,1.0);


	gl_FragColor = (Ca + Cd) * vec4( textureAlgo.xyz, 1.0 ) + Cs;

}
