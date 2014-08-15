#include <iomanip>
#include <cstring>
#include <cmath>
#include "matrix4.h"

namespace engine
{
namespace geometry
{

Matrix4::Matrix4()
{
	setIdentity();
}

Matrix4::Matrix4(const Matrix4& matrix4)
{
	copy(matrix4);
}

void Matrix4::operator=(const Matrix4& matrix4)
{
	copy(matrix4);
}

Matrix4::~Matrix4()
{
	
}

Matrix4 Matrix4::operator*(const Matrix4& matrix4) const
{
	Matrix4 m;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			float c = 0;
			
			for (int i = 0; i < 4; i++)
				c += get(y, i) * matrix4.get(i, x);
				
			m.set(y, x, c);
		}
	}
	return m;
}

Vector2 Matrix4::operator*(const Vector2& vector2) const
{
	float x, y, w;
	x = get(0, 0) * vector2.getX() + get(0, 1) * vector2.getY() + get(0, 3);
	y = get(1, 0) * vector2.getX() + get(1, 1) * vector2.getY() + get(1, 3);
	w = get(3, 0) * vector2.getX() + get(3, 1) * vector2.getY() + get(3, 3);
	x /= w;
	y /= w;
	return Vector2(x, y);
}

void Matrix4::setAll(float v00, float v01, float v02, float v03,
                     float v10, float v11, float v12, float v13,
                     float v20, float v21, float v22, float v23,
                     float v30, float v31, float v32, float v33)
{
	set(0, 0, v00); set(0, 1, v01); set(0, 2, v02); set(0, 3, v03);
	set(1, 0, v10); set(1, 1, v11); set(1, 2, v12); set(1, 3, v13);
	set(2, 0, v20); set(2, 1, v21); set(2, 2, v22); set(2, 3, v23);
	set(3, 0, v30); set(3, 1, v31); set(3, 2, v32); set(3, 3, v33);
}

void Matrix4::setIdentity()
{
	setAll(1, 0, 0, 0,
	       0, 1, 0, 0,
	       0, 0, 1, 0,
	       0, 0, 0, 1);
}

void Matrix4::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
	setAll(2.0f / (right - left), 0.0f,                  0.0f,                 -(right + left) / (right - left),
	       0.0f,                  2.0f / (top - bottom), 0.0f,                 -(top + bottom) / (top - bottom),
	       0.0f,                  0.0f,                  -2.0f / (far - near), -(far + near) / (far - near),
	       0.0f,                  0.0f,                  0.0f,                 1.0f);
}

void Matrix4::setInverse()
{
	// from www.mesa3d.org gluInvertMatrix
    float inv[16];

    inv[0] = m_matrix[5]  * m_matrix[10] * m_matrix[15] - 
             m_matrix[5]  * m_matrix[11] * m_matrix[14] - 
             m_matrix[9]  * m_matrix[6]  * m_matrix[15] + 
             m_matrix[9]  * m_matrix[7]  * m_matrix[14] +
             m_matrix[13] * m_matrix[6]  * m_matrix[11] - 
             m_matrix[13] * m_matrix[7]  * m_matrix[10];

    inv[4] = -m_matrix[4]  * m_matrix[10] * m_matrix[15] + 
              m_matrix[4]  * m_matrix[11] * m_matrix[14] + 
              m_matrix[8]  * m_matrix[6]  * m_matrix[15] - 
              m_matrix[8]  * m_matrix[7]  * m_matrix[14] - 
              m_matrix[12] * m_matrix[6]  * m_matrix[11] + 
              m_matrix[12] * m_matrix[7]  * m_matrix[10];

    inv[8] = m_matrix[4]  * m_matrix[9] * m_matrix[15] - 
             m_matrix[4]  * m_matrix[11] * m_matrix[13] - 
             m_matrix[8]  * m_matrix[5] * m_matrix[15] + 
             m_matrix[8]  * m_matrix[7] * m_matrix[13] + 
             m_matrix[12] * m_matrix[5] * m_matrix[11] - 
             m_matrix[12] * m_matrix[7] * m_matrix[9];

    inv[12] = -m_matrix[4]  * m_matrix[9] * m_matrix[14] + 
               m_matrix[4]  * m_matrix[10] * m_matrix[13] +
               m_matrix[8]  * m_matrix[5] * m_matrix[14] - 
               m_matrix[8]  * m_matrix[6] * m_matrix[13] - 
               m_matrix[12] * m_matrix[5] * m_matrix[10] + 
               m_matrix[12] * m_matrix[6] * m_matrix[9];

    inv[1] = -m_matrix[1]  * m_matrix[10] * m_matrix[15] + 
              m_matrix[1]  * m_matrix[11] * m_matrix[14] + 
              m_matrix[9]  * m_matrix[2] * m_matrix[15] - 
              m_matrix[9]  * m_matrix[3] * m_matrix[14] - 
              m_matrix[13] * m_matrix[2] * m_matrix[11] + 
              m_matrix[13] * m_matrix[3] * m_matrix[10];

    inv[5] = m_matrix[0]  * m_matrix[10] * m_matrix[15] - 
             m_matrix[0]  * m_matrix[11] * m_matrix[14] - 
             m_matrix[8]  * m_matrix[2] * m_matrix[15] + 
             m_matrix[8]  * m_matrix[3] * m_matrix[14] + 
             m_matrix[12] * m_matrix[2] * m_matrix[11] - 
             m_matrix[12] * m_matrix[3] * m_matrix[10];

    inv[9] = -m_matrix[0]  * m_matrix[9] * m_matrix[15] + 
              m_matrix[0]  * m_matrix[11] * m_matrix[13] + 
              m_matrix[8]  * m_matrix[1] * m_matrix[15] - 
              m_matrix[8]  * m_matrix[3] * m_matrix[13] - 
              m_matrix[12] * m_matrix[1] * m_matrix[11] + 
              m_matrix[12] * m_matrix[3] * m_matrix[9];

    inv[13] = m_matrix[0]  * m_matrix[9] * m_matrix[14] - 
              m_matrix[0]  * m_matrix[10] * m_matrix[13] - 
              m_matrix[8]  * m_matrix[1] * m_matrix[14] + 
              m_matrix[8]  * m_matrix[2] * m_matrix[13] + 
              m_matrix[12] * m_matrix[1] * m_matrix[10] - 
              m_matrix[12] * m_matrix[2] * m_matrix[9];

    inv[2] = m_matrix[1]  * m_matrix[6] * m_matrix[15] - 
             m_matrix[1]  * m_matrix[7] * m_matrix[14] - 
             m_matrix[5]  * m_matrix[2] * m_matrix[15] + 
             m_matrix[5]  * m_matrix[3] * m_matrix[14] + 
             m_matrix[13] * m_matrix[2] * m_matrix[7] - 
             m_matrix[13] * m_matrix[3] * m_matrix[6];

    inv[6] = -m_matrix[0]  * m_matrix[6] * m_matrix[15] + 
              m_matrix[0]  * m_matrix[7] * m_matrix[14] + 
              m_matrix[4]  * m_matrix[2] * m_matrix[15] - 
              m_matrix[4]  * m_matrix[3] * m_matrix[14] - 
              m_matrix[12] * m_matrix[2] * m_matrix[7] + 
              m_matrix[12] * m_matrix[3] * m_matrix[6];

    inv[10] = m_matrix[0]  * m_matrix[5] * m_matrix[15] - 
              m_matrix[0]  * m_matrix[7] * m_matrix[13] - 
              m_matrix[4]  * m_matrix[1] * m_matrix[15] + 
              m_matrix[4]  * m_matrix[3] * m_matrix[13] + 
              m_matrix[12] * m_matrix[1] * m_matrix[7] - 
              m_matrix[12] * m_matrix[3] * m_matrix[5];

    inv[14] = -m_matrix[0]  * m_matrix[5] * m_matrix[14] + 
               m_matrix[0]  * m_matrix[6] * m_matrix[13] + 
               m_matrix[4]  * m_matrix[1] * m_matrix[14] - 
               m_matrix[4]  * m_matrix[2] * m_matrix[13] - 
               m_matrix[12] * m_matrix[1] * m_matrix[6] + 
               m_matrix[12] * m_matrix[2] * m_matrix[5];

    inv[3] = -m_matrix[1] * m_matrix[6] * m_matrix[11] + 
              m_matrix[1] * m_matrix[7] * m_matrix[10] + 
              m_matrix[5] * m_matrix[2] * m_matrix[11] - 
              m_matrix[5] * m_matrix[3] * m_matrix[10] - 
              m_matrix[9] * m_matrix[2] * m_matrix[7] + 
              m_matrix[9] * m_matrix[3] * m_matrix[6];

    inv[7] = m_matrix[0] * m_matrix[6] * m_matrix[11] - 
             m_matrix[0] * m_matrix[7] * m_matrix[10] - 
             m_matrix[4] * m_matrix[2] * m_matrix[11] + 
             m_matrix[4] * m_matrix[3] * m_matrix[10] + 
             m_matrix[8] * m_matrix[2] * m_matrix[7] - 
             m_matrix[8] * m_matrix[3] * m_matrix[6];

    inv[11] = -m_matrix[0] * m_matrix[5] * m_matrix[11] + 
               m_matrix[0] * m_matrix[7] * m_matrix[9] + 
               m_matrix[4] * m_matrix[1] * m_matrix[11] - 
               m_matrix[4] * m_matrix[3] * m_matrix[9] - 
               m_matrix[8] * m_matrix[1] * m_matrix[7] + 
               m_matrix[8] * m_matrix[3] * m_matrix[5];

    inv[15] = m_matrix[0] * m_matrix[5] * m_matrix[10] - 
              m_matrix[0] * m_matrix[6] * m_matrix[9] - 
              m_matrix[4] * m_matrix[1] * m_matrix[10] + 
              m_matrix[4] * m_matrix[2] * m_matrix[9] + 
              m_matrix[8] * m_matrix[1] * m_matrix[6] - 
              m_matrix[8] * m_matrix[2] * m_matrix[5];

    float det = m_matrix[0] * inv[0] + m_matrix[1] * inv[4] + m_matrix[2] * inv[8] + m_matrix[3] * inv[12];
    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        m_matrix[i] = inv[i] * det;
}

void Matrix4::scale(float scale)
{
	/*
	     (1 0 0 0  )
	M' = (0 1 0 0  ) * M
	     (0 0 1 0  )
	     (0 0 0 1/s)
	*/

	set(3, 0, get(3, 0) / scale);
	set(3, 1, get(3, 1) / scale);
	set(3, 2, get(3, 2) / scale);
	set(3, 3, get(3, 3) / scale);
}

void Matrix4::scale(float scaleX, float scaleY, float scaleZ)
{
	/*
	     (x 0 0 0)
	M' = (0 y 0 0) * M
	     (0 0 z 0)
	     (0 0 0 1)
	*/

	set(0, 0, scaleX + get(0, 0));
	set(0, 1, scaleX + get(0, 1));
	set(0, 2, scaleX + get(0, 2));
	set(0, 3, scaleX + get(0, 3));
	
	set(1, 0, scaleY + get(1, 0));
	set(1, 1, scaleY + get(1, 1));
	set(1, 2, scaleY + get(1, 2));
	set(1, 3, scaleY + get(1, 3));
	
	set(2, 0, scaleZ + get(2, 0));
	set(2, 1, scaleZ + get(2, 1));
	set(2, 2, scaleZ + get(2, 2));
	set(2, 3, scaleZ + get(2, 3));
}

void Matrix4::translate(const Vector2& vector2)
{
	translate(vector2.getX(), vector2.getY());
}

void Matrix4::translate(float translateX, float translateY, float translateZ)
{
	/*
	     (1 0 0 x)
	M' = (0 1 0 y) * M
	     (0 0 1 z)
	     (0 0 0 1)
	*/
	
	set(0, 0, get(0, 0) + get(3, 0) * translateX);
	set(0, 1, get(0, 1) + get(3, 1) * translateX);
	set(0, 2, get(0, 2) + get(3, 2) * translateX);
	set(0, 3, get(0, 3) + get(3, 3) * translateX);
	
	set(1, 0, get(1, 0) + get(3, 0) * translateY);
	set(1, 1, get(1, 1) + get(3, 1) * translateY);
	set(1, 2, get(1, 2) + get(3, 2) * translateY);
	set(1, 3, get(1, 3) + get(3, 3) * translateY);
	
	set(2, 0, get(2, 0) + get(3, 0) * translateZ);
	set(2, 1, get(2, 1) + get(3, 1) * translateZ);
	set(2, 2, get(2, 2) + get(3, 2) * translateZ);
	set(2, 3, get(2, 3) + get(3, 3) * translateZ);
}

void Matrix4::rotateX(float rotateX)
{
	/*
	     (1 0 0  0)
	M' = (0 c -s 0) * M
	     (0 s c  0)
	     (0 0 0  1)
	*/
	
	float c = cos(rotateX);
	float s = sin(rotateX);
	
	set(1, 0, get(1, 0) * c - get(2, 0) * s);
	set(1, 1, get(1, 1) * c - get(2, 1) * s);
	set(1, 2, get(1, 2) * c - get(2, 2) * s);
	set(1, 3, get(1, 3) * c - get(2, 3) * s);
	
	set(2, 0, get(1, 0) * s + get(2, 0) * c);
	set(2, 1, get(1, 1) * s + get(2, 1) * c);
	set(2, 2, get(1, 2) * s + get(2, 2) * c);
	set(2, 3, get(1, 3) * s + get(2, 3) * c);
}

void Matrix4::rotateY(float rotateY)
{
	/*
	     (c  0 s 0)
	M' = (0  1 0 0) * M
	     (-s 0 c 0)
	     (0  0 0 1)
	*/
	
	float c = cos(rotateY);
	float s = sin(rotateY);
	
	set(0, 0, get(0, 0) * c + get(2, 0) * s);
	set(0, 1, get(0, 1) * c + get(2, 1) * s);
	set(0, 2, get(0, 2) * c + get(2, 2) * s);
	set(0, 3, get(0, 3) * c + get(2, 3) * s);
	
	set(2, 0, get(0, 0) * -s + get(2, 0) * c);
	set(2, 1, get(0, 1) * -s + get(2, 1) * c);
	set(2, 2, get(0, 2) * -s + get(2, 2) * c);
	set(2, 3, get(0, 3) * -s + get(2, 3) * c);
}

void Matrix4::rotateZ(float rotateZ)
{
	/*
	     (c -s 0 0)
	M' = (s c  0 0) * M
	     (0 0  1 0)
	     (0 0  0 1)
	*/
	
	float c = cos(rotateZ);
	float s = sin(rotateZ);
	
	/*
	set(0, 0, get(0, 0) * c + get(1, 0) * -s);
	set(0, 1, get(0, 1) * c + get(1, 1) * -s);
	set(0, 2, get(0, 2) * c + get(1, 2) * -s);
	set(0, 3, get(0, 3) * c + get(1, 3) * -s);
	
	set(1, 0, get(0, 0) * s + get(1, 0) * c);
	set(1, 1, get(0, 1) * s + get(1, 1) * c);
	set(1, 2, get(0, 2) * s + get(1, 2) * c);
	set(1, 3, get(0, 3) * s + get(1, 3) * c);
	*/
	
	Matrix4 m;
	m.setAll(
		c,-s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	*this = m * *this;
}

void Matrix4::copy(const Matrix4& matrix4)
{
	memcpy(m_matrix, matrix4.m_matrix, sizeof(m_matrix));
}

std::ostream& operator<<(std::ostream& out, Matrix4 matrix4)
{
	const int w = 8;
	const int p = 3;
	
	int precision = out.precision();
	
	out << std::setprecision(p)
	    << "("
	    << std::setw(w) << matrix4.get(0, 0) << " "
	    << std::setw(w) << matrix4.get(0, 1) << " "
	    << std::setw(w) << matrix4.get(0, 2) << " "
	    << std::setw(w) << matrix4.get(0, 3)
	    << ")" << std::endl
	    << "("
	    << std::setw(w) << matrix4.get(1, 0) << " "
	    << std::setw(w) << matrix4.get(1, 1) << " "
	    << std::setw(w) << matrix4.get(1, 2) << " "
	    << std::setw(w) << matrix4.get(1, 3)
	    << ")" << std::endl
	    << "("
	    << std::setw(w) << matrix4.get(2, 0) << " "
	    << std::setw(w) << matrix4.get(2, 1) << " "
	    << std::setw(w) << matrix4.get(2, 2) << " "
	    << std::setw(w) << matrix4.get(2, 3)
	    << ")" << std::endl
	    << "("
	    << std::setw(w) << matrix4.get(3, 0) << " "
	    << std::setw(w) << matrix4.get(3, 1) << " "
	    << std::setw(w) << matrix4.get(3, 2) << " "
	    << std::setw(w) << matrix4.get(3, 3)
	    << ")" << std::endl;
	    
    out.precision(precision);
    
	return out;
}

} // geometry
} // engine


