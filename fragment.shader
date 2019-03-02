#version 330 core

uniform bool lightsOn;
uniform bool redOn;
uniform bool greenOn;
uniform bool blueOn;
uniform bool allOn;
uniform bool phong;

flat in vec3 gouraud_color;
in vec3 world_position;
in vec3 trans_normal;

out vec4 color;

vec3 light_position = vec3(0.0, 20.0, 5.0);
vec3 viewer_position = vec3(0.0f, 0.0f, 15.0f);
vec3 light_color = vec3(0.8f, 0.8f, 0.8f);
vec3 objColor = vec3(.2f, 0.2f, 0.2f);
	
void main()
{
	///the normal of the fragment
	if (!phong)
	{
		color = vec4(objColor*gouraud_color, 1.0f);
	}
	else
	{
		//light direction

		vec3 light_direction = normalize(light_position - world_position);

		//viewer direction

		vec3 viewer_direction = normalize(viewer_position - world_position);

		//ambient

		float ambient_coefficient = 0.25f;
		vec3 ambient = ambient_coefficient * light_color;

		//diffuse

		float diffuse_coefficient = 0.75;
		vec3 diffuse = max(dot(normalize(trans_normal), light_direction), 0.0f) * diffuse_coefficient * light_color;

		//specular

		vec3 reflect_light_direction = reflect(-light_direction, normalize(trans_normal));
		float specular_coefficient = pow(max(dot(viewer_direction, reflect_light_direction), 0.0f), 32);
		vec3 specular = specular_coefficient * light_color;

		//result

		color = vec4((ambient + diffuse + specular) * objColor, 1.0f);
		
	}

} 