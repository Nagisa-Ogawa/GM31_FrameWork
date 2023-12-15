#pragma once

#include <list>
#include <string>

// ファクトリーのインターフェース
template<class TFactory,class TObject>
class Factory
{
protected:
	// 生成されたオブジェクトのリスト
	std::list<TObject*> m_ObjectList;

	// コンストラクタの外部での使用を禁止
	Factory(){}
	// デストラクタの使用を禁止
	virtual ~Factory() 
	{
		delete m_Instance;
		m_Instance = nullptr;
	}

	// 初期化処理
	virtual void Init() = 0;
private:
	// コピーコンストラクタの使用を禁止
	Factory(const Factory& singleton) = delete;
	Factory& operator= (const Factory& singleton) = delete;
public:
	static TFactory* GetInstance()
	{
		if (m_Instance == nullptr) {
			m_Instance = new TFactory();
			m_Instance->Init();
		}
		return m_Instance;
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
	static TFactory* m_Instance;
};

template<class TFactory,class TObjcet>
TFactory* Factory<TFactory, TObjcet>::m_Instance = NULL;

