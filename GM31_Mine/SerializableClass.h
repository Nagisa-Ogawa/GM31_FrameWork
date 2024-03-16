#pragma once
#include <d3dx9math.h>
#include "cereal/cereal.hpp"

//-----------------------------------------------------
// �V���A���C�Y�p�ϊ��N���X
//------------------------------------------------------
class Vector2
{
public:
	float x;
	float y;

	Vector2() {
		x = 0.0f;
		y = 0.0f;
	}

	// DirectX��Vector2�^�ł̈����t���R���X�g���N�^
	Vector2(const D3DXVECTOR2& v) {
		x = v.x;
		y = v.y;
	}

	// DirectX��Vector2�^����̉��Z�q�I�[�o�[���[�h
	Vector2& operator=(const D3DXVECTOR2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(x),
			CEREAL_NVP(y)
		);
	}
};


class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	// DirectX��Vector3�^�ł̈����t���R���X�g���N�^
	Vector3(const D3DXVECTOR3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	// DirectX��Vector3�^����̉��Z�q�I�[�o�[���[�h
	Vector3& operator=(const D3DXVECTOR3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(x),
			CEREAL_NVP(y),
			CEREAL_NVP(z)
		);
	}
};

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	// DirectX��Vector4�^�ł̈����t���R���X�g���N�^
	Vector4(const D3DXVECTOR4& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	// DirectX��Quaternion�^�ł̈����t���R���X�g���N�^
	Vector4(const D3DXQUATERNION& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	// DirectX��Vector4�^����̉��Z�q�I�[�o�[���[�h
	Vector4& operator=(const D3DXVECTOR4& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	// DirectX��Quaternion�^����̉��Z�q�I�[�o�[���[�h
	Vector4& operator=(const D3DXQUATERNION& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(x),
			CEREAL_NVP(y),
			CEREAL_NVP(z),
			CEREAL_NVP(w)
		);
	}
};


