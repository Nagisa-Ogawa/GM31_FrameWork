#include "MyImGuiManager.h"
#include "renderer.h"
#include "gameObject.h"
#include "dispInspector.h"
#include "animationModel.h"
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

void DispComponent(AnimationModel* model)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("AnimationModel", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("FileName : %s", model->GetFileName().c_str());
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DispComponent(BoxCollision* collision)
{
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	bool treeopen = ImGui::TreeNodeEx("BoxCollision", ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
	// コンポーネント削除ボタンを作成
	ImGui::SameLine(ImGui::GetWindowWidth() - 40);
	if (ImGui::Button("X")) {
		collision->SetDestroy();
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
}

void DispComponent(Camera* camera)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DispComponent(Model* model)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("FileName : %s", model->GetFileName().c_str());
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DispComponent(QuadCollision* collision)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("QuadCollision", ImGuiTreeNodeFlags_DefaultOpen)) {
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
}

void DispComponent(Script* script)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("FileName : %s", script->GetFileName().c_str());
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DispComponent(Shader* shader)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::TreeNodeEx("VertexShader", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("FileName : %s", shader->GetVSFile().c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("PixelShader", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("FileName : %s", shader->GetPSFile().c_str());
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DispComponent(SphereCollision* collision)
{
	ImGui::Separator();
	if (ImGui::TreeNodeEx("SphereCollision", ImGuiTreeNodeFlags_DefaultOpen)) {
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
}

void DispComponent(Sprite* sprite)
{
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
}

void DispComponent(Transform* transform)
{
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
}