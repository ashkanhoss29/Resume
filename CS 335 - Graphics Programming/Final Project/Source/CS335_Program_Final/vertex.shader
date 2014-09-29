varying vec3 N;
varying vec3 V;
varying vec3 gEyePosW;

uniform vec3 eyePosW;

mat3 GetLinearPart( mat4 m )
{
   mat3 result;
	
   result[0][0] = m[0][0]; 
   result[0][1] = m[0][1]; 
   result[0][2] = m[0][2]; 

   result[1][0] = m[1][0]; 
   result[1][1] = m[1][1]; 
   result[1][2] = m[1][2]; 
	
   result[2][0] = m[2][0]; 
   result[2][1] = m[2][1]; 
   result[2][2] = m[2][2]; 
	
   return result;
}

void main()
{
   gEyePosW = eyePosW;

   V = vec3(gl_ModelViewMatrix * gl_Vertex);
   N = gl_Normal;
   //N = normalize(gl_NormalMatrix * gl_Normal);
   //N = gl_NormalMatrix * gl_Normal;


   gl_TexCoord[0] = gl_MultiTexCoord0;


   mat3 ModelWorld = GetLinearPart(gl_ModelViewMatrix);

   vec3 tempN = normalize(ModelWorld * gl_Normal);
   vec3 tempE = normalize(V.xyz - eyePosW);

   gl_TexCoord[1].xyz = reflect(tempE,tempN);
   gl_TexCoord[1].y = -gl_TexCoord[1].y;


   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   //gl_Position = ftransform();
}