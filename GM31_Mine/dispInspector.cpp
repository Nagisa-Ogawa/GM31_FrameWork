#include "manager.h"
#include "scene.h"
#include "MyImGuiManager.h"
#include "hierarchyGui.h"
#include "renderer.h"
#include "gameObject.h"
#include "fileDialog.h"
#include "dispInspector.h"
#include "boxCollision.h"
#include "camera.h"
#include "quadCollision.h"
#include "script.h"
#include "shader.h"
#include "sphereCollision.h"
#include "sprite.h"
#include "transform.h"
#include "model.h"

//----------------------------------------------
//
// �e��R���|�[�l���g��ImGui�ŕ\������֐�
//
//----------------------------------------------

void DispComponent(BoxCollision* collision)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	std::string name = "BoxCollision##" + std::to_string(collision->GetID());
	bool treeopen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// �R���|�[�l���g�폜�{�^�����쐬
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// �{�^���̐F��ԐF�ɂ���
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("X", ImVec2(20, 20))) {
		collision->SetDestroy();
		style = ImGuiStyle();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		return;
	}
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Component");
	// �R���|�[�l���g�̗v�f��\��
	if (treeopen)
	{
		if (ImGui::TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR3 sizeVec = collision->GetSize();
			float size[3] = { sizeVec.x,sizeVec.y ,sizeVec.z };
			if (ImGui::InputFloat3("", size)) {
				collision->SetSize(D3DXVECTOR3(size[0], size[1], size[2]));
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Offset", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR3 offsetVec = collision->GetOffset();
			float offset[3] = { offsetVec.x,offsetVec.y ,offsetVec.z };
			if (ImGui::InputFloat3("", offset)) {
				collision->SetOffset(D3DXVECTOR3(offset[0], offset[1], offset[2]));
			}
			ImGui::TreePop();
		}
		bool* isTrigger = collision->GetIsTrigger();
		ImGui::Checkbox("IsTrigger", isTrigger);
		ImGui::TreePop();
	}
	ImGui::Separator();
	style = ImGuiStyle();
	ImGui::PopStyleVar();
}

void DispComponent(Camera* camera)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(Model* model)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
		char str[256];
		strcpy_s(str, model->GetFileName().c_str());
		ImGui::PushItemWidth(-60);
		ImGui::InputText("##FileName", str,IM_ARRAYSIZE(str),ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		// ���f���ύX�p�{�^��
		ImGui::SameLine(ImGui::GetWindowWidth() - 60);
		if (ImGui::Button("Set")) {
			std::string filePath;
			if (OpneFileDialog(GetWindow(), "�t�@�C����I�����Ă�������", "Assets\\Models", "�t�@�C��(.obj)\0*.obj\0", &filePath)) {
				// ���f���ύX
				model->ChangeModel(filePath);
			}
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(QuadCollision* collision)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	std::string name = "QuadCollision##" + std::to_string(collision->GetID());
	bool treeopen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// �R���|�[�l���g�폜�{�^�����쐬
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// �{�^���̐F��ԐF�ɂ���
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("X", ImVec2(20, 20))) {
		collision->SetDestroy();
		style = ImGuiStyle();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		return;
	}
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Component");
	// �R���|�[�l���g�̗v�f��\��
	if (treeopen){
		if (ImGui::TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR2* sizeVec = collision->GetSize();
			float size[2] = { sizeVec->x,sizeVec->y };
			if (ImGui::InputFloat2("", size)) {
				collision->SetSize(D3DXVECTOR2(size[0], size[1]));
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Normal", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR3* normalVec = collision->GetNormal();
			float normal[3] = { normalVec->x,normalVec->y ,normalVec->z };
			if (ImGui::InputFloat3("", normal)) {
				collision->SetNormal(D3DXVECTOR3(normal[0], normal[1], normal[2]));
			}
			ImGui::TreePop();
		}
		bool* isTrigger = collision->GetIsTrigger();
		ImGui::Checkbox("IsTrigger", isTrigger);
		ImGui::TreePop();
	}
	ImGui::Separator();
	style = ImGuiStyle();
	ImGui::PopStyleVar();
}

void DispComponent(Script* script)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	std::string name = "Script##" + std::to_string(script->GetID());
	bool treeopen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// �R���|�[�l���g�폜�{�^�����쐬
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// �{�^���̐F��ԐF�ɂ���
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("X", ImVec2(20, 20))) {
		script->SetDestroy();
		style = ImGuiStyle();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		return;
	}
	style.Colors[ImGuiCol_Button] = ImGuiStyle().Colors[ImGuiCol_Button];
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Component");
	// �R���|�[�l���g�̗v�f��\��
	if (treeopen) {
		char str[256];
		strcpy_s(str, script->GetFileName().c_str());
		ImGui::PushItemWidth(-60);
		ImGui::InputText("##ScriptFileName", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		// �X�N���v�g�t�@�C���ύX�p�{�^��
		ImGui::SameLine(ImGui::GetWindowWidth() - 60);
		if (ImGui::Button("Set")) {
			std::string filePath;
			if (OpneFileDialog(GetWindow(), "�t�@�C����I�����Ă�������", "Assets\\Scripts", "�t�@�C��(.lua)\0*.lua\0", &filePath)) {
				// �X�N���v�g�t�@�C���ύX
				script->ChangeScriptFile(filePath);
			}
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(Shader* shader)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::TreeNodeEx("VertexShader", ImGuiTreeNodeFlags_DefaultOpen)) {
			char str[256];
			strcpy_s(str, shader->GetVSFile().c_str());
			ImGui::PushItemWidth(-60);
			ImGui::InputText("##VSFileName", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			// VSShader�ύX�p�{�^��
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			if (ImGui::Button("Set")) {
				std::string filePath;
				if (OpneFileDialog(GetWindow(), "�t�@�C����I�����Ă�������", "Assets\\Shaders", "�t�@�C��(VS.cso)\0*VS.cso\0", &filePath)) {
					// �V�F�[�_�[�ύX
					shader->ChangeVertexShaer(filePath);
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("PixelShader", ImGuiTreeNodeFlags_DefaultOpen)) {
			char str[256];
			strcpy_s(str, shader->GetPSFile().c_str());
			ImGui::PushItemWidth(-60);
			ImGui::InputText("##PSFileName", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			// PSShader�ύX�p�{�^��
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			if (ImGui::Button("Set")) {
				std::string filePath;
				if (OpneFileDialog(GetWindow(), "�t�@�C����I�����Ă�������", "Assets\\Shaders", "�t�@�C��(.PScso)\0*PS.cso\0", &filePath)) {
					// �V�F�[�_�[�ύX
					shader->ChangePixelShaer(filePath);
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(SphereCollision* collision)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	std::string name = "SphereCollision##" + std::to_string(collision->GetID());
	bool treeopen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// �R���|�[�l���g�폜�{�^�����쐬
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// �{�^���̐F��ԐF�ɂ���
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
	if (ImGui::Button("X", ImVec2(20, 20))) {
		collision->SetDestroy();
		style = ImGuiStyle();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		return;
	}
	style = ImGuiStyle();
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Component");
	// �R���|�[�l���g�̗v�f��\��
	if (treeopen) {
		float* radius = collision->GetRadius();
		ImGui::InputFloat("Radius : ", radius);
		if (ImGui::TreeNodeEx("Offset", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR3* offsetVec = collision->GetOffset();
			float offset[3] = { offsetVec->x,offsetVec->y ,offsetVec->z };
			if (ImGui::InputFloat3("", offset)) {
				collision->SetOffset(D3DXVECTOR3(offset[0], offset[1], offset[2]));
			}
			ImGui::TreePop();
		}
		bool* isTrigger = collision->GetIsTrigger();
		ImGui::Checkbox("IsTrigger", isTrigger);
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(Sprite* sprite)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::TreeNodeEx("Position", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR2* posVec = sprite->GetPosition();
			float pos[2] = { posVec->x,posVec->y };
			if (ImGui::InputFloat2("", pos)) {
				sprite->SetPosition(D3DXVECTOR2(pos[0], pos[1]));
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen)) {
			D3DXVECTOR2* sizeVec = sprite->GetSize();
			float size[2] = { sizeVec->x,sizeVec->y };
			if (ImGui::InputFloat2("", size)) {
				sprite->SetSize(D3DXVECTOR2(size[0], size[1]));
			}
			ImGui::TreePop();
		}
		ImGui::Text("TextureName : %s", sprite->GetTextureName().c_str());
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DispComponent(Transform* transform)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		// �ʒu�A��]�A�X�P�[����\��
		if (ImGui::TreeNodeEx("Local",ImGuiTreeNodeFlags_DefaultOpen))	{
			// LoacalPosition�ϐ��̕\��
			if (ImGui::TreeNodeEx("Position",ImGuiTreeNodeFlags_DefaultOpen)) {
				float pos[3] = { transform->m_localPosition.x,transform->m_localPosition.y ,transform->m_localPosition.z };
				if (ImGui::InputFloat3("", pos)) {
					transform->m_localPosition = D3DXVECTOR3(pos[0], pos[1], pos[2]);
				}
				ImGui::TreePop();
			}
			// LocalRotation�ϐ��̕\��
			if (ImGui::TreeNodeEx("Rotation",ImGuiTreeNodeFlags_DefaultOpen)) {
				D3DXVECTOR3 rotVec = transform->GetLocalRotationAsDegree();
				float rot[3] = { rotVec.x,rotVec.y ,rotVec.z};
				if (ImGui::InputFloat3("", rot)) {
					transform->SetLocalRotationFromDegree(D3DXVECTOR3(rot[0], rot[1], rot[2]));
				}
				ImGui::TreePop();
			}
			// LocalScale�ϐ��̕\��
			if (ImGui::TreeNodeEx("Scale", ImGuiTreeNodeFlags_DefaultOpen)) {
				float scale[3] = { transform->m_localScale.x,transform->m_localScale.y ,transform->m_localScale.z };
				if (ImGui::InputFloat3("", scale)) {
					transform->m_localScale = D3DXVECTOR3(scale[0], scale[1], scale[2]);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// �e��\�����ύX
		if (ImGui::TreeNodeEx("Parent", ImGuiTreeNodeFlags_DefaultOpen)) {
			auto parent = transform->GetParent();
			char str[256];
			// �e������Ȃ�e�̖��O��\��
			if (parent) {
				strcpy_s(str, transform->GetParent()->GetGameObject()->GetName().c_str());
			}
			// ���Ȃ��Ȃ�uNone�v�ƕ\��
			else {
				strcpy_s(str, "None");
			}
			ImGui::PushItemWidth(-60);
			ImGui::InputText("##ParentName", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			static bool isSetParent = false;	// �V�����e��I�ԃ��X�g�{�b�N�X��\�����Ă��邩�ǂ���
			if (ImGui::Button("Set")) {
				isSetParent = !isSetParent;
			}
			if (isSetParent) {
				// �V�����e��I�ԃ��X�g�{�b�N�X��\��
				if (DispCreateParent(transform)) {
					isSetParent = false;
				}
			}
			ImGui::TreePop();
		}

		//if (ImGui::TreeNodeEx("World")) {
		//	// WorldPosition�ϐ��̕\��
		//	if (ImGui::TreeNodeEx("WorldPosition", ImGuiTreeNodeFlags_DefaultOpen)) {
		//		float pos[3] = { transform->m_worldPosition.x,transform->m_worldPosition.y ,transform->m_worldPosition.z };
		//		if (ImGui::InputFloat3("", pos)) {
		//			transform->m_worldPosition = D3DXVECTOR3(pos[0], pos[1], pos[2]);
		//		}
		//		ImGui::TreePop();
		//	}
		//	// WorldRotation�ϐ��̕\��
		//	if (ImGui::TreeNodeEx("WorldRotation", ImGuiTreeNodeFlags_DefaultOpen)) {
		//		D3DXVECTOR3 rotVec = transform->GetWorldRotationAsDegree();
		//		float rot[3] = { rotVec.x,rotVec.y ,rotVec.z };
		//		if (ImGui::InputFloat3("", rot)) {
		//			transform->SetWorldRotationFromDegree(D3DXVECTOR3(rot[0], rot[1], rot[2]));
		//		}
		//		ImGui::TreePop();
		//	}
		//	// WorldScale�ϐ��̕\��
		//	if (ImGui::TreeNodeEx("WorldScale", ImGuiTreeNodeFlags_DefaultOpen)) {
		//		float scale[3] = { transform->m_worldScale.x,transform->m_worldScale.y ,transform->m_worldScale.z };
		//		if (ImGui::InputFloat3("", scale)) {
		//			transform->m_worldScale = D3DXVECTOR3(scale[0], scale[1], scale[2]);
		//		}
		//		ImGui::TreePop();
		//	}
		//	ImGui::TreePop();
		//}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}


/// <summary>
/// �V�����e��I�ԃ��X�g�{�b�N�X��\������֐�
/// </summary>
/// <param name="yourName">�����̖��O</param>
bool DispCreateParent(Transform* transform)
{
	// �V�[����ɂ���I�u�W�F�N�g�̃��X�g���擾
	auto objectList = Manager::GetInstance()->GetScene()->GetAllGameObjects();
	// �����Ɠ����I�u�W�F�N�g�̓��X�g����폜
	objectList.remove_if([&transform](GameObject* object) {return object->GetID() == transform->GetGameObject()->GetID(); });
	int selectObjID = -1; // �I�񂾃I�u�W�F�N�g��ID
	GameObject* selectObj = nullptr;	// �I�񂾃I�u�W�F�N�g
	// ���X�g�{�b�N�X��\��
	if (ImGui::BeginListBox("##SetParent"))
	{
		// �e�ɂ���I�u�W�F�N�g��I��
		for (auto object : objectList)
		{
			const bool isSelected = (selectObjID == object->GetID());
			if (ImGui::Selectable(object->GetName().c_str(), isSelected)) {
				selectObjID = object->GetID();
				selectObj = object;
			}
		}
		// �e�𖳂��ɂ���Ȃ�
		const bool isNoneSelected = (selectObjID == -2);
		if (ImGui::Selectable("None", isNoneSelected)) {
			selectObjID = -2;
			selectObj = nullptr;
		}
		ImGui::EndListBox();
	}
	// �I�΂ꂽ�I�u�W�F�N�g���������Ȃ�
	if (selectObjID != -1) {
		// �q�G�����L�[�E�B���h�E�̖؍\����ύX
		auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
		int nodeID = transform->GetGameObject()->GetID();
		int parentID = -1;
		if (transform->GetParent()) {
			parentID = transform->GetParent()->GetGameObject()->GetID();
		}
		else {
			parentID = -2;
		}

		hierarchy->ChangeParentNode(nodeID, parentID, selectObjID);
		// �e�𖳂��ɂ���
		if (selectObjID == -2) {
			transform->SetParent(nullptr);
			
		}
		// �e��V�����e�ɕύX
		else {
			transform->SetParent(selectObj->GetTransform());
		}
		return true;
	}
	else {
		return false;
	}

}
