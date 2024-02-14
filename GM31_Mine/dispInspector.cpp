#include "MyImGuiManager.h"
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
// 各種コンポーネントをImGuiで表示する関数
//
//----------------------------------------------

void DispComponent(BoxCollision* collision)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	std::string name = "BoxCollision##" + std::to_string(collision->GetID());
	bool treeopen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// コンポーネント削除ボタンを作成
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// ボタンの色を赤色にする
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
	// コンポーネントの要素を表示
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
		// モデル変更用ボタン
		ImGui::SameLine(ImGui::GetWindowWidth() - 60);
		if (ImGui::Button("Set")) {
			std::string filePath;
			if (OpneFileDialog(GetWindow(), "ファイルを選択してください", "Assets\\Models", "ファイル(.obj)\0*.obj\0", &filePath)) {
				// モデル変更
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
	// コンポーネント削除ボタンを作成
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// ボタンの色を赤色にする
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
	// コンポーネントの要素を表示
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
	// コンポーネント削除ボタンを作成
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// ボタンの色を赤色にする
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
	// コンポーネントの要素を表示
	if (treeopen) {
		char str[256];
		strcpy_s(str, script->GetFileName().c_str());
		ImGui::PushItemWidth(-60);
		ImGui::InputText("##ScriptFileName", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		// スクリプトファイル変更用ボタン
		ImGui::SameLine(ImGui::GetWindowWidth() - 60);
		if (ImGui::Button("Set")) {
			std::string filePath;
			if (OpneFileDialog(GetWindow(), "ファイルを選択してください", "Assets\\Scripts", "ファイル(.lua)\0*.lua\0", &filePath)) {
				// スクリプトファイル変更
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
			// VSShader変更用ボタン
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			if (ImGui::Button("Set")) {
				std::string filePath;
				if (OpneFileDialog(GetWindow(), "ファイルを選択してください", "Assets\\Shaders", "ファイル(VS.cso)\0*VS.cso\0", &filePath)) {
					// シェーダー変更
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
			// PSShader変更用ボタン
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			if (ImGui::Button("Set")) {
				std::string filePath;
				if (OpneFileDialog(GetWindow(), "ファイルを選択してください", "Assets\\Shaders", "ファイル(.PScso)\0*PS.cso\0", &filePath)) {
					// シェーダー変更
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
	// コンポーネント削除ボタンを作成
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	// ボタンの色を赤色にする
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
	// コンポーネントの要素を表示
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
		if (ImGui::TreeNodeEx("Local",ImGuiTreeNodeFlags_DefaultOpen))	{
			// LoacalPosition変数の表示
			if (ImGui::TreeNodeEx("LocalPosition",ImGuiTreeNodeFlags_DefaultOpen)) {
				float pos[3] = { transform->m_localPosition.x,transform->m_localPosition.y ,transform->m_localPosition.z };
				if (ImGui::InputFloat3("", pos)) {
					transform->m_localPosition = D3DXVECTOR3(pos[0], pos[1], pos[2]);
				}
				ImGui::TreePop();
			}
			// LocalRotation変数の表示
			if (ImGui::TreeNodeEx("LocalRotation",ImGuiTreeNodeFlags_DefaultOpen)) {
				D3DXVECTOR3 rotVec = transform->GetLocalRotationAsDegree();
				float rot[3] = { rotVec.x,rotVec.y ,rotVec.z};
				if (ImGui::InputFloat3("", rot)) {
					transform->SetLocalRotationFromDegree(D3DXVECTOR3(rot[0], rot[1], rot[2]));
				}
				ImGui::TreePop();
			}
			// LocalScale変数の表示
			if (ImGui::TreeNodeEx("LocalScale", ImGuiTreeNodeFlags_DefaultOpen)) {
				float scale[3] = { transform->m_localScale.x,transform->m_localScale.y ,transform->m_localScale.z };
				if (ImGui::InputFloat3("", scale)) {
					transform->m_localScale = D3DXVECTOR3(scale[0], scale[1], scale[2]);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("World")) {

			// WorldPosition変数の表示
			if (ImGui::TreeNodeEx("WorldPosition", ImGuiTreeNodeFlags_DefaultOpen)) {
				float pos[3] = { transform->m_worldPosition.x,transform->m_worldPosition.y ,transform->m_worldPosition.z };
				if (ImGui::InputFloat3("", pos)) {
					transform->m_worldPosition = D3DXVECTOR3(pos[0], pos[1], pos[2]);
				}
				ImGui::TreePop();
			}
			// WorldRotation変数の表示
			if (ImGui::TreeNodeEx("WorldRotation", ImGuiTreeNodeFlags_DefaultOpen)) {
				D3DXVECTOR3 rotVec = transform->GetWorldRotationAsDegree();
				float rot[3] = { rotVec.x,rotVec.y ,rotVec.z };
				if (ImGui::InputFloat3("", rot)) {
					transform->SetWorldRotationFromDegree(D3DXVECTOR3(rot[0], rot[1], rot[2]));
				}
				ImGui::TreePop();
			}
			// WorldScale変数の表示
			if (ImGui::TreeNodeEx("WorldScale", ImGuiTreeNodeFlags_DefaultOpen)) {
				float scale[3] = { transform->m_worldScale.x,transform->m_worldScale.y ,transform->m_worldScale.z };
				if (ImGui::InputFloat3("", scale)) {
					transform->m_worldScale = D3DXVECTOR3(scale[0], scale[1], scale[2]);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::PopStyleVar();
}