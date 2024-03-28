
#include "myMath.h"


/// <summary>
/// �x�N�g��A��B�̊e�v�f�̑傫�����̒l�ō\�������x�N�g����Ԃ��֐�
/// </summary>
/// <param name="vecA"></param>
/// <param name="vecB"></param>
/// <returns>�傫���l�ō\�������x�N�g��</returns>
D3DXVECTOR3 MaxVecElem(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
{
    return D3DXVECTOR3(
        vecA.x > vecB.x ? vecA.x : vecB.x,
        vecA.y > vecB.y ? vecA.y : vecB.y,
        vecA.z > vecB.z ? vecA.z : vecB.z
    );

}

/// <summary>
/// �s��(4x4��3x3�Ƃ����s��)�ƃx�N�g���̊|���Z������֐�
/// </summary>
/// <param name="mat">�s��</param>
/// <param name="vec">�x�N�g��</param>
/// <returns>�o�̓x�N�g��</returns>
D3DXVECTOR3 MulMatVec(D3DXMATRIX mat, D3DXVECTOR3 vec)
{
    return D3DXVECTOR3(
        mat._11 * vec.x + mat._12 * vec.y + mat._13 * vec.z,
        mat._21 * vec.x + mat._22 * vec.y + mat._23 * vec.z,
        mat._31 * vec.x + mat._32 * vec.y + mat._33 * vec.z
    );
}


/// <summary>
/// �s��̗v�f���Βl�ɂ���֐�
/// </summary>
/// <param name="mat"></param>
/// <returns></returns>
D3DXMATRIX AbsMat(D3DXMATRIX mat)
{
    return D3DXMATRIX(
        fabsf(mat._11), fabsf(mat._12), fabsf(mat._13), fabsf(mat._14),
        fabsf(mat._21), fabsf(mat._22), fabsf(mat._23), fabsf(mat._24),
        fabsf(mat._31), fabsf(mat._32), fabsf(mat._33), fabsf(mat._34),
        fabsf(mat._41), fabsf(mat._42), fabsf(mat._43), fabsf(mat._44)
    );
}


/// <summary>
/// �x�N�g�����N�H�[�^�j�I���ŉ�]������֐�
/// </summary>
/// <param name="quat">�N�H�[�^�j�I��</param>
/// <param name="vec">�x�N�g��</param>
/// <returns>��]�����x�N�g��</returns>
D3DXVECTOR3 RotateVecForQuat(D3DXQUATERNION quat, D3DXVECTOR3 vec)
{
    // v' = q(v,0)q^-1
    // ��]��̃x�N�g�� = �p���N�H�[�^�j�I���~(�ϊ��O�̃x�N�g��,0)�N�H�[�^�j�I���~�p���̋����N�H�[�^�j�I��

    D3DXVECTOR3 quatVec = D3DXVECTOR3(quat.x, quat.y, quat.z);
    D3DXQUATERNION invQuat; D3DXQuaternionInverse(&invQuat, &quat);
    D3DXVECTOR3 invQuatVec = D3DXVECTOR3(invQuat.x, invQuat.y, invQuat.z);
    float sA = quat.w;
    float sB = 0.0f;
    float sC = invQuat.w;

    // �N�H�[�^�j�I���̐� q0 = (v0,s0) q1 = (v1,s1)
    // q0q1 = (s0v1 + s1v0 + v0�~v1, s0s1-v0�Ev1)

    // �p���N�H�[�^�j�I����(�ϊ��O�̃x�N�g��,0)�N�H�[�^�j�I���̐�
    D3DXVECTOR3 crossAB; D3DXVec3Cross(&crossAB, &quatVec, &vec);
    float dotAB = D3DXVec3Dot(&quatVec, &vec);
    D3DXVECTOR3 vec1 = vec * sA + quatVec * sB + crossAB;
    float sAB = sA * sB - dotAB;

    // ��̎��̌��ʂƎp���̋����N�H�[�^�j�I���̐�
    D3DXVECTOR3 crossVec; D3DXVec3Cross(&crossVec, &vec1, &invQuatVec);
    D3DXVECTOR3 rotVec = invQuatVec * sAB + vec1 * sC + crossVec;

    return rotVec;

}