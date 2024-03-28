#pragma once
#include "SerializableClass.h"
#include "component.h"

class CollisionMesh;

//-----------------------------------------------------
// ボックスコリジョンコンポーネントクラス
//------------------------------------------------------
// 機能は実装完了していますが、
// 実際の当たり判定は未実装です。
//------------------------------------------------------
class BoxCollision :public Component
{
private:
	// ブロードフェーズ用AABB変数
	D3DXVECTOR3 m_BFCenter{};
	D3DXVECTOR3 m_BFHalf{};
	// ナローフェーズ用OBB変数
	CollisionMesh* m_collisionMesh = nullptr;	// 当たり判定に使うメッシュデータ
	D3DXVECTOR3 m_size{};
	D3DXVECTOR3 m_offset{};
	bool		m_isTrigger{};
	class BoxCollisionFrame* m_collFrame = nullptr;	// エディタ上に表示するためのオブジェクト
	int m_collID = -1;							// 表示用オブジェクトのID

	D3DXMATRIX m_worldMatrix{};

public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	D3DXVECTOR3 GetBFCenter() { return m_BFCenter; }
	D3DXVECTOR3 GetBFHalf() { return m_BFHalf; }
	CollisionMesh* GetCollisionMesh() { return m_collisionMesh; }
	D3DXVECTOR3 GetSize() { return m_size; }
	D3DXVECTOR3 GetOffset() { return m_offset; }
	bool* GetIsTrigger() { return &m_isTrigger; }

	void SetBFCenter(D3DXVECTOR3 center) { m_BFCenter = center; }
	void SetBFHalf(D3DXVECTOR3 half) { m_BFHalf = half; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetOffset(D3DXVECTOR3 offset) { m_offset = offset; }
	void SetIsTrigger(bool flag) { m_isTrigger = flag; }
	D3DXMATRIX* GetWorldMatrix();

	D3DXMATRIX CalcInertiaBox(float mass);	// 直方体の慣性テンソルを算出する関数

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector3 size = m_size;
		Vector3 offset = m_offset;
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(size),
			CEREAL_NVP(offset),
			CEREAL_NVP(m_isTrigger),
			CEREAL_NVP(m_collID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector3 size, offset;
		archive(
			cereal::base_class<Component>(this),
			size,
			offset,
			m_isTrigger,
			m_collID
		);
		m_size = D3DXVECTOR3(size.x, size.y, size.z);
		m_offset = D3DXVECTOR3(offset.x, offset.y, offset.z);
	}

};

CEREAL_REGISTER_TYPE(BoxCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BoxCollision);
