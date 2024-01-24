#pragma once
#include <d3dx9math.h>
#include "cereal/cereal.hpp"

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

