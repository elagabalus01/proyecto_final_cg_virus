#version 330 core

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform float opacity;

void main()
{
	// Properties
	vec3 lightDir = normalize(-dirLight.direction);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

	// Specular shadding
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0f);
    
	vec3 LightPower = vec3(0.5f,0.5f,0.5f);

	// Ambient
    vec3 ambient = dirLight.ambient * vec3(1.0,1.0,1.0);
	
	//Diffuse light
    vec3 diffuse = dirLight.diffuse * diff * vec3(1.0,1.0,1.0);
	
	//Specular light
	vec3 specular = dirLight.specular * pow(spec,5) * vec3(1.0,1.0,1.0);

	vec3 result = ambient + diffuse + specular;

    vec4 texColor = texture(texture_diffuse1, TexCoords);

    color = vec4(texColor.xyz*result,opacity);
    //color = vec4(result,opacity);
}