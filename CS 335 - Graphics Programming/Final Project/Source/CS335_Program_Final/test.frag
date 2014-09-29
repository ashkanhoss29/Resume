uniform sampler2D colorMap;
uniform sampler2D normalMap;

void main()
{
  vec3 theColor = vec3(texture2D(colorMap, (gl_TexCoord[0].st)));

  gl_FragColor = vec4(theColor, 0.0);
}