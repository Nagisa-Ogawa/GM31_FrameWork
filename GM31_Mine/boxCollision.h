#pragma once
#include "component.h"


class BoxCollision :public Component
{
private:
	D3DXVECTOR3 m_Size{};
	D3DXVECTOR3 m_Offset{};
	bool		m_IsTrigger{};
	bool		m_IsShowFrame{};

	class BoxCollisionFrame* m_CollFrame{};

public:
	void Init(D3DXVECTOR3 size,D3DXVECTOR3 offset, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsTrigger() { return m_IsTrigger; }
	bool GetIsShowFrame() { return m_IsShowFrame; }
	void SetIsShowFrame(bool flag);
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetOffset() { return m_Offset; }
};
