#pragma once

#include <list>

struct MODEL;
class  Enemy;

class EnemyFactory : public Factory<EnemyFactory, Enemy>
{
	friend class Factory<EnemyFactory, Enemy>;
private:
	MODEL* m_pModel = nullptr;

	EnemyFactory();
	~EnemyFactory();

	void Init();
public:
	Enemy* ActiveObject() override;
	void HideObject(Enemy* enemy) override;
	void Uninit() override;
};


