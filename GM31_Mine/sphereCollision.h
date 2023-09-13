#pragma once
#include "component.h"


class SphereCollision : public Component
{
private:
	float		m_Radius{};
	D3DXVECTOR3 m_Offset{};
	bool		m_IsShowFrame{};

	class SphereCollisionFrame* m_CollFrame{};

public:
	void Init(float radius, D3DXVECTOR3 offset, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsShowFrame() { return m_IsShowFrame; }
	void SetIsShowFrame(bool flag);
	float GetRadius() { return m_Radius; }
	D3DXVECTOR3 GetOffset() { return m_Offset; }
};
