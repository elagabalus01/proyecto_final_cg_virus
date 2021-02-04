#version 400 core

layout(location=0) in vec4 aPos;
//layout(location=1) in vec4 in_Color;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

//out vec4 ex_Color;
out vec2 ex_Texcoord;
out vec3 ex_N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;

// Outgoing surface normal and view-space position
out vec3 vs_fs_normal;
out vec3 vs_fs_position;

// Outgoing Fresnel reflection and refraction parameters
out vec3  vReflect;
out vec3  vRefract[3];
out float vReflectionFactor;


void main(void)
{
    // Clip-space position

    vec4 position = aPos + 0.005*sin(0.02*time) * vec4(aNormal, 1.0f) ;

    gl_Position = projection * view * model * position;
    gl_PointSize = 1.0f;

    // Position of the vertex, in worldspace : model * position
    Position_worldspace = (model * aPos).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( view * model * aPos).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // View-space normal and position
    vs_fs_normal = mat3(view*model) * aNormal;
    vs_fs_position = (view*model*aPos).xyz;

    // Fresnel parameters
    float mRefractionRatio = 2.65f;
    //float mFresnelBias = 0.1f;
    float mFresnelBias = 0.1f;
    float mFresnelScale = 5.0f;
    float mFresnelPower = 0.1f;

    // Fresnel calculations
    vec3 cameraPosition = -view[3].xyz * mat3(view);
    vec3 I = Position_worldspace - cameraPosition;
    vec3 worldNormal = normalize( mat3( model[0].xyz, model[1].xyz, model[2].xyz ) * -aNormal );

    vReflect = reflect( I, worldNormal );
    vRefract[0] = refract( normalize( I ), worldNormal, mRefractionRatio );
    vRefract[1] = refract( normalize( I ), worldNormal, mRefractionRatio * 0.99 );
    vRefract[2] = refract( normalize( I ), worldNormal, mRefractionRatio * 0.98 );
    vReflectionFactor = mFresnelBias + mFresnelScale * pow( 1.0 + dot( normalize( I ), worldNormal ), mFresnelPower );
    
    //ex_Color = in_Color;
    ex_Texcoord = aTexCoords;
    ex_N = aNormal;
}
