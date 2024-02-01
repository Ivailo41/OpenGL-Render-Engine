#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec3 aNormal;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 3) in vec3 aTangent;

        out VS_OUT
        {
            out vec3 FragPos;
            out vec2 TexCoord;
            out vec3 TanViewDirection;
            out mat3 TanMatrix;
        } vs_out;

        uniform mat4 transform;
        uniform mat4 viewMat;
        uniform mat4 perspectiveMat;
        uniform vec3 viewDirection;

        void main()
        {
            vec3 T = normalize(vec3(transform * vec4(aTangent,   0.0)));
            vec3 B = normalize(vec3(transform * vec4(cross(aNormal, aTangent),   0.0)));
            vec3 N = normalize(vec3(transform * vec4(aNormal,    0.0)));

            mat3 TBN = transpose(mat3(T, B, N));
            vs_out.TanViewDirection = TBN * -viewDirection;
            vs_out.FragPos = vec3(transform * vec4(aPos, 1.0f)); //pass vertices in world position to the fragmet shader
            vs_out.TexCoord = aTexCoord;
            vs_out.TanMatrix = TBN;


            gl_Position = perspectiveMat * viewMat * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }