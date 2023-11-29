#pragma once

#include "factory.h"

class  Tree;

class TreeFactory : public Factory<TreeFactory, Tree>
{
	friend class Factory<TreeFactory, Tree>;
private:
	ID3D11ShaderResourceView* m_Texture = nullptr;
	TreeFactory();
	~TreeFactory();

	void Init();
public:
	Tree* ActiveObject(std::string name) override;
	void HideObject(Tree* bullet) override;
	void Uninit() override;
	void Clear() override;
};


