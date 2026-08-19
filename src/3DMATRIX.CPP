// 3dMatrix.cpp
//
// Copyright (c) Nigel Thompson 1996
//
// Implementation for:
// C3dMatrix
//

#include "stdafx.h"
#include "3dplus.h"

#define D2R  0.01745329251994

//////////////////////////////////////////////////////////////////
// C3dMatrix

IMPLEMENT_DYNAMIC(C3dMatrix, C3dObject)

C3dMatrix::C3dMatrix()
{
    m_00=1.0; m_01=0.0;  m_02=0.0; m_03=0.0;
    m_10=0.0; m_11=1.0;  m_12=0.0; m_13=0.0;
    m_20=0.0; m_21=0.0;  m_22=1.0; m_23=0.0;
    m_30=0.0; m_31=0.0;  m_32=0.0; m_33=1.0;
}

C3dMatrix::~C3dMatrix()
{
}

C3dMatrix::C3dMatrix(const C3dMatrix& r)
{
    m_00=r.m_00; m_01=r.m_01; m_02=r.m_02; m_03=r.m_03; 
    m_10=r.m_10; m_11=r.m_11; m_12=r.m_12; m_13=r.m_13; 
    m_20=r.m_20; m_21=r.m_21; m_22=r.m_22; m_23=r.m_23; 
    m_30=r.m_30; m_31=r.m_31; m_32=r.m_32; m_33=r.m_33; 
}

C3dMatrix::C3dMatrix(double v00, double v01, double v02, double v03,
                     double v10, double v11, double v12, double v13,
                     double v20, double v21, double v22, double v23,
                     double v30, double v31, double v32, double v33)
{
    m_00=v00; m_01=v01; m_02=v02; m_03=v03; 
    m_10=v10; m_11=v11; m_12=v12; m_13=v13; 
    m_20=v20; m_21=v21; m_22=v22; m_23=v23; 
    m_30=v30; m_31=v31; m_32=v32; m_33=v33; 
}

C3dMatrix& C3dMatrix::operator = (const C3dMatrix& r)
{
    m_00=r.m_00; m_01=r.m_01; m_02=r.m_02; m_03=r.m_03; 
    m_10=r.m_10; m_11=r.m_11; m_12=r.m_12; m_13=r.m_13; 
    m_20=r.m_20; m_21=r.m_21; m_22=r.m_22; m_23=r.m_23; 
    m_30=r.m_30; m_31=r.m_31; m_32=r.m_32; m_33=r.m_33; 
    return *this;
}

C3dMatrix operator + (const C3dMatrix& a, const C3dMatrix& b)
{
    return C3dMatrix(a.m_00 + b.m_00,
                     a.m_01 + b.m_01,
                     a.m_02 + b.m_02,
                     a.m_03 + b.m_03,
                     a.m_10 + b.m_10,
                     a.m_11 + b.m_11,
                     a.m_12 + b.m_12,
                     a.m_13 + b.m_13,
                     a.m_20 + b.m_20,
                     a.m_21 + b.m_21,
                     a.m_22 + b.m_22,
                     a.m_23 + b.m_23,
                     a.m_30 + b.m_30,
                     a.m_31 + b.m_31,
                     a.m_32 + b.m_32,
                     a.m_33 + b.m_33);
}

C3dMatrix& C3dMatrix::operator += (const C3dMatrix& r)
{
    m_00+=r.m_00; m_01+=r.m_01; m_02+=r.m_02; m_03+=r.m_03;
    m_10+=r.m_10; m_11+=r.m_11; m_12+=r.m_12; m_13+=r.m_13;
    m_20+=r.m_20; m_21+=r.m_21; m_22+=r.m_22; m_23+=r.m_23;
    m_30+=r.m_30; m_31+=r.m_31; m_32+=r.m_32; m_33+=r.m_33;
    return *this;
}

// friend
C3dMatrix operator * (const C3dMatrix& a, const C3dMatrix& b)
{
    return C3dMatrix(a.m_00*b.m_00 + a.m_01*b.m_10 + a.m_02*b.m_20 + a.m_03*b.m_30,
                     a.m_00*b.m_01 + a.m_01*b.m_11 + a.m_02*b.m_21 + a.m_03*b.m_31,
                     a.m_00*b.m_02 + a.m_01*b.m_12 + a.m_02*b.m_22 + a.m_03*b.m_32,
                     a.m_00*b.m_03 + a.m_01*b.m_13 + a.m_02*b.m_23 + a.m_03*b.m_33,
                     a.m_10*b.m_00 + a.m_11*b.m_10 + a.m_12*b.m_20 + a.m_13*b.m_30,
                     a.m_10*b.m_01 + a.m_11*b.m_11 + a.m_12*b.m_21 + a.m_13*b.m_31,
                     a.m_10*b.m_02 + a.m_11*b.m_12 + a.m_12*b.m_22 + a.m_13*b.m_32,
                     a.m_10*b.m_03 + a.m_11*b.m_13 + a.m_12*b.m_23 + a.m_13*b.m_33,
                     a.m_20*b.m_00 + a.m_21*b.m_10 + a.m_22*b.m_20 + a.m_23*b.m_30,
                     a.m_20*b.m_01 + a.m_21*b.m_11 + a.m_22*b.m_21 + a.m_23*b.m_31,
                     a.m_20*b.m_02 + a.m_21*b.m_12 + a.m_22*b.m_22 + a.m_23*b.m_32,
                     a.m_20*b.m_03 + a.m_21*b.m_13 + a.m_22*b.m_23 + a.m_23*b.m_33,
                     a.m_30*b.m_00 + a.m_31*b.m_10 + a.m_32*b.m_20 + a.m_33*b.m_30,
                     a.m_30*b.m_01 + a.m_31*b.m_11 + a.m_32*b.m_21 + a.m_33*b.m_31,
                     a.m_30*b.m_02 + a.m_31*b.m_12 + a.m_32*b.m_22 + a.m_33*b.m_32,
                     a.m_30*b.m_03 + a.m_31*b.m_13 + a.m_32*b.m_23 + a.m_33*b.m_33);
}

// friend
C3dVector operator * (const C3dMatrix& m, const D3DVECTOR& v)
{
	C3dVector r;
	r.x = m.m_00 * v.x + m.m_01 * v.y + m.m_02 * v.z;
	r.y = m.m_10 * v.x + m.m_11 * v.y + m.m_12 * v.z;
	r.z = m.m_20 * v.x + m.m_21 * v.y + m.m_22 * v.z;
	return r;
}

C3dMatrix& C3dMatrix::operator *= (const C3dMatrix& r)
{
    // deliberately create a temp to hold result
    *this = *this * r;
    return *this;
}
                      
// rotate n degrees about each axis
void C3dMatrix::Rotate(double rx, double ry, double rz)
{
    double sinx = sin(rx * D2R);
	double cosx = cos(rx * D2R);
    double siny = sin(ry * D2R);
	double cosy = cos(ry * D2R);
    double sinz = sin(rz * D2R);
	double cosz = cos(rz * D2R);

	C3dMatrix mx(1, 0,     0,    0,
                 0,  cosx, sinx, 0,
                 0, -sinx, cosx, 0,
                 0, 0,     0,    1);

    C3dMatrix my(cosy, 0, -siny, 0,
                 0,    1, 0,     0,
                 siny, 0,  cosy, 0,
                 0,    0, 0,     1);

    C3dMatrix mz( cosz, sinz, 0, 0,
                 -sinz, cosz, 0, 0,
                 0,     0,    1, 0,
                 0,     0,    0, 1);

    *this *= mx * my * mz;
}

void C3dMatrix::Translate(double dx, double dy, double dz)
{
    C3dMatrix tx( 1, 0,  0, 0,
                  0, 1,  0, 0,
                  0, 0,  1, 0,
                 dx, dy, dz,1);

    *this *= tx;
}

void C3dMatrix::Scale(double sx, double sy, double sz)
{
    C3dMatrix tx(sx,  0,  0, 0,
                  0, sy,  0, 0,
                  0,  0, sz, 0,
                  0,  0,  0, 1);

    *this *= tx;
}

void C3dMatrix::MakeUnit()
{
    m_00=1.0; m_01=0.0;  m_02=0.0; m_03=0.0;
    m_10=0.0; m_11=1.0;  m_12=0.0; m_13=0.0;
    m_20=0.0; m_21=0.0;  m_22=1.0; m_23=0.0;
    m_30=0.0; m_31=0.0;  m_32=0.0; m_33=1.0;
}

void C3dMatrix::Initialize(D3DRMMATRIX4D& rlm)
{
	rlm[0][0] = D3DVAL(m_00);
	rlm[0][1] = D3DVAL(m_01);
	rlm[0][2] = D3DVAL(m_02);
	rlm[0][3] = D3DVAL(m_03);
	rlm[1][0] = D3DVAL(m_10);
	rlm[1][1] = D3DVAL(m_11);
	rlm[1][2] = D3DVAL(m_12);
	rlm[1][3] = D3DVAL(m_13);
	rlm[2][0] = D3DVAL(m_20);
	rlm[2][1] = D3DVAL(m_21);
	rlm[2][2] = D3DVAL(m_22);
	rlm[2][3] = D3DVAL(m_23);
	rlm[3][0] = D3DVAL(m_30);
	rlm[3][1] = D3DVAL(m_31);
	rlm[3][2] = D3DVAL(m_32);
	rlm[3][3] = D3DVAL(m_33);
}
