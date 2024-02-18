#include "main.h"
#include "fieldObject.h"
#include "renderer.h"
#include "shader.h"
#include "meshField.h"

void FieldObject::Init()
{
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<MeshField>()->Init("Assets\\Textures\\floor01.jpg");
}
