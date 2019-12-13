varying vec2 out_textCoord;
varying vec3 out_normal;
varying vec4 out_position;

uniform sampler2D texture1;
uniform mat4 inv_model_matrix;
uniform vec3 V;
uniform float in_ang;
uniform float out_ang;

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
uniform LightParameters Light2;
uniform LightParameters Light3;
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


	vec4 luz1 = (Ca + Cd) * vec4( textureAlgo.xyz, 1.0 ) + Cs;





	
	
	vec3 L2 = normalize(Light2.Position-out_position.xyz);	
	vec3 R2 = normalize(reflect(-L2,N));

	//Ambient2
	vec4 Ca2 = vec4(Light2.La, 1.0) * vec4(Material.Ka, 1.0);

	//Difuse
	vec4 Cd2 = max( dot(L2, N), 0.0 ) * vec4(Light2.Ld, 1.0) * vec4(Material.Kd, 1.0);

	//Specular
	float Cs_float2 = pow( max ( dot(R2, normalize(V)), 0.0 ), Material.Shininess);
	vec4 Cs2 = Cs_float2 * vec4(Light2.Ls, 1.0) * vec4(Material.Ks,1.0);


	vec4 luz2 = (Ca2 + Cd2) * vec4( textureAlgo.xyz, 1.0 ) + Cs2;



	
	
	
	vec3 L3 = normalize(-Light3.Position+out_position.xyz);	
	vec3 R3 = normalize(reflect(-L,N));
	vec3 l = normalize(Light3.Position);

	
	// Spot light 
	float ang = max ( acos ( dot (L3, -l) / (length(l) *  length(L3) ) ), 0.0);
	
	if (ang < in_ang){
		L3 = L3 * (1 - (ang - in_ang) / (out_ang - in_ang) );	
	}

	if (ang > out_ang){
		L3 = vec3(0, 0, 0);
	}

	//Ambient
	vec4 Ca3 = vec4(Light3.La, 1.0) * vec4(Material.Ka, 1.0);

	//Difuse
	vec4 Cd3 = max( dot(L3, N), 0.0 ) * vec4(Light3.Ld, 1.0) * vec4(Material.Kd, 1.0);

	//Specular
	float Cs_float3 = pow( max ( dot(R3, normalize(V)), 0.0 ), Material.Shininess);
	vec4 Cs3 = Cs_float3 * vec4(Light3.Ls, 1.0) * vec4(Material.Ks,1.0);

	gl_FragColor = (Ca3 + Cd3) * vec4( textureAlgo.xyz, 1.0 ) + Cs3+luz1+luz2;

}


