#pragma once

#include "gameObject.h"


//-----------------------------------------------------
// エディタ用カメラオブジェクト
//------------------------------------------------------
// エディタを映す用のカメラオブジェクトです
//------------------------------------------------------
class EditorCameraObject :public GameObject
{
private:
	bool	m_isRButton = false;	// 右クリックを押しているかのフラグ
	POINT	m_startMousePos{};		// 右クリックを押したときのマウス座標
public:
	void Init() override;
	void Update() override;

	void RotateCamera(D3DXVECTOR2 delta);	// カメラをカメラ自身を中心として回転させる関数

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(EditorCameraObject);

