#pragma once
#include "MyImGui.h"

class GameObject;

class SceneGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* gameobject) { m_selectedObject = gameobject; }

	POINT		ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize);	// 通常の画面で取得した座標をゲーム画面をレンダリングしている画面での座標に変換
	GameObject* GetMousePosObject(POINT mousePos);		// マウス座標にオブジェクトがあるかを調べそのオブジェクトを返す関数

};

