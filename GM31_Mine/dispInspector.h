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

// ImGui上で各種コンポーネントを表示する関数
void DispComponent(AnimationModel* model);
void DispComponent(BoxCollision* collision);
void DispComponent(Camera* camera);
void DispComponent(QuadCollision* collision);
void DispComponent(Script* script);
void DispComponent(Shader* shader);
void DispComponent(SphereCollision* collision);
void DispComponent(Sprite* sprite);
void DispComponent(Model* model);
void DispComponent(Transform* transform);
