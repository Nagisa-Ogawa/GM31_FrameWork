#pragma once
#include <string>
#include <luabind/object.hpp>
#include "component.h"


class Script :public Component
{
private:
	luabind::object m_object;
	std::string m_fileName;
public:
	void Init(std::string fileName);
	void Uninit();
	void Update();
	void Draw();

	void Start();	// シーンの実行時に最初に一度だけ呼ばれる関数
	void CompileLua();	// Luaファイルをコンパイルする関数
};
