#pragma once

#include "gameObject.h"
#include "model.h"

class Sky :public GameObject
{

private:

public:
	void Init() override;
	void Update() override;

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

CEREAL_REGISTER_TYPE(Sky);
