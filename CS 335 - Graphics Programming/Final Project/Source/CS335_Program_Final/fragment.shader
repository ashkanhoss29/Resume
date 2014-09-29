struct Light
{
   vec3 pos;
   vec3 dir;
   vec4 ambient;
   vec4 diffuse;
   vec4 spec;
   vec3 att;
   float spotPower;
   float range;
};

struct SurfaceInfo
{
   vec3 pos;
   vec3 normal;
   vec4 diffuse;
   vec4 spec;
};

varying vec3 N;
varying vec3 V;
varying vec3 gEyePosW;

uniform Light light1;

uniform sampler2D diffuseMap;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform samplerCube cubeMap;

const float reflect_factor = 0.2;

void ParallelLight(in SurfaceInfo s, in Light L, in vec3 eyePos, out vec3 litColor)
{
     vec3 lightVec = -L.dir;

     litColor = s.diffuse * L.ambient;

     float diffuseFactor = dot(lightVec, s.normal);

     litColor = litColor + diffuseFactor * s.diffuse * L.diffuse;
}

void main()
{
   vec4 diffuse = texture2D(diffuseMap, (gl_TexCoord[0].st));
   vec4 specular = texture2D(specMap, (gl_TexCoord[0].st));
   vec3 cube = textureCube(cubeMap, gl_TexCoord[1].xyz).rgb;

   SurfaceInfo s = {V, N, diffuse, specular};

   vec3 litColor = vec3(0,0,0);
   ParallelLight(s, light1, gEyePosW, litColor);

   //gl_FragColor = vec4(litColor, 0.0);
   gl_FragColor = vec4(mix(litColor, cube, reflect_factor), 0.0);

   if (specular.r > 0.99 && specular.g > 0.99 && specular.b > 0.99)
   {
      discard;
   }


   //color = mix(litColor, cube, reflect_factor);

   //vec3 reflectionDirection = normalize(reflect(V, normalize(N)));
   //reflectionDirection.y = -reflectionDirection.y;
   //vec4 fragColor = textureCube(cubeMap, reflectionDirection);

   //vec3 theDiffuse = vec3(texture2D(diffuseMap, (gl_TexCoord[0].st)));
   //vec3 L = normalize(gl_LightSource[0].position.xyz - V);
   //vec4 Idiff = gl_LightSource[0].diffuse * max(dot(N,L), 0.0);
   //Idiff = clamp(Idiff, 0.0, 1.0);

   //gl_FragColor = Idiff * vec4(theDiffuse, 0.0);
   //gl_FragColor = fragColor;
   //gl_FragColor = vec4(theDiffuse, 0.0) + fragColor;
   //gl_FragColor = vec4(0.5, 0, 0, 0);
}