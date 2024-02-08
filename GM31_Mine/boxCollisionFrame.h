#pragma once

#include "gameObject.h"

class BoxCollisionFrame : public GameObject
{

private:
	int m_collObjID = -1;	// コリジョンを持つオブジェクトのID
	int m_collCompID = -1;	// コリジョンのID
	class BoxCollision* m_collision = nullptr;
public:
	void Init(BoxCollision* collision);
	void Draw() override;
};