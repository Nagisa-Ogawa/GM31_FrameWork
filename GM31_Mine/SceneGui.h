#pragma once
#include "MyImGui.h"

class GameObject;

class SceneGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* gameobject) { m_selectedObject = gameobject; }

	POINT		ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize);	// �ʏ�̉�ʂŎ擾�������W���Q�[����ʂ������_�����O���Ă����ʂł̍��W�ɕϊ�
	GameObject* GetMousePosObject(POINT mousePos);		// �}�E�X���W�ɃI�u�W�F�N�g�����邩�𒲂ׂ��̃I�u�W�F�N�g��Ԃ��֐�

};

