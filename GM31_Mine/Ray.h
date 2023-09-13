#pragma once
#include "main.h"

class Ray
{
private:
	// 開始点
	D3DXVECTOR3 m_StartPos{};
	// 単位方向ベクトル
	D3DXVECTOR3 m_Vec{};

public:
	Ray(D3DXVECTOR3 startPos, D3DXVECTOR3 vec) :m_StartPos(startPos), m_Vec(vec) {};

	D3DXVECTOR3* GetStartPos() { return &m_StartPos; }
	D3DXVECTOR3* GetVec() { return &m_Vec; }
	D3DXVECTOR3 GetRayPos(float t) { return m_StartPos + m_Vec * t; }

};
