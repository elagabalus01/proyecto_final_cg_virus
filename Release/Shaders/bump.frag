#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in mat3 TBN;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;
uniform sampler2D texture_specular1;
uniform sampler2D texture_opacity1;
uniform sampler2D texture_normal1;
uniform int opacityFlag = 0;
uniform int normalFlag = 0;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );

void main( )
{
    // Properties
    vec3 viewDir = normalize( viewPos - FragPos );
    vec3 norm;
    if( normalFlag == 0 ) {
        norm = normalize( Normal );
    }
    else {
        norm = texture(texture_normal1, TexCoords).rgb;
        // transform normal vector to range [-1,1]
        norm = normalize((norm * 2.0) - 1.0);  // this normal is in tangent space
        viewDir = normalize( TangentViewPos - TangentFragPos );
    }
    
    // Directional lighting
    vec3 result = CalcDirLight( dirLight, norm, viewDir );
    
    float alpha = 1.0;
    
	if( opacityFlag == 1 ) { //if we have valid mask texture coordinates, we are masking
          vec4 opacityMap = texture( texture_opacity1, TexCoords ); //get the rgba values of the mask texture
          alpha = opacityMap.r;
    }
    
    color = vec4( result, texture( texture_specular1, TexCoords ).a * alpha );
	  //if(color.a < 0.05)
        //discard;
}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -TBN*light.direction);
    if( normalFlag == 0 )
    {
        lightDir = normalize( -light.direction );
    }
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Combine results
    /*
    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ).rgb );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ).rgb );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ).rgb );
    */
    //COREGIDO
    vec3 ambient = light.ambient * vec3(0.5,0.5,0.5);
    vec3 diffuse = light.diffuse * diff * vec3(1.0,1.0,1.0);
    vec3 specular = light.specular * pow(spec,5) * vec3(1.0,1.0,1.0);

	/*vec4 result= vec4(ambient + diffuse + specular,texture( material.diffuse, TexCoords).a) ;
	  if(result.a < 0.1)
        discard;
    */
    vec4 texColor = texture(texture_specular1, TexCoords);    
    vec3 result=ambient+diffuse+specular;
    result = vec3(texColor.xyz*result);
    //result = vec3(result); //Probando sin color
    return (result);
}
