#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight light;

void main()
{
	// Properties
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

	// Specular shadding
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0f);
    
	// Ambient
    vec3 ambient = light.ambient * material.ambient;
	
	//Diffuse light
    vec3 diffuse = light.diffuse * diff * material.diffuse;
	
	//Specular light
	vec3 specular = light.specular * spec * material.specular;
	
    vec3 LightPower = vec3(0.5f,0.5f,0.5f);

    //vec3 result = ambient + diffuse + specular;
    vec3 result = ambient +
    diffuse * LightPower * diff + 
    specular * LightPower * pow(spec,5);

    color = vec4(result, 1.0f);
}