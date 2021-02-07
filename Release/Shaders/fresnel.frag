#version 400 core

//#extension GL_NV_shadow_samplers_cube : enable

//in vec4 ex_Color;
in vec2 ex_Texcoord;
in vec3 ex_N;

in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;

out vec4 out_Color;

uniform sampler2D tex;
uniform vec3 lightDir;

uniform float opacity;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// incoming surface normal and view-space position
in vec3 vs_fs_normal;
in vec3 vs_fs_position;

// incoming Fresnel reflection and refraction parameters
in vec3  vReflect;
in vec3  vRefract[3];
in float vReflectionFactor;

// the cubemap texture
uniform sampler2D texture_diffuse1;

void main(void)
{

	// Fresnel
	vec4 reflectedColor = texture( texture_diffuse1, vec2( -vReflect.x, vReflect.yz ) );
	//vec4 reflectedColor = texture( texture_diffuse1, ex_Texcoord );
	vec4 refractedColor = vec4( 1.0f );

	refractedColor.r = texture( texture_diffuse1, vec2( -vRefract[0].x, vRefract[0].yz ) ).r;
	refractedColor.g = texture( texture_diffuse1, vec2( -vRefract[1].x, vRefract[1].yz ) ).g;
	refractedColor.b = texture( texture_diffuse1, vec2( -vRefract[2].x, vRefract[2].yz ) ).b;

	// sample just fresnel
	vec4 aux = texture(tex, ex_Texcoord) * mix( refractedColor, reflectedColor, clamp( vReflectionFactor, 0.0, 1.0 ) );
	out_Color = vec4(aux.r,aux.g,aux.b,opacity);
}
