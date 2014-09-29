
package cs335;

public class Vector3d
{
	public float x, y, z;
	
	public Vector3d()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	
	public Vector3d(float x, float y, float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public void Normalize()
	{
		float denom = (float)Math.sqrt(x*x + y*y + z*z);
		if(denom != 0)
		{
			x = x / denom;
			y = y / denom;
			z = z / denom;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
		}
	}
	
	public void addVector(Vector3d v)
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
	}
}
