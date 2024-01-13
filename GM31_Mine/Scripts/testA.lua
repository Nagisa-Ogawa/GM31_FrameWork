local TestA = {}
local gameObject
local vec = Vector3(0, 0, 0)
local count = 0

function TestA.Start()
    ImGuiManager:DebugLog("TestA : Start")
end

function TestA.Update()
    local transform = gameObject:GetTransform()

    -- 移動のキー入力
    if Input.GetKeyPress(KEY_A) then
        transform.position = transform.position - transform:GetRight() * 0.1
    end
    if Input.GetKeyPress(KEY_D) then
        transform.position = transform.position + transform:GetRight() * 0.1
    end
    if Input.GetKeyPress(KEY_S) then
        transform.position = transform.position - transform:GetForward() * 0.1
    end
    if Input.GetKeyPress(KEY_W) then
        transform.position = transform.position + transform:GetForward() * 0.1
    end

    if Input.GetKeyPress(KEY_Q) then
        transform.rotation.y = transform.rotation.y - 0.05;
    end
    if Input.GetKeyPress(KEY_E) then
        transform.rotation.y = transform.rotation.y + 0.05;
    end

end

-- スクリプトがアタッチされているオブジェクトを取得
function GetGameObject(object)
    gameObject = object
end

return TestA

