uniform sampler2D bgl_RenderedTexture;
uniform sampler2D bgl_DepthTexture;

void main()
{
   vec4 color1 = texture2D(bgl_RenderedTexture, gl_TexCoord[0].st);
   gl_FragColor = color1;
}
