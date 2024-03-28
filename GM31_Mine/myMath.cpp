
#include "myMath.h"


/// <summary>
/// ベクトルAとBの各要素の大きい方の値で構成したベクトルを返す関数
/// </summary>
/// <param name="vecA"></param>
/// <param name="vecB"></param>
/// <returns>大きい値で構成したベクトル</returns>
D3DXVECTOR3 MaxVecElem(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
{
    return D3DXVECTOR3(
        vecA.x > vecB.x ? vecA.x : vecB.x,
        vecA.y > vecB.y ? vecA.y : vecB.y,
        vecA.z > vecB.z ? vecA.z : vecB.z
    );

}

/// <summary>
/// 行列(4x4を3x3とした行列)とベクトルの掛け算をする関数
/// </summary>
/// <param name="mat">行列</param>
/// <param name="vec">ベクトル</param>
/// <returns>出力ベクトル</returns>
D3DXVECTOR3 MulMatVec(D3DXMATRIX mat, D3DXVECTOR3 vec)
{
    return D3DXVECTOR3(
        mat._11 * vec.x + mat._12 * vec.y + mat._13 * vec.z,
        mat._21 * vec.x + mat._22 * vec.y + mat._23 * vec.z,
        mat._31 * vec.x + mat._32 * vec.y + mat._33 * vec.z
    );
}


/// <summary>
/// 行列の要素を絶対値にする関数
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
/// ベクトルをクォータニオンで回転させる関数
/// </summary>
/// <param name="quat">クォータニオン</param>
/// <param name="vec">ベクトル</param>
/// <returns>回転したベクトル</returns>
D3DXVECTOR3 RotateVecForQuat(D3DXQUATERNION quat, D3DXVECTOR3 vec)
{
    // v' = q(v,0)q^-1
    // 回転後のベクトル = 姿勢クォータニオン×(変換前のベクトル,0)クォータニオン×姿勢の共役クォータニオン

    D3DXVECTOR3 quatVec = D3DXVECTOR3(quat.x, quat.y, quat.z);
    D3DXQUATERNION invQuat; D3DXQuaternionInverse(&invQuat, &quat);
    D3DXVECTOR3 invQuatVec = D3DXVECTOR3(invQuat.x, invQuat.y, invQuat.z);
    float sA = quat.w;
    float sB = 0.0f;
    float sC = invQuat.w;

    // クォータニオンの積 q0 = (v0,s0) q1 = (v1,s1)
    // q0q1 = (s0v1 + s1v0 + v0×v1, s0s1-v0・v1)

    // 姿勢クォータニオンと(変換前のベクトル,0)クォータニオンの積
    D3DXVECTOR3 crossAB; D3DXVec3Cross(&crossAB, &quatVec, &vec);
    float dotAB = D3DXVec3Dot(&quatVec, &vec);
    D3DXVECTOR3 vec1 = vec * sA + quatVec * sB + crossAB;
    float sAB = sA * sB - dotAB;

    // 上の式の結果と姿勢の共役クォータニオンの積
    D3DXVECTOR3 crossVec; D3DXVec3Cross(&crossVec, &vec1, &invQuatVec);
    D3DXVECTOR3 rotVec = invQuatVec * sAB + vec1 * sC + crossVec;

    return rotVec;

}