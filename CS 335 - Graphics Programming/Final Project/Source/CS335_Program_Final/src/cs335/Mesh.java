/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cs335;

import com.obj.Group;
import com.obj.TextureCoordinate;
import com.obj.Vertex;
import com.obj.WavefrontObject;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;

/**
 *
 * @author Bob
 */
public class Mesh
{
	WavefrontObject object = null;
	int diffuse = -1;
	int specular = -1;
	int normal = -1;
	
	
	boolean usingLoader = true;
	
	ArrayList<Vertex> vertices = new ArrayList();
	ArrayList<Vertex> normals = new ArrayList();
	ArrayList<TextureCoordinate> textureCoords = new ArrayList();
	ArrayList<Integer> vIndices = new ArrayList();
	ArrayList<Integer> tIndices = new ArrayList();
	ArrayList<Integer> nIndices = new ArrayList();
	
	
	public Mesh(String file, int diffuseID, int specularID, int normalID, boolean useLoader)
	{
		usingLoader = useLoader;
		
		normal = normalID;
		specular = specularID;
		diffuse = diffuseID;
		
		if(useLoader)
		{
		object = new WavefrontObject(file);
		}
		else
		{
			try
			{
				File f = new File(file);
				BufferedReader reader = new BufferedReader(new FileReader(file));
				
				String line = "";
				while((line = reader.readLine()) != null)
				{
					String strs[] = line.split("\\s+");
					if(strs[0].equals("v"))
					{
						Vertex v = new Vertex();
						v.setX(Float.parseFloat(strs[1]));
						v.setY(Float.parseFloat(strs[2]));
						v.setZ(Float.parseFloat(strs[3]));
						vertices.add(v);
					}
					else if(strs[0].equals("vt"))
					{
						TextureCoordinate t = new TextureCoordinate();
						t.setU(Float.parseFloat(strs[1]));
						t.setV(-Float.parseFloat(strs[2]));
						textureCoords.add(t);
					}
					else if(strs[0].equals("vn"))
					{
						Vertex n = new Vertex();
						n.setX(Float.parseFloat(strs[1]));
						n.setY(Float.parseFloat(strs[2]));
						n.setZ(Float.parseFloat(strs[3]));
						normals.add(n);
					}
					else if(strs[0].equals("f"))
					{
						String vFaces[] = strs[1].split("/");
						vIndices.add(Integer.parseInt(vFaces[0])-1);
						tIndices.add(Integer.parseInt(vFaces[1])-1);
						nIndices.add(Integer.parseInt(vFaces[2])-1);
						//System.out.println(vFaces[0] + " " + vFaces[1] + " " + vFaces[2]);
						
						String tFaces[] = strs[2].split("/");
						vIndices.add(Integer.parseInt(tFaces[0])-1);
						tIndices.add(Integer.parseInt(tFaces[1])-1);
						nIndices.add(Integer.parseInt(tFaces[2])-1);
						//System.out.println(tFaces[0] + " " + tFaces[1] + " " + tFaces[2]);
						
						String nFaces[] = strs[3].split("/");
						vIndices.add(Integer.parseInt(nFaces[0])-1);
						tIndices.add(Integer.parseInt(nFaces[1])-1);
						nIndices.add(Integer.parseInt(nFaces[2])-1);
						//System.out.println(nFaces[0] + " " + nFaces[1] + " " + nFaces[2]);
					}
				}
				
				//System.out.println(" " + vIndices.size() + " " + tIndices.size() + " " + nIndices.size());
//				System.out.println(" " + vertices.size() + " " + textureCoords.size() + " " + normals.size());
			}
			catch(Exception ex)
			{
				System.out.println("Error in loading obj OOOOH NOOOOO");
			}
		}
		
		/*
		 * ArrayList<Group> groups = object.getGroups(); for(int i = 0; i <
		 * groups.size(); i++) {
		 *
		 * }
		 *
		 */


	}
}
