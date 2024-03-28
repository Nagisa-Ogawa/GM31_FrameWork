#pragma once

#include <d3dx9math.h>

#define TIME_STEP (0.016f)
#define MAX_FLOAT (3.40282e+38F)
#define EPSILON	 (1e-5f)


D3DXVECTOR3 MaxVecElem(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);	// ベクトルAとBの各要素の大きい方の値で構成したベクトルを返す関数
D3DXVECTOR3 MulMatVec(D3DXMATRIX mat, D3DXVECTOR3 vec);	// 行列(4x4を3x3とした行列)とベクトルの掛け算をする関数
D3DXMATRIX AbsMat(D3DXMATRIX mat);	// 行列の要素を絶対値にする関数
D3DXVECTOR3 RotateVecForQuat(D3DXQUATERNION quat, D3DXVECTOR3 vec);	// ベクトルをクォータニオンで回転させる関数
