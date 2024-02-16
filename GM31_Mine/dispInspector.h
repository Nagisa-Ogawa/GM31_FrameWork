#pragma once

class AnimationModel;
class BoxCollision;
class Camera;
class QuadCollision;
class Script;
class Shader;
class SphereCollision;
class Sprite;
class Model;
class Transform;

// ImGui��Ŋe��R���|�[�l���g��\������֐�
// void DispComponent(AnimationModel* model);		// ������
void DispComponent(BoxCollision* collision);
void DispComponent(Camera* camera);
void DispComponent(QuadCollision* collision);
void DispComponent(Script* script);
void DispComponent(Shader* shader);
void DispComponent(SphereCollision* collision);
void DispComponent(Sprite* sprite);
void DispComponent(Model* model);
void DispComponent(Transform* transform);

bool DispCreateParent(Transform* transform);	// �I�u�W�F�N�g�̐e��I�ԃ��X�g�{�b�N�X��\������֐�
