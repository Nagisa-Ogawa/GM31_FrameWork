#pragma once

#include <list>
#include <string>
#include <memory>

//-----------------------------------------------------
// テンプレート版フライウェイトパターン実装用親クラス
//------------------------------------------------------
template<class TFactory,class TObject>
class Factory
{
protected:
	// 生成されたオブジェクトのリスト
	std::list<TObject*> m_objectList;

	// コンストラクタの外部での使用を禁止
	Factory(){}
	// デストラクタの使用を禁止
	virtual ~Factory() 
	{
		m_instance = nullptr;
	}

	// 初期化処理
	virtual void Init() = 0;
private:
	// コピーコンストラクタの使用を禁止
	Factory(const Factory& singleton) = delete;
	Factory& operator= (const Factory& singleton) = delete;
public:
	static Factory* GetInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new TFactory();
			m_instance->Init();
		}
		return m_instance;
	}

	// 生成されたオブジェクトを入手する関数
	virtual TObject* ActiveObject(std::string name) = 0;
	// オブジェクトを非表示にする（deleteはしない)関数
	virtual void HideObject(TObject* t) = 0;

	// 終了時処理
	virtual void Uninit()
	{
	}

	virtual void Clear() 
	{

	}
private:
	static TFactory* m_instance;
};

template<class TFactory,class TObjcet>
TFactory* Factory<TFactory, TObjcet>::m_instance = NULL;

