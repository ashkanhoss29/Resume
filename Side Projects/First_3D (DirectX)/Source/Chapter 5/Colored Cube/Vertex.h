#ifndef VERTEX_H
#define VERTEX_H

#include "..\..\Common\d3dUtil.h"

struct Vertex
{
	Vertex() 
		: pos(0.0f,0.0f,0.0f),
		  tangent(0.0f,0.0f,0.0f),
		  normal(0.0f,0.0f,0.0f),
		  texC(0.0f,0.0f)
	{
	}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};
 
#endif // VERTEX_H