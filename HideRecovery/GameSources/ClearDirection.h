
/*!
@file ClearDirection.h
@brief ClearDirectionなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class SpriteObject;
	class ClearUIObject;

	namespace Task {
		struct Wait_Parametor;
		struct InsertUI_Parametor;
	}

	struct ScalePop_Parametor;

	//--------------------------------------------------------------------------------------
	/// クリア演出パラメータ
	//--------------------------------------------------------------------------------------
	struct ClearDirection_Parametor {
		float slowTime;                                                        //Slowにする時間
		float slowTimeSpeed;                                                   //Slowのスピード
		std::shared_ptr<Task::Wait_Parametor> beforeInsetUIStartWaitParamPtr;  //開始するまでの待機時間
		float fadeInBackSpriteTime;                                            //フェードインにかかる時間
		std::shared_ptr<Task::InsertUI_Parametor> insertUIParamPtr;            //UIを表示するまでのパラメータ
		std::shared_ptr<Task::Wait_Parametor> endWaitParamPtr;                 //UIを表示してから、完全にタスクを終了するまでの時間。

		ClearDirection_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// クリア演出管理
	//--------------------------------------------------------------------------------------
	class ClearDirection : public Component
	{
	public:
		using Parametor = ClearDirection_Parametor;

	private:
		/// <summary>
		/// タスクタイプ
		/// </summary>
		enum class TaskEnum {
			Wait_BeforeInsertUI, //UIを表示する前の待機
			FadeInBackSprite,    //バックスプライトをフェードインさせる
			InsertUI,            //ClearUIを表示
			Wait_End,            //UI表示後の待機
			ChangeStageStart,    //ステート遷移を開始する
		};

		Parametor m_param;                               //パラメータ
		std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス
		
		ex_weak_ptr<ClearUIObject> m_clearUI;            //クリアUI

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ClearDirection(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		ClearDirection(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// 演出開始
		/// </summary>
		void Start();

	private:
		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		void SelectTask();

		/// <summary>
		/// クリアUIの生成
		/// </summary>
		void CreateClearUI();

	};

	//--------------------------------------------------------------------------------------
	/// タスクの実装
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// UIの挿入パラメータ
		//--------------------------------------------------------------------------------------
		struct InsertUI_Parametor {
			std::shared_ptr<ScalePop_Parametor> scalePopParamPtr; //スケール変更パラメータ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			InsertUI_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scalePopParamPtr">スケール変更パラメータ</param>
			InsertUI_Parametor(const std::shared_ptr<ScalePop_Parametor>& scalePopParamPtr);
		};

		//--------------------------------------------------------------------------------------
		/// UIの挿入タスク
		//--------------------------------------------------------------------------------------
		class InsertUI : public TaskNodeBase<SpriteObject>
		{
		public:
			using Parametor = InsertUI_Parametor;

		private:
			ex_weak_ptr<Parametor> m_paramPtr; //パラメータ

		public:
			/// <summary>
			/// 
			/// </summary>
			InsertUI(const std::shared_ptr<SpriteObject> owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}