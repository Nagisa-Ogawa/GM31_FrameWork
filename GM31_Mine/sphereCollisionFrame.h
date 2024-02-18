#pragma once

#include "gameObject.h"


//-----------------------------------------------------
// 球コリジョン表示用オブジェクト
//------------------------------------------------------
// エディタ上でコリジョンを表示する用のオブジェクトです
//------------------------------------------------------
class SphereCollisionFrame : public GameObject
{

private:
	class Transform* m_collTransform{};
	D3DXVECTOR3 m_Offset{};
public:
	void Init(float radius, D3DXVECTOR3 offset);
	void Update() override;

	void SetCollTransform(Transform* transform) { m_collTransform = transform; }
};