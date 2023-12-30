#version 460 core
in vec3 color;
in vec3 Normal;  
in vec3 FragPos;
out vec4 out_FragColor;
void main()
{
	lightPos = vec4(1.2, 1.0, 2.0, 1.0);
	
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * color;
    out_FragColor = vec4(result, 1.0);
	
	
    //out_FragColor = vec4(color, 1.0);
}