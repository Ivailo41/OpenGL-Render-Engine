#version 330 core
        layout (location = 0) out vec4 FragColor;

        const int MAX_POINT_LIGHTS = 4;
        const float PI = 3.14159265359;

         in VS_OUT
        {
            vec3 FragPos;
            vec2 TexCoord;
            vec3 TanViewDirection; // local to vertex position
            mat3 TanMatrix;
            
        } fs_in; 

        struct PointLight
        {
            vec3 position;

            float constant;
            float linear;
            float quadratic;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular; 
        };

        uniform sampler2D diffTexture;
        uniform sampler2D ORMTexture;
        uniform sampler2D normalTexture;
        uniform PointLight pointLights[MAX_POINT_LIGHTS];

        uniform vec3 camPos;

        vec3 CalcPointLight(PointLight light, vec3 normal)
        {
            vec3 lightDir = fs_in.TanMatrix * normalize(light.position - fs_in.FragPos);
            //vec3 lightDir = normalize(light.position - fs_in.FragPos);

            //shadows
            float diff = max(dot(normal, lightDir), 0.0); // CALCULATE IT IN TANGENT SPACE

            vec3 reflectDir = reflect(-lightDir, normal);

            float specularStrength = 0.5;
            float spec = pow(max(dot(lightDir, reflectDir), 0.0), 32);
            float specular = specularStrength * spec;

            float distance = length(light.position - fs_in.FragPos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

            vec3 ambient = vec3(0.1f);
            vec3 diffuse = vec3(1.0f);

            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;

            return (diffuse * diff + ambient); //+specular
        }

        float DistributionGGX(vec3 N, vec3 H, float roughness)
        {
            float a      = roughness*roughness;
            float a2     = a*a;
            float NdotH  = max(dot(N, H), 0.0);
            float NdotH2 = NdotH*NdotH;
	
            float num   = a2;
            float denom = (NdotH2 * (a2 - 1.0) + 1.0);
            denom = PI * denom * denom;
	
            return num / denom;
        }
        float GeometrySchlickGGX(float NdotV, float roughness)
        {
            float r = (roughness + 1.0);
            float k = (r*r) / 8.0;

            float num   = NdotV;
            float denom = NdotV * (1.0 - k) + k;

            return num / denom;
        }
        float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
        {
            float NdotV = max(dot(N, V), 0.0);
            float NdotL = max(dot(N, L), 0.0);
            float ggx2  = GeometrySchlickGGX(NdotV, roughness);
            float ggx1  = GeometrySchlickGGX(NdotL, roughness);

            return ggx1 * ggx2;
        }
        vec3 fresnelSchlick(float cosTheta, vec3 F0)
        {
            return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
        }  

        void main()
        {
            vec3 normal = texture(normalTexture, fs_in.TexCoord).rgb;

            // transform normal vector to range [-1,1]
            normal = normalize(normal * 2.0 - 1.0);

            vec3 V = fs_in.TanMatrix * normalize(camPos - fs_in.FragPos);

            vec3 ORM = texture(ORMTexture, fs_in.TexCoord).rgb; 

            vec3 diffuseTexture = vec3(texture(diffTexture, fs_in.TexCoord));
            vec3 lightResult = vec3(0.0f);

            
            vec3 F0 = vec3(0.24); 
            F0 = mix(F0, diffuseTexture, ORM.b);  

           for(int i = 0; i < MAX_POINT_LIGHTS; i++)
           {
                vec3 L = fs_in.TanMatrix * normalize(pointLights[i].position - fs_in.FragPos);
                vec3 H = normalize(V + L);

                float distance    = length(pointLights[i].position - fs_in.FragPos);
                float attenuation = 100.0 / (distance * distance);
                vec3 radiance     = pointLights[i].diffuse * attenuation; 

                // cook-torrance brdf
                float NDF = DistributionGGX(normal, H, ORM.g);        
                float G   = GeometrySmith(normal, V, L, ORM.g);      
                vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       

                vec3 kS = F;
                vec3 kD = vec3(1.0) - kS;
                kD *= 1.0 - ORM.b; 

                vec3 numerator    = NDF * G * F;
                float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
                vec3 specular = numerator / denominator;

                float NdotL = max(dot(normal, L), 0.0);
                lightResult += (kD * diffuseTexture / PI + specular) * attenuation * NdotL;
                //lightResult += (kD * diffuseTexture / PI + specular) * CalcPointLight(pointLights[i], normal);
           }
            vec3 ambient = vec3(0.03) * diffuseTexture * ORM.r;
            vec3 color = ambient * lightResult;

            color = color / (color + vec3(1.0));
            color = pow(color, vec3(1.0/2.2));

            FragColor = vec4(color, 1.0f);
        }