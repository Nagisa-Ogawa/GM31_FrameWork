#pragma once

#include <d3dx9math.h>

#define TIME_STEP (0.016f)
#define MAX_FLOAT (3.40282e+38F)
#define EPSILON	 (1e-5f)


D3DXVECTOR3 MaxVecElem(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);	// �x�N�g��A��B�̊e�v�f�̑傫�����̒l�ō\�������x�N�g����Ԃ��֐�
D3DXVECTOR3 MulMatVec(D3DXMATRIX mat, D3DXVECTOR3 vec);	// �s��(4x4��3x3�Ƃ����s��)�ƃx�N�g���̊|���Z������֐�
D3DXMATRIX AbsMat(D3DXMATRIX mat);	// �s��̗v�f���Βl�ɂ���֐�
D3DXVECTOR3 RotateVecForQuat(D3DXQUATERNION quat, D3DXVECTOR3 vec);	// �x�N�g�����N�H�[�^�j�I���ŉ�]������֐�
