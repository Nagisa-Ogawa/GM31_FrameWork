-- どのスクリプトにも必要な変数
local player = {} -- クラスを表すテーブル
local gameObject -- このスクリプトがコンポーネントされているオブジェクト
local count=0

-- ゲームオブジェクトが作成されたときに一度だけ実行される関数
function player.Start()
    ImGuiManager:DebugLog(string.format("two : %d",count))
    count=count+1
end

-- 毎フレーム呼ばれる更新関数
function player.Update()
    local transform = gameObject:GetTransform()
    -- 回転のキー入力
    if Input.GetKeyPress(KEY_Q) then
        transform.localRotation.y = transform.localRotation.y - 0.05;
    end
    if Input.GetKeyPress(KEY_E) then
        transform.localRotation.y = transform.localRotation.y + 0.05;
    end
end

-- スクリプトがアタッチされているオブジェクトを取得
function GetGameObject(object)
    gameObject = object
end

return player
