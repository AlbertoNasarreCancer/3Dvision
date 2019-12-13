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

uniform LightParameters Light1;
uniform LightParameters Light2;
uniform MaterialParameters Material;


void main()
{
	vec4 textureAlgo = texture(texture1, out_textCoord);
	vec3 N = normalize( mat3( inv_model_matrix ) * normalize( out_normal ) );
	vec3 L1 = normalize( Light1.Position-out_position.xyz) ;
	vec3 L2 = normalize( Light2.Position-out_position.xyz) ;	
	vec3 R1 = normalize(reflect(-L1,N));
	vec3 R2 = normalize(reflect(-L2,N));

	//Ambient
	vec4 Ca = vec4(Light1.La, 1.0) * vec4(Material.Ka, 1.0) + vec4(Light2.La, 1.0) * vec4(Material.Ka, 1.0);

	//Difuse
	vec4 Cd = max( dot(L1, N), 0.0 ) * vec4(Light1.Ld, 1.0) * vec4(Material.Kd, 1.0) +  max( dot(L2, N), 0.0 ) * vec4(Light2.Ld, 1.0) * vec4(Material.Kd, 1.0);

	//Specular
	float Cs_float1 = pow( max ( dot(R1, normalize(V)), 0.0 ), Material.Shininess);
	float Cs_float2 = pow( max ( dot(R2, normalize(V)), 0.0 ), Material.Shininess);
	vec4 Cs = Cs_float1 * vec4(Light1.Ls, 1.0) * vec4(Material.Ks,1.0) + Cs_float2 * vec4(Light2.Ls, 1.0) * vec4(Material.Ks,1.0);


	gl_FragColor = (Ca + Cd) * vec4( textureAlgo.xyz, 1.0 ) + Cs;

}


