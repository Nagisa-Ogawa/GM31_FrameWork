-- どのスクリプトにも必要な変数
local TestA = {}
local gameObject

-- ゲームオブジェクトが作成されたときに一度だけ実行される関数
function TestA.Start()
    -- ImGuiManager:DebugLog("TestA : Start")
end

-- 毎フレーム呼ばれる更新関数
function TestA.Update()
    local transform = gameObject:GetTransform()

    -- 移動のキー入力
    if Input.GetKeyPress(KEY_A) then
        transform.localPosition = transform.localPosition - transform:GetRight() * 0.1
    end
    if Input.GetKeyPress(KEY_D) then
        transform.localPosition = transform.localPosition + transform:GetRight() * 0.1
    end
    if Input.GetKeyPress(KEY_S) then
        transform.localPosition = transform.localPosition - transform:GetForward() * 0.1
    end
    if Input.GetKeyPress(KEY_W) then
        transform.localPosition = transform.localPosition + transform:GetForward() * 0.1
    end

end

-- スクリプトがアタッチされているオブジェクトを取得
function GetGameObject(object)
    gameObject = object
end

return TestA

