#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3d
{
public:
	double x, y, z;

	Vector3d();
	Vector3d(double x, double y, double z);
	void normalize();
	void addVector(Vector3d &v);
	static double Dot(Vector3d &v1, Vector3d &v2);
	static Vector3d Cross(Vector3d &v1, Vector3d &v2);

	Vector3d operator+(const Vector3d &v) const {
		return Vector3d(x + v.x, y + v.y, z + v.z);
	}
	Vector3d operator-(const Vector3d &v) const {
		return Vector3d(x - v.x, y - v.y, z - v.z);
	}
	Vector3d operator*(const Vector3d &v) const {
		return Vector3d(x * v.x, y * v.y, z * v.z);
	}
	Vector3d operator*(const double &v) const {
		return Vector3d(x*v, y*v, z*v);
	}

};

#endif