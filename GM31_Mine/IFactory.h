#pragma once

#include <list>

// ファクトリーのインターフェース
template<class TFactory,class TObject>
class IFactory
{
protected:
	// 生成されたオブジェクトのリスト
	std::list<TObject*> m_ObjectList;

	// コンストラクタの外部での使用を禁止
	IFactory(){}
	// デストラクタの使用を禁止
	vrtual ~IFactory() { std::atexit(Uninit); }

	// 初期化処理
	virtual void Init() = 0;
	// 終了時処理
	virtual void Uninit() 
	{
		// インスタンスを解放
		delete m_Instance;
	}
private:
	// コピーコンストラクタの使用を禁止
	IFactory(const IFactory& singleton) = delete;
	IFactory& operator= (const IFactory& singleton) = delete;
public:
	static IFactory* GetInstance()
	{
		if (m_Instance == nullptr) {
			m_Instance = new TFactory();
			m_Instance->Init();
		}
	}

	// 生成されたオブジェクトを入手する関数
	TObject* ActiveObject() = 0;
	// オブジェクトを非表示にする（deleteはしない)関数
	void HideObject(TObject* t) = 0;
private:
	static TFactory* m_Instance;
};

template<class TFactory,class TObjcet>
TFactory* IFactory<TFactory, TObjcet>::m_Instance = NULL;

