#pragma once

class AnimationModel;
class BoxCollision;
class Camera;
class QuadCollision;
class Script;
class Shader;
class SphereCollision;
class Sprite;
class MeshField;
class Model;
class Transform;

// ImGui上で各種コンポーネントを表示する関数
void DispComponent(BoxCollision* collision);
void DispComponent(Camera* camera);
void DispComponent(QuadCollision* collision);
void DispComponent(Script* script);
void DispComponent(Shader* shader);
void DispComponent(SphereCollision* collision);
void DispComponent(Sprite* sprite);
void DispComponent(Model* model);
void DispComponent(Transform* transform);
void DispComponent(MeshField* meshField);

bool DispCreateParentListBox(Transform* transform);	// オブジェクトの親を選ぶリストボックスを表示する関数
bool DispSetFieldTargetListBox(MeshField* meshField);	// フィールドのターゲットを選ぶリストボックスを表示する関数
