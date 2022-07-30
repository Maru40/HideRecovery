#pragma once
#include"stdafx.h"

namespace basecross
{
	//ファンクションInterfaceクラス
	class I_Function
	{
	public:

		virtual bool operator()() const = 0;
	};


	//オブジェクトの関数ポインタを生成する用のクラス
	template<class T>
	class C_Function : public I_Function
	{
	public:

		std::weak_ptr<T> m_objPtr;  //そのオブジェクト
		void(T::*m_fn)();           //テンプレート型の関数ポインタを代入する変数

		void nullFunction() {}

	public:

		C_Function(std::weak_ptr<T> obj, void(T::*fn)()) :
			m_objPtr(obj),
			m_fn(fn)
		{

		}

		virtual bool operator()() const override   //オペレータのオーバーライド
		{
			auto obj = m_objPtr.lock();
			if (!obj)
			{
				return false;
			}

			auto* ptr = obj.get();   //オブジェクトの生ポインタを取得
			(ptr->*m_fn)();          //オブジェクトの取得していた関数を呼び出す。
			return true;
		}
	};


	//デリゲードクラス
	class MyDelegate
	{
		std::vector<std::shared_ptr<I_Function>> m_functions;    //I_Functionを持つベクター配列

	public:

		//MyDelegate() {};

		template<class T>
		void AddFunction(std::shared_ptr<T> thisPtr, void(T::*fn)())   //関数の追加
		{
			std::shared_ptr<I_Function> shard = std::make_shared<C_Function<T>>(thisPtr, fn);  //ファンクションの生成
			m_functions.push_back(shard);   //配列に追加
		}

		void RemoveAll()   //全てのファンクションの削除
		{
			m_functions.clear();
		}

		void Invoke() //invoke関数
		{
			if (m_functions.empty())  //配列が空だったら処理を行わない。
			{
				return;
			}

			auto iter = m_functions.begin();   //イテレータの取得

			while (iter != m_functions.end())  //イテレータが最後になるまで
			{

				//関数ポインタを使いたいときは生ポインタを使う必要がある。
				bool b = (*(iter->get()))();  //生ポインタを取得し、そのアドレス先のオーバーライドした関数を呼び出す。
				
				if (!b)  //もしfalseがかえってきたらobjPtrの取得に失敗
				{
					iter = m_functions.erase(iter);  //そのため配列から削除
					continue;
				}

				iter++;
			}
		}

		bool empty()  //エンプティー
		{
			return m_functions.empty();
		}
	};

}