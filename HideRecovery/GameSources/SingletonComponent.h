
/*!
@file SingletonComponent.h
@brief SingletonComponentクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "DebugObject.h"
#include "MaruUtility.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		template<class T>
		class SingletonComponent;

		//--------------------------------------------------------------------------------------
		///	シングルトンコンポーネント
		//--------------------------------------------------------------------------------------
		template<class T>
		class SingletonComponent : public Component
		{ 
			static ex_weak_ptr<T> sm_instance;	//自分自身のポインタ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SingletonComponent<T>(const std::shared_ptr<GameObject>& objPtr)
				:Component(objPtr)
			{}

			void OnDestroy() override {
				sm_instance = nullptr;
			}

			virtual void OnLateStart() override {
				if (this != GetInstance().get())
				{
					DebugObject::AddString(Util::GetWSTypeName<T>(), false);
					DebugObject::AddString(L"、は生成されているためコンポーネントを削除しました。");

					RemoveComponent<T>();
				}
			}

			/// <summary>
			/// 自分自身のスマートポインタの取得
			/// </summary>
			/// <returns>自分自身のスマートポインタ</returns>
			static std::shared_ptr<T> GetInstance() {
				return GetInstance(maru::Utility::GetStage());
			}

			/// <summary>
			/// 自分自身のスマートポインタの取得
			/// </summary>
			/// <param name="stage">検索するステージ</param>
			/// <returns>自分自身のスマートポインタ</returns>
			static std::shared_ptr<T> GetInstance(const std::shared_ptr<Stage>& stage) {
				if (sm_instance.GetShard() == nullptr)
				{
					sm_instance = maru::Utility::FindComponent<T>(stage);
					if (sm_instance.GetShard() == nullptr)
					{
						DebugObject::AddString(Util::GetWSTypeName<T>(), false);
						DebugObject::AddString(L"、がGameSceneに追加されていません");
					}
				}

				return sm_instance.GetShard();
			}
		};

	}
}

//endbasecross