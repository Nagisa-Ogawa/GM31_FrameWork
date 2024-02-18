#pragma once
#include "SerializableClass.h"
#include "component.h"

struct POLYGON_POSITION
{
	D3DXVECTOR3 position[3];
};


//-----------------------------------------------------
// ポリゴンコリジョンコンポーネントクラス
//------------------------------------------------------
// マウスでオブジェクトを指し示す処理に使用しています
// 実際の当たり判定は未実装です
//------------------------------------------------------
class PolygonCollision :public Component
{
private:
	POLYGON_POSITION* m_polyArray = nullptr;	// ポリゴンの各頂点座標を持つ配列
	unsigned int m_arrayCount = 0;				// 配列の要素数
public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void DispInspector() override;

	POLYGON_POSITION* GetPolygonArray() { return m_polyArray; }		// ポリゴンの頂点配列を取得する関数
	unsigned int GetArrayCount() { return m_arrayCount; }

	void SetPolygonArray(POLYGON_POSITION* polyArray) { m_polyArray = polyArray; }		// ポリゴンの頂点配列をセットする関数
	void SetArrayCount(unsigned int arrayCount) { m_arrayCount = arrayCount; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(PolygonCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PolygonCollision);
