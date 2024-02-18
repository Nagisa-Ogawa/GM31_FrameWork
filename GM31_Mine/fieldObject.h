#pragma once

#include "gameObject.h"


//-----------------------------------------------------
// フィールドオブジェクト
//------------------------------------------------------
class FieldObject : public GameObject
{
public:
	void Init() override;

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

CEREAL_REGISTER_TYPE(FieldObject);