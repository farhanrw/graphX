#version 330 core
  
layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 Model, View, Projection;
uniform bool phong;

flat out vec3 gouraud_color;
out vec3 world_position;
out vec3 trans_normal;

vec3 light_position = vec3(0.0, 20.0, 5.0);
vec3 viewer_position = vec3(0.0f, 0.0f, 15.0f);
vec3 light_color = vec3(0.8f, 0.8f, 0.8f);

void main()
{
    gl_Position = Projection * View * Model * vec4(position.x , position.y, position.z, 1.0);
	world_position = vec3(Model * vec4(position.x, position.y, position.z, 1.0));
	trans_normal = mat3(transpose(inverse(Model))) * normal;

	if(!phong)
	{
	//light direction

	vec3 light_direction = normalize(light_position - world_position);

	//viewer direction

	vec3 viewer_direction = normalize(viewer_position - world_position);

	//ambient

	float ambient_coefficient = 0.25f;
	vec3 ambient = ambient_coefficient * light_color;

	//diffuse

	float diffuse_coefficient = 0.75f;
	vec3 diffuse = max(dot(normalize(trans_normal), light_direction) , 0.0f) * diffuse_coefficient * light_color;

	//specular

	vec3 reflect_light_direction = reflect(-light_direction, normalize(trans_normal));
	float specular_coefficient = pow(max(dot(viewer_direction, reflect_light_direction), 0.0f), 32);
	vec3 specular = specular_coefficient * light_color;

	//result

	gouraud_color = ambient + diffuse + specular;
	}
}