/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cs335;

import com.obj.Group;
import com.obj.TextureCoordinate;
import com.obj.Vertex;
import com.sun.opengl.util.GLUT;
import java.io.BufferedReader;
import java.io.FileReader;
import java.nio.IntBuffer;
import java.sql.Timestamp;
import java.util.ArrayList;
import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;
import objimp.ObjImpScene;

public class GLRender implements GLEventListener {

    CarPhysics physics = new CarPhysics();
    private GL gl;
    private GLU glu;
    public int wWidth;
    public int wHeight;
    float minute = 0;
    float hour = 0;
    float second = 0;
    public boolean overviewMode = false;
    Vector3d cameraPos = new Vector3d();
    Vector3d targetPos = new Vector3d();
    float movement = 0.0f;
    double theta = Math.PI / 2;
    double phi = Math.PI / 2;
    float transX = -3.0f;
    float transY = -3.0f;
    float thetaX = 0.0f;
    float thetaY = 0.0f;
    float scaleU = 0.2f;
    float lightPosX = 0;
    float lightPosY = 0;
    Timestamp ts;
    String dateStr1;
    int shaderV = 0;
    int shaderF = 0;
    int programObject = 0;
    int samplerDiffuse = 0;
    int samplerSpecular = 0;
    int samplerNormal = 0;
    int samplerCubeMap = 0;
    int light1[] = new int[8];
    int eyePosW = 0;
    int postShaderV = 0;
    int postShaderF = 0;
    int postProgramObject = 0;
    int samplerPostColor = 0;
    int samplerPostDepth = 0;
    int uniformWidth = 0;
    int uniformHeight = 0;
	int uniformFocalLength = 0;
	public float focalLength = 15.0f;
    int texRender_FBO;
    int texRender_Depth;
    int texRender_Color;
    int skyShaderV = 0;
    int skyShaderF = 0;
    int skyProgramObject = 0;
    int samplerSky = 0;
    final int numberOfTextures = 30;
    int texture[] = new int[numberOfTextures];
    int cubeText = 0;
    boolean lightOn = true;
    boolean blendOn = false;
    boolean replaceOn = false;
    boolean circleTrack = true;
    int onCircle = 0;
    Mesh mesh, wheel1, wheel2, wheel3, wheel4;
    Mesh building1;
    Mesh segment1, segment2, segment3, segment4, segment5, segment6;
    Mesh roadStrip;
    Mesh outsideMesh, insideMesh;
    Mesh ground;
    Mesh car1, car2, car3, car4;
    Mesh panel;
    Mesh tree;
    Mesh cherry;
    Mesh branch1, branch2, smallCherry, largeCherry;
    int thetaCar = 0;
    int thetaEight = -45;
    float xEight = 0;
    float yEight = 0;
    ObjImpScene scene;
    //ObjImpMesh objMesh;

    public void init(GLAutoDrawable drawable) {
        gl = drawable.getGL();
        glu = new GLU();

        //Set up the fbo textures
        texRender_FBO = genFBO(gl);
        gl.glBindFramebufferEXT(GL.GL_FRAMEBUFFER_EXT, this.texRender_FBO);
        texRender_Color = genTexture(gl);
        texRender_Depth = genTexture(gl);

        int mytexture[] = new int[1];
        gl.glGenTextures(1, mytexture, 0);
        cubeText = mytexture[0];
        gl.glBindTexture(GL.GL_TEXTURE_CUBE_MAP, mytexture[0]);
        gl.glPixelStorei(GL.GL_UNPACK_ALIGNMENT, 1);
        gl.glTexParameteri(GL.GL_TEXTURE_CUBE_MAP, GL.GL_TEXTURE_WRAP_S,
                GL.GL_REPEAT);
        gl.glTexParameteri(GL.GL_TEXTURE_CUBE_MAP, GL.GL_TEXTURE_WRAP_T,
                GL.GL_REPEAT);
        gl.glTexParameteri(GL.GL_TEXTURE_CUBE_MAP, GL.GL_TEXTURE_WRAP_R,
                GL.GL_REPEAT);
        gl.glTexParameteri(GL.GL_TEXTURE_CUBE_MAP, GL.GL_TEXTURE_MAG_FILTER,
                GL.GL_NEAREST);
        gl.glTexParameteri(GL.GL_TEXTURE_CUBE_MAP, GL.GL_TEXTURE_MIN_FILTER,
                GL.GL_NEAREST);

        loadTexture("SaintLazarusChurch_CubeMap\\posx.jpg", GL.GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubeText);
        loadTexture("SaintLazarusChurch_CubeMap\\negx.jpg", GL.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cubeText);
        loadTexture("SaintLazarusChurch_CubeMap\\negy.jpg", GL.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cubeText);
        loadTexture("SaintLazarusChurch_CubeMap\\posy.jpg", GL.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cubeText);
        loadTexture("SaintLazarusChurch_CubeMap\\posz.jpg", GL.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cubeText);
        loadTexture("SaintLazarusChurch_CubeMap\\negz.jpg", GL.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cubeText);

        gl.glTexGeni(GL.GL_S, GL.GL_TEXTURE_GEN_MODE, GL.GL_NORMAL_MAP);
        gl.glTexGeni(GL.GL_T, GL.GL_TEXTURE_GEN_MODE, GL.GL_NORMAL_MAP);
        gl.glTexGeni(GL.GL_R, GL.GL_TEXTURE_GEN_MODE, GL.GL_NORMAL_MAP);
        gl.glEnable(GL.GL_TEXTURE_GEN_S);
        gl.glEnable(GL.GL_TEXTURE_GEN_T);
        gl.glEnable(GL.GL_TEXTURE_GEN_R);
        gl.glTexEnvf(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_MODULATE);

        gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
        gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
        gl.glClearDepth(1.0f);                      // Depth Buffer Setup
        gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
        gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
        gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations

        gl.glEnable(GL.GL_TEXTURE_2D);


        gl.glTexEnvf(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_MODULATE);

        //gl.glEnable(GL.GL_TEXTURE_CUBE_MAP);


        // Load shaders ********************************************************
        shaderV = gl.glCreateShader(GL.GL_VERTEX_SHADER_ARB);
        shaderF = gl.glCreateShader(GL.GL_FRAGMENT_SHADER_ARB);

        skyShaderV = gl.glCreateShader(GL.GL_VERTEX_SHADER_ARB);
        skyShaderF = gl.glCreateShader(GL.GL_FRAGMENT_SHADER_ARB);

        postShaderV = gl.glCreateShader(GL.GL_VERTEX_SHADER_ARB);
        postShaderF = gl.glCreateShader(GL.GL_FRAGMENT_SHADER_ARB);

        loadShader(shaderV, "vertex.shader");
        loadShader(shaderF, "fragment.shader");

        loadShader(skyShaderV, "sky.vert");
        loadShader(skyShaderF, "sky.frag");

        loadShader(postShaderV, "post.vert");
        loadShader(postShaderF, "post2.frag");

        programObject = gl.glCreateProgram();
        gl.glAttachShader(programObject, shaderV);
        gl.glAttachShader(programObject, shaderF);
        gl.glLinkProgram(programObject);
        gl.glValidateProgram(programObject);

        //Texture uniforms from shader
        samplerDiffuse = gl.glGetUniformLocationARB(programObject, "diffuseMap");
        samplerSpecular = gl.glGetUniformLocationARB(programObject, "specMap");
        samplerNormal = gl.glGetUniformLocationARB(programObject, "normalMap");
        samplerCubeMap = gl.glGetUniformLocationARB(programObject, "cubeMap");

        //Light uniforms from shader
        light1[0] = gl.glGetUniformLocationARB(programObject, "light1.pos");
        light1[1] = gl.glGetUniformLocationARB(programObject, "light1.dir");
        light1[2] = gl.glGetUniformLocationARB(programObject, "light1.ambient");
        light1[3] = gl.glGetUniformLocationARB(programObject, "light1.diffuse");
        light1[4] = gl.glGetUniformLocationARB(programObject, "light1.spec");
        light1[5] = gl.glGetUniformLocationARB(programObject, "light1.att");
        light1[6] = gl.glGetUniformLocationARB(programObject, "light1.spotPower");
        light1[7] = gl.glGetUniformLocationARB(programObject, "light1.range");

        //Eye position uniform
        eyePosW = gl.glGetUniformLocationARB(programObject, "eyePosW");


        skyProgramObject = gl.glCreateProgram();
        gl.glAttachShader(skyProgramObject, skyShaderV);
        gl.glAttachShader(skyProgramObject, skyShaderF);
        gl.glLinkProgram(skyProgramObject);
        gl.glValidateProgram(skyProgramObject);
        samplerSky = gl.glGetUniformLocationARB(skyProgramObject, "cubeMap");

        postProgramObject = gl.glCreateProgram();
        gl.glAttachShader(postProgramObject, postShaderV);
        gl.glAttachShader(postProgramObject, postShaderF);
        gl.glLinkProgram(postProgramObject);
        gl.glValidateProgram(postProgramObject);
        samplerPostColor = gl.glGetUniformLocationARB(postProgramObject, "bgl_RenderedTexture");
        samplerPostDepth = gl.glGetUniformLocationARB(postProgramObject, "bgl_DepthTexture");
        uniformWidth = gl.glGetUniformLocationARB(postProgramObject, "bgl_RenderedTextureWidth");
        uniformHeight = gl.glGetUniformLocationARB(postProgramObject, "bgl_RenderedTextureHeight");
		uniformFocalLength = gl.glGetUniformLocationARB(postProgramObject, "focalLength");


        // Load textures ********************************************************
        gl.glGenTextures(numberOfTextures, texture, 0); //Last six reserved for cube map


        // Asset textures
        loadTexture("defaultNormal.png", GL.GL_TEXTURE_2D, 3);
        loadTexture("specular.png", GL.GL_TEXTURE_2D, 4);
        loadTexture("curve.png", GL.GL_TEXTURE_2D, 5);
        loadTexture("straight.png", GL.GL_TEXTURE_2D, 6);
        loadTexture("building1Diffuse.png", GL.GL_TEXTURE_2D, 7);
        loadTexture("groundDiffuse.png", GL.GL_TEXTURE_2D, 8);
        loadTexture("car1Diffuse.png", GL.GL_TEXTURE_2D, 9);
        loadTexture("tree.png", GL.GL_TEXTURE_2D, 10);
        loadTexture("roadStripDiffuse.png", GL.GL_TEXTURE_2D, 11);
        loadTexture("car2Diffuse.png", GL.GL_TEXTURE_2D, 12);
        loadTexture("car3Diffuse.png", GL.GL_TEXTURE_2D, 13);
        loadTexture("car4Diffuse.png", GL.GL_TEXTURE_2D, 14);
        loadTexture("panel.png", GL.GL_TEXTURE_2D, 15);
        loadTexture("cherryDiffuse.png", GL.GL_TEXTURE_2D, 16);
        loadTexture("branch1Diffuse.png", GL.GL_TEXTURE_2D, 17);
        loadTexture("branch2Diffuse.png", GL.GL_TEXTURE_2D, 18);
        loadTexture("smallCherryDiffuse.png", GL.GL_TEXTURE_2D, 19);
        loadTexture("largeCherryDiffuse.png", GL.GL_TEXTURE_2D, 20);


        //Car
        //mesh = new Mesh("Road\\straight2.obj", texture[1], texture[4], texture[3], false);
        //wheel1 = new Mesh("Marcos\\wheel1.obj", texture[0], texture[4], texture[3], true);
        //wheel2 = new Mesh("Marcos\\wheel2.obj", texture[0], texture[4], texture[3], true);
        //wheel3 = new Mesh("Marcos\\wheel3.obj", texture[0], texture[4], texture[3], true);
        //wheel4 = new Mesh("Marcos\\wheel4.obj", texture[0], texture[4], texture[3], true);

        //Buildings
        building1 = new Mesh("Buildings\\building1.obj", texture[7], texture[4], texture[3], false);

        //Track
        segment1 = new Mesh("Road\\segment1.obj", texture[5], texture[4], texture[3], false);
        segment2 = new Mesh("Road\\segment2.obj", texture[6], texture[4], texture[3], false);
        segment3 = new Mesh("Road\\segment3.obj", texture[5], texture[4], texture[3], false);
        segment4 = new Mesh("Road\\segment4.obj", texture[5], texture[4], texture[3], false);
        segment5 = new Mesh("Road\\segment5.obj", texture[6], texture[4], texture[3], false);
        segment6 = new Mesh("Road\\segment6.obj", texture[5], texture[4], texture[3], false);

        ground = new Mesh("Ground\\ground.obj", texture[8], texture[4], texture[3], false);
        car1 = new Mesh("Cars\\car1.obj", texture[9], texture[4], texture[3], false);
        car2 = new Mesh("Cars\\car1.obj", texture[12], texture[4], texture[3], false);
        car3 = new Mesh("Cars\\car1.obj", texture[13], texture[4], texture[3], false);
        car4 = new Mesh("Cars\\car1.obj", texture[14], texture[4], texture[3], false);
        panel = new Mesh("Cars\\panel.obj", texture[15], texture[4], texture[3], false);
        tree = new Mesh("Trees\\tree.obj", texture[10], texture[10], texture[3], false);
        roadStrip = new Mesh("Road\\roadStrip.obj", texture[11], texture[4], texture[3], false);
        cherry = new Mesh("BoostItems\\cherry.obj", texture[16], texture[4], texture[3], false);
        branch1 = new Mesh("Trees\\branch1.obj", texture[17], texture[4], texture[3], false);
        branch2 = new Mesh("Trees\\branch2.obj", texture[18], texture[4], texture[3], false);
        smallCherry = new Mesh("Trees\\smallCherry.obj", texture[19], texture[4], texture[3], false);
        largeCherry = new Mesh("Trees\\largeCherry.obj", texture[20], texture[4], texture[3], false);


        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // KEN: these are the mesh objects that have the vertices you need
        outsideMesh = new Mesh("Road\\outsideEdge.obj", texture[1], texture[1], texture[1], false);
        insideMesh = new Mesh("Road\\insideEdge.obj", texture[1], texture[1], texture[1], false);
        // I have set them for you here:
        physics.outsideEdge = outsideMesh;
        physics.insideEdge = insideMesh;


        cameraPos.x = 16f;
        cameraPos.y = 1.7f;
        cameraPos.z = -7.5f;

        targetPos.x = 20.8f;
        targetPos.y = 0.38f;
        targetPos.z = -10.0f;

        //scene = new ObjImpScene(gl);
        //objMesh = new ObjImpMesh();

        /*
         if(mesh != null)
         {
         if(mesh.object == null) return;
         ArrayList<Group> group = mesh.object.getGroups();
			
         for(int i = 0; i < group.size(); i++)
         {
         ObjImpMesh objMesh = new ObjImpMesh();
				
         ArrayList<Integer> indices = group.get(i).indices;
         ArrayList<Vertex> vertices = group.get(i).vertices;
         ArrayList<Vertex> normals = group.get(i).normals;
         ArrayList<TextureCoordinate> textureCoord = group.get(i).texcoords;
				
         int counter = 0;
         while(counter < indices.size())
         {
					
         }
         }
         }
         * 
         */
    }

    private int genRB(GL gl) {
        int[] array = new int[1];
        IntBuffer ib = IntBuffer.wrap(array);
        gl.glGenRenderbuffersEXT(1, ib);
        return ib.get(0);
    }

    private int genFBO(GL gl) {
        int[] array = new int[1];
        IntBuffer ib = IntBuffer.wrap(array);
        gl.glGenFramebuffersEXT(1, ib);
        return ib.get(0);
    }

    private int genTexture(GL gl) {
        final int[] tmp = new int[1];
        gl.glGenTextures(1, tmp, 0);
        return tmp[0];
    }

    private void loadShader(int shader, String fileName) {
        try {
            BufferedReader brv = new BufferedReader(new FileReader(fileName));
            String vsrc = "";
            String line;
            while ((line = brv.readLine()) != null) {
                vsrc += line + "\n";
            }
            gl.glShaderSource(shader, 1, new String[]{vsrc}, (int[]) null, 0);
        } catch (Exception ex) {
            System.out.println("Failed to load shader");
        }
        gl.glCompileShader(shader);
    }

    private void makeRGBTexture(GL gl, GLU glu, TextureReader.Texture img, int target, boolean mipmapped) {
        if (target == GL.GL_TEXTURE_2D) {
            if (mipmapped) {
                glu.gluBuild2DMipmaps(target, GL.GL_RGB8, img.getWidth(), img.getHeight(), GL.GL_RGB, GL.GL_UNSIGNED_BYTE, img.getPixels());
            } else {
                gl.glTexImage2D(target, 0, GL.GL_RGB, img.getWidth(), img.getHeight(), 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, img.getPixels());
            }
        } else {
            gl.glTexImage2D(target, 0, GL.GL_RGBA, img.getWidth(), img.getHeight(), 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, img.getPixels());
        }
    }

    private void loadTexture(String name, int target, int id) {
        if (target == GL.GL_TEXTURE_2D) {
            gl.glBindTexture(target, texture[id]);
        } else //cube map
        {
            //gl.glBindTexture(GL.GL_TEXTURE_CUBE_MAP, myText);
        }

        TextureReader.Texture textureRead = null;
        try {
            textureRead = TextureReader.readTexture(name);
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        }
        makeRGBTexture(gl, glu, textureRead, target, false);

        if (target == GL.GL_TEXTURE_2D) {
            float maxAnisotropy[] = new float[1];
            gl.glGetFloatv(GL.GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy, 0);
            if (maxAnisotropy[0] == 16.0f) {
                gl.glTexParameteri(target, GL.GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
            }

            gl.glTexParameteri(target, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
            gl.glTexParameteri(target, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
        }
    }

    //OVERHEAD FUNCTIONS
    public void RotateX(float theta) {
        this.thetaX = this.thetaX + theta;
    }

    public void RotateY(float theta) {
        thetaY = thetaY + theta;
    }

    public void TranslateX(float x) {
        transX = transX + x;
    }

    public void TranslateY(float y) {
        transY = transY + y;
    }

    public void Scale(float s) {
        scaleU = scaleU + s;
    }

    public void reset() {
        thetaX = 0.0f;
        thetaY = 0.0f;
        transX = 0.0f;
        transY = 0.0f;
        scaleU = 1.0f;
    }

    //FIRST VERSION VIEW FUNCTIONS
    public void look(double lookTheta, double lookPhi) {
        theta = theta + lookTheta;
        phi = phi + lookPhi;

        if (phi < 0.1) {
            phi = 0.1;
        }
        if (phi > Math.PI - 0.1) {
            phi = Math.PI - 0.1;
        }


    }

    public void move(boolean forward) {
        if (forward) {
            movement = 1.0f;
        } else {
            movement = -1.0f;
        }
    }

    public void resetFirstPerson() {
        cameraPos.x = 16f;
        cameraPos.y = 1.7f;
        cameraPos.z = -7.5f;
        theta = Math.PI / 2;
        phi = Math.PI / 2;

        transX = -3.0f;
        transY = -3.0f;
    }

    public void drawBase(float x, float y) {
        gl.glPushMatrix();
        gl.glTranslatef(x * 2, y * 2, 0.0f);
        gl.glRotatef(180, 0, 0, 1);
        gl.glBindTexture(GL.GL_TEXTURE_2D, texture[0]);

        gl.glBegin(GL.GL_QUADS);
        // Front Face
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        // Back Face
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        // Top Face
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        // Bottom Face
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        // Right face
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        // Left Face
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glEnd();
        gl.glPopMatrix();
    }

    private void drawCubeMap(float eyeX, float eyeY, float eyeZ) {
        //Forward
        gl.glDisable(GL.GL_DEPTH_TEST);

        gl.glUseProgram(skyProgramObject);

        gl.glUniform1i(samplerSky, 0);
        gl.glActiveTexture(GL.GL_TEXTURE0);
        gl.glBindTexture(GL.GL_TEXTURE_CUBE_MAP, cubeText);


        gl.glPushMatrix();
        gl.glTranslated(eyeX, eyeY, eyeZ);
        //gl.glRotatef(180, 0, 1, 0);


        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[9]);
        gl.glBegin(GL.GL_QUADS);
        // Front Face
        gl.glNormal3d(0, 0, 1);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(-1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(-1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glEnd();

        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[8]);
        gl.glBegin(GL.GL_QUADS);
        // Back Face
        gl.glNormal3d(0, 0, -1);
        gl.glTexCoord2f(-1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(-1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glEnd();

        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[6]);
        gl.glBegin(GL.GL_QUADS);
        // Top Face
        gl.glNormal3d(0, 1, 0);
        gl.glTexCoord2f(0.0f, -1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, -1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glEnd();

        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[7]);
        gl.glBegin(GL.GL_QUADS);
        // Bottom Face
        gl.glNormal3d(0, -1, 0);
        gl.glTexCoord2f(-1.0f, 1.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(-1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glEnd();

        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[4]);
        gl.glBegin(GL.GL_QUADS);
        // Right face
        gl.glNormal3d(1, 0, 0);

        gl.glTexCoord2f(-1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(-1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glEnd();

        //gl.glBindTexture(GL.GL_TEXTURE_2D, texture[5]);
        gl.glBegin(GL.GL_QUADS);
        // Left Face
        gl.glNormal3d(-1, 0, 0);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(-1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(-1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glEnd();


        gl.glPopMatrix();

        gl.glUseProgram(0);
        gl.glEnable(GL.GL_DEPTH_TEST);
    }

    private void drawMesh(Mesh m) {
        if (m != null) {
            gl.glUseProgram(programObject);

            //Load diffuse texture
            gl.glUniform1i(samplerDiffuse, 0);
            gl.glActiveTexture(GL.GL_TEXTURE0);
            gl.glBindTexture(GL.GL_TEXTURE_2D, m.diffuse);


            //Load spec texture
            gl.glUniform1i(samplerSpecular, 1);
            gl.glActiveTexture(GL.GL_TEXTURE1);
            gl.glBindTexture(GL.GL_TEXTURE_2D, m.specular);


            //Load normal map
            gl.glUniform1i(samplerNormal, 2);
            gl.glActiveTexture(GL.GL_TEXTURE2);
            gl.glBindTexture(GL.GL_TEXTURE_2D, m.normal);


            //Load cube map
            gl.glUniform1i(samplerCubeMap, 3);
            gl.glActiveTexture(GL.GL_TEXTURE3);
            gl.glBindTexture(GL.GL_TEXTURE_CUBE_MAP, cubeText);


            //Pass light1
            gl.glUniform3fARB(light1[1], 0.576f, -0.576f, -0.576f);
            gl.glUniform4fARB(light1[2], 0.8f, 0.8f, 0.8f, 1.0f);
            gl.glUniform4fARB(light1[3], 0.8f, 0.8f, 0.8f, 1.0f);
            gl.glUniform4fARB(light1[4], 0.5f, 0.5f, 0.5f, 1.0f);


            //Pass eye position in world space
            gl.glUniform3fARB(eyePosW, cameraPos.x, cameraPos.y, cameraPos.z);


            if (!m.usingLoader) {
                //render
                int counter = 0;
                while (counter < m.vIndices.size()) {
                    gl.glBegin(GL.GL_TRIANGLES);

                    gl.glNormal3f(
                            m.normals.get(m.nIndices.get(counter)).getX(),
                            m.normals.get(m.nIndices.get(counter)).getY(),
                            m.normals.get(m.nIndices.get(counter)).getZ());
                    gl.glTexCoord2f(
                            m.textureCoords.get(m.tIndices.get(counter)).getU(),
                            -m.textureCoords.get(m.tIndices.get(counter)).getV());
                    gl.glVertex3f(
                            m.vertices.get(m.vIndices.get(counter)).getX(),
                            m.vertices.get(m.vIndices.get(counter)).getY(),
                            m.vertices.get(m.vIndices.get(counter)).getZ());


                    gl.glNormal3f(
                            m.normals.get(m.nIndices.get(counter + 1)).getX(),
                            m.normals.get(m.nIndices.get(counter + 1)).getY(),
                            m.normals.get(m.nIndices.get(counter + 1)).getZ());
                    gl.glTexCoord2f(
                            m.textureCoords.get(m.tIndices.get(counter + 1)).getU(),
                            -m.textureCoords.get(m.tIndices.get(counter + 1)).getV());
                    gl.glVertex3f(
                            m.vertices.get(m.vIndices.get(counter + 1)).getX(),
                            m.vertices.get(m.vIndices.get(counter + 1)).getY(),
                            m.vertices.get(m.vIndices.get(counter + 1)).getZ());


                    gl.glNormal3f(
                            m.normals.get(m.nIndices.get(counter + 2)).getX(),
                            m.normals.get(m.nIndices.get(counter + 2)).getY(),
                            m.normals.get(m.nIndices.get(counter + 2)).getZ());
                    gl.glTexCoord2f(
                            m.textureCoords.get(m.tIndices.get(counter + 2)).getU(),
                            -m.textureCoords.get(m.tIndices.get(counter + 2)).getV());
                    gl.glVertex3f(
                            m.vertices.get(m.vIndices.get(counter + 2)).getX(),
                            m.vertices.get(m.vIndices.get(counter + 2)).getY(),
                            m.vertices.get(m.vIndices.get(counter + 2)).getZ());
                    gl.glEnd();
                    counter = counter + 3;
                }

                return;
            }

            if (m.object == null) {
                return;
            }
            ArrayList<Group> group = m.object.getGroups();

            for (int i = 0; i < group.size(); i++) {
                ArrayList<Integer> indices = group.get(i).indices;
                ArrayList<Vertex> vertices = group.get(i).vertices;
                ArrayList<Vertex> normals = group.get(i).normals;
                ArrayList<TextureCoordinate> textureCoord = group.get(i).texcoords;

                int counter = 0;
                while (counter < group.get(i).indexCount) {
                    //	System.out.println(
                    //			indices.get(counter) + " " +
                    //			indices.get(counter+1) + " " +
                    //			indices.get(counter+2)
                    //			);
                    gl.glBegin(GL.GL_TRIANGLES);
                    /*
                     gl.glNormal3f(
                     normals.get(counter).getX(),
                     normals.get(counter).getY(),
                     normals.get(counter).getZ()
                     );
                     * 
                     */
                    gl.glNormal3f(
                            normals.get(indices.get(counter)).getX(),
                            normals.get(indices.get(counter)).getY(),
                            normals.get(indices.get(counter)).getZ());
                    gl.glTexCoord2f(
                            textureCoord.get(counter).getU(),
                            -textureCoord.get(counter).getV());
                    gl.glVertex3f(
                            vertices.get(indices.get(counter)).getX(),
                            vertices.get(indices.get(counter)).getY(),
                            vertices.get(indices.get(counter)).getZ());


                    gl.glNormal3f(
                            normals.get(indices.get(counter + 1)).getX(),
                            normals.get(indices.get(counter + 1)).getY(),
                            normals.get(indices.get(counter + 1)).getZ());
                    gl.glTexCoord2f(
                            textureCoord.get(counter + 1).getU(),
                            -textureCoord.get(counter + 1).getV());
                    gl.glVertex3f(
                            vertices.get(indices.get(counter + 1)).getX(),
                            vertices.get(indices.get(counter + 1)).getY(),
                            vertices.get(indices.get(counter + 1)).getZ());


                    gl.glNormal3f(
                            normals.get(indices.get(counter + 2)).getX(),
                            normals.get(indices.get(counter + 2)).getY(),
                            normals.get(indices.get(counter + 2)).getZ());
                    gl.glTexCoord2f(
                            textureCoord.get(counter + 2).getU(),
                            -textureCoord.get(counter + 2).getV());
                    gl.glVertex3f(
                            vertices.get(indices.get(counter + 2)).getX(),
                            vertices.get(indices.get(counter + 2)).getY(),
                            vertices.get(indices.get(counter + 2)).getZ());
                    gl.glEnd();
                    counter = counter + 3;
                }
            }
        }
        gl.glUseProgram(0);
    }

    public void display(GLAutoDrawable drawable) {

        //Render to texture
        gl.glBindTexture(GL.GL_TEXTURE_2D, 0);
        gl.glEnable(GL.GL_TEXTURE_2D);
        gl.glBindFramebufferEXT(GL.GL_FRAMEBUFFER_EXT, this.texRender_FBO);

        gl.glClearColor(0.5f, 1.0f, 0.8f, 0.0f);
        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();

        camera();
        drawCubeMap(cameraPos.x, cameraPos.y, cameraPos.z);



        ////////////////////////////////////////////////////////////////////////////////////////
        // KEN:

         //PLAYER
                //                   KEN, draw this when in overview mode ***********************
        //                                      drawMesh(panel);
        if(overviewMode) {
            physics.drawCar(gl);
            gl.glScalef(0.006f, 0.006f, 0.006f);
            drawMesh(car1);
            gl.glLoadIdentity();       
        }
        else {
            physics.drawPanel(gl);
            gl.glRotated(-90, 0, 1, 0);
            gl.glScalef(0.2f, 0.2f, 0.2f);
            drawMesh(panel);
            gl.glLoadIdentity();
        }
        //CPU 1
        physics.drawCPUCar1(gl);
        gl.glScalef(0.006f, 0.006f, 0.006f);
        drawMesh(car2);
        gl.glLoadIdentity();

        //CPU 2
        physics.drawCPUCar2(gl);
        gl.glScalef(0.006f, 0.006f, 0.006f);
        drawMesh(car3);
        gl.glLoadIdentity();
        
        //CPU 3
        physics.drawCPUCar3(gl);
        gl.glScalef(0.006f, 0.006f, 0.006f);
        drawMesh(car4);
        gl.glLoadIdentity();


        physics.display(gl, overviewMode);
        gl.glLoadIdentity();

        /////////////////////////////////////////////////////////////////////////////////////////////



        //MOVE EVERYTHING DOWN:
        gl.glPushMatrix();
        gl.glTranslatef(0, -1.2f, 0);


        drawMesh(segment1);
        drawMesh(segment2);
        drawMesh(segment3);
        drawMesh(segment4);
        drawMesh(segment5);
        drawMesh(segment6);

        drawMesh(roadStrip);

        drawMesh(outsideMesh);
        drawMesh(insideMesh);

        drawMesh(ground);

//        gl.glPushMatrix();
//        gl.glTranslatef(-30f, 1.2f, 15f);
//        gl.glScalef(0.002f, 0.002f, 0.002f);
//        drawMesh(car1);
//        gl.glPopMatrix();
//
//        gl.glPushMatrix();
//        gl.glTranslatef(-30f, 1.2f, 13f);
//        gl.glScalef(0.002f, 0.002f, 0.002f);
//        drawMesh(car2);
//        gl.glPopMatrix();
//
//        gl.glPushMatrix();
//        gl.glTranslatef(-30f, 1.2f, 10f);
//        gl.glScalef(0.002f, 0.002f, 0.002f);
//        drawMesh(car3);
//        gl.glPopMatrix();
//
//        gl.glPushMatrix();
//        gl.glTranslatef(-30f, 1.2f, 8f);
//        gl.glScalef(0.002f, 0.002f, 0.002f);
//        drawMesh(car4);
//        gl.glPopMatrix();

        gl.glPushMatrix();
        gl.glTranslatef(-10f, 0f, -10f);
        gl.glRotatef(45, 0, 1, 0);
        drawMesh(tree);
        gl.glPopMatrix();


        gl.glPushMatrix();
        gl.glTranslatef(-30f, 1.5f, 0f);
        gl.glScalef(0.5f, 0.5f, 0.5f);
        if(physics.boost == false) {
            drawMesh(cherry);
        }
        gl.glPopMatrix();


        gl.glPushMatrix();
        gl.glTranslatef(-40f, 1.5f, 40f);
        //gl.glScalef(0.5f, 0.5f, 0.5f);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();

        gl.glPushMatrix();
        gl.glTranslatef(40f, 1.5f, -60f);
        gl.glRotatef(45, 0, 1, 0);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();

        gl.glPushMatrix();
        gl.glTranslatef(80f, 1.5f, -50f);
        gl.glScalef(1.5f, 1.5f, 1.5f);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();

        gl.glPushMatrix();
        gl.glTranslatef(-20f, 1.5f, -70f);
        gl.glRotatef(20, 0, 1, 0);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();

        gl.glPushMatrix();
        gl.glTranslatef(-90f, 1.5f, 40f);
        gl.glRotatef(60, 0, 1, 0);
        gl.glScalef(0.8f, 0.8f, 0.8f);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();
		
		gl.glPushMatrix();
        gl.glTranslatef(-90f, 1.5f, -90f);
        gl.glRotatef(60, 0, 1, 0);
        gl.glScalef(1.8f, 1.8f, 1.8f);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();
		
		gl.glPushMatrix();
        gl.glTranslatef(90f, 1.5f, 90f);
        gl.glRotatef(60, 0, 1, 0);
        gl.glScalef(0.8f, 0.8f, 0.8f);
        drawMesh(branch1);
        drawMesh(branch2);
        drawMesh(smallCherry);
        drawMesh(largeCherry);
        gl.glPopMatrix();


        gl.glPushMatrix();
        gl.glTranslatef(10, 0, 10);
        drawMesh(building1);
        gl.glPopMatrix();
        //drawMesh(mesh);
        //drawMesh(wheel1);
        //drawMesh(wheel2);
        //drawMesh(wheel3);
        //drawMesh(wheel4); 


        //END MOVE EVERYTHING DOWN
        gl.glPopMatrix();



        gl.glBindTexture(GL.GL_TEXTURE_2D, this.texRender_Color);
        gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, wWidth, wHeight, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, null);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
        gl.glFramebufferTexture2DEXT(GL.GL_DRAW_FRAMEBUFFER_EXT, GL.GL_COLOR_ATTACHMENT0_EXT, GL.GL_TEXTURE_2D, this.texRender_Color, 0);
        //gl.glDrawBuffer(GL.GL_COLOR_ATTACHMENT0_EXT);

        gl.glBindTexture(GL.GL_TEXTURE_2D, this.texRender_Depth);
        gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_DEPTH_COMPONENT24, wWidth, wHeight, 0, GL.GL_DEPTH_COMPONENT, GL.GL_FLOAT, null);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
        gl.glFramebufferTexture2DEXT(GL.GL_DRAW_FRAMEBUFFER_EXT, GL.GL_DEPTH_ATTACHMENT_EXT, GL.GL_TEXTURE_2D, this.texRender_Depth, 0);

        gl.glDisable(GL.GL_DEPTH_TEST);
        gl.glBindFramebufferEXT(GL.GL_FRAMEBUFFER_EXT, 0);
        gl.glClearColor(0.5f, 1.0f, 0.8f, 0.0f);
        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        //gl.glLoadIdentity();

        //gl.glEnable(GL.GL_TEXTURE_2D);

        gl.glUseProgram(postProgramObject);

        //Load diffuse texture
        gl.glUniform1i(samplerPostColor, 0);
        gl.glActiveTexture(GL.GL_TEXTURE0);
        gl.glBindTexture(GL.GL_TEXTURE_2D, texRender_Color);

        //Load spec texture
        gl.glUniform1i(samplerPostDepth, 1);
        gl.glActiveTexture(GL.GL_TEXTURE1);
        gl.glBindTexture(GL.GL_TEXTURE_2D, texRender_Depth);

        gl.glUniform1fARB(uniformWidth, (float) wWidth);
        gl.glUniform1fARB(uniformHeight, (float) wHeight);
	gl.glUniform1fARB(uniformFocalLength, focalLength);


        //Draw full screen quad
        gl.glBegin(GL.GL_QUADS);
        gl.glVertex3f(-1.0f, -1.0f, 0.0f);
        gl.glVertex3f(100.0f, -1.0f, 0.0f);
        gl.glVertex3f(100.0f, 1.0f, 0.0f);
        gl.glVertex3f(-1.0f, 1.0f, 0.0f);
        gl.glEnd();

        //gl.glBindFramebufferEXT(GL.GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT, 0);


        gl.glBindFramebufferEXT(GL.GL_FRAMEBUFFER_EXT, 0);
        gl.glBindRenderbufferEXT(GL.GL_RENDERBUFFER_EXT, 0);
        gl.glUseProgram(0);
    }

    private void camera() {
        gl.glMatrixMode(GL.GL_PROJECTION);
        gl.glLoadIdentity();

        // Perspective.
        float widthHeightRatio = (float) wWidth / (float) wHeight;
        glu.gluPerspective(75, widthHeightRatio, 0.15, 1000);


        if (overviewMode) {
            targetPos.x = (float) (5.0 * Math.sin(phi) * Math.sin(theta));
            targetPos.y = (float) (5.0 * Math.cos(phi));
            targetPos.z = (float) (-5.0 * Math.sin(phi) * Math.cos(theta));
        } else {
            targetPos.x = (float) (5.0 * Math.sin(phi) * Math.cos(physics.radianPlayer));
            targetPos.y = (float) (5.0 * Math.cos(phi));
            targetPos.z = (float) (-5.0 * Math.sin(phi) * Math.sin(physics.radianPlayer));
        }

        Vector3d d = new Vector3d(targetPos.x, targetPos.y, targetPos.z);
        //Normalize directionToMove:
        d.Normalize();

        Vector3d move = new Vector3d();
        if (movement != 0) {
            move = new Vector3d(d.x * movement, d.y * movement, d.z * movement);
            movement = 0;
        }
        move.Normalize();

        if (overviewMode) {
            cameraPos.addVector(move);
        } else {
            cameraPos.x = (float) physics.totalXPlayer;
            cameraPos.y = 1.0f;
            cameraPos.z = (float) physics.totalZPlayer;
        }

        targetPos.addVector(cameraPos);



        glu.gluLookAt(
                cameraPos.x,
                cameraPos.y,
                cameraPos.z,
                targetPos.x,
                targetPos.y,
                targetPos.z,
                0,
                1,
                0);

        // Change back to model view matrix.
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glLoadIdentity();



        /* OLD CODE
         targetPos.x = (float)(5.0*Math.sin(phi)*Math.sin(theta));
         targetPos.y = (float)(5.0*Math.cos(phi));
         targetPos.z = (float)(-5.0*Math.sin(phi)*Math.cos(theta));
		
         Vector3d d = new Vector3d(targetPos.x, targetPos.y, targetPos.z);
         //Normalize directionToMove:
         d.Normalize();

         Vector3d move = new Vector3d();
         if(movement != 0)
         {
         move = new Vector3d(d.x*movement, d.y*movement, d.z*movement);
         movement = 0;
         }
         move.Normalize();
         cameraPos.addVector(move);
		
         targetPos.addVector(cameraPos);
		
         glu.gluLookAt(
         cameraPos.x,
         cameraPos.y,
         cameraPos.z,
         targetPos.x,
         targetPos.y,
         targetPos.z,
         0,
         1,
         0);

         // Change back to model view matrix.
         gl.glMatrixMode(GL.GL_MODELVIEW);
         gl.glLoadIdentity();
         */


    }

    public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
        gl.glMatrixMode(GL.GL_PROJECTION);
        gl.glLoadIdentity();

        wWidth = width;
        wHeight = height;

        // Perspective.
        float widthHeightRatio = (float) width / (float) height;
        glu.gluPerspective(45, widthHeightRatio, 1, 1000);
        glu.gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

        // Change back to model view matrix.
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glLoadIdentity();
    }

    public void displayChanged(GLAutoDrawable drawable, boolean modeChanged, boolean deviceChanged) {
    }
}