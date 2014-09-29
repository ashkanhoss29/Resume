const vec2 madd = vec2(0.5,0.5);

void main()
{
   //gl_Position = ftransform();
  
   // Clean up inaccuracies
   //vec2 Pos;
   //Pos = sign(gl_Vertex.xy);
 
   //gl_Position = vec4(Pos, 0.0, 1.0);
   // Image-space
   //gl_TexCoord[0] = Pos * madd+madd;

   gl_TexCoord[0].xy = gl_Vertex.xy * madd+madd;
   //gl_TexCoord[0] = gl_TexCoord[0];
   gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);
}