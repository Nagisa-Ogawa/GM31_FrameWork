#pragma once

#include "gameObject.h"

class BoxCollisionFrame : public GameObject
{

private:
	int m_collObjID = -1;	// �R���W���������I�u�W�F�N�g��ID
	int m_collCompID = -1;	// �R���W������ID
	class BoxCollision* m_collision = nullptr;
public:
	void Init(BoxCollision* collision);
	void Draw() override;
};