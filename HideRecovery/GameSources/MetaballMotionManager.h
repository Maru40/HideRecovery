/*!
@file MetaballMotionManager.h
@brief MetaballMotionManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class GameTimer;

	namespace Enemy {
		class SlimeCore;
	}

	namespace Metaball {

		class ChirdlenRender;

		namespace Task {

			//--------------------------------------------------------------------------------------
			///	追従オフセット移動パラメータ
			//--------------------------------------------------------------------------------------
			struct SeekOffsetMove_Parametor {
				float time;                                //どのくらい時間を掛けて遷移するか
				float radius;                              //遷移するスフィアの大きさ
				vector<SeekTarget::Parametor> seekParams;  //遷移するパラメータ
				SeekTarget::Parametor coreSeekParam;       //コアの追従パラメータ
				float firstSpeed;                          //初期スピード

				/// <summary>
				/// コンストラクタ
				/// </summary>
				SeekOffsetMove_Parametor();
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="time">移動時間</param>
				/// <param name="radius">半径</param>
				SeekOffsetMove_Parametor(const float& time, const float& radius);
			};

			//--------------------------------------------------------------------------------------
			///	補間移動用の初期値補間データ
			//--------------------------------------------------------------------------------------
			struct SaveLerpData {
				float radius;      //半径
				Vec3 startOffset;  //初期オフセット値
				Vec3 velocity;     //初期速度

				/// <summary>
				/// コンストラクタ
				/// </summary>
				SaveLerpData()
					:SaveLerpData(1.0f, Vec3(0.0f))
				{}

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="radius">半径</param>
				/// <param name="startOffset">初期オフセット</param>
				SaveLerpData(const float& radius, const Vec3& startOffset)
					:radius(radius), startOffset(startOffset), velocity(Vec3(0.0f))
				{}
			};

			//--------------------------------------------------------------------------------------
			///	追従オフセット移動
			//--------------------------------------------------------------------------------------
			class SeekOffsetMove : public TaskNodeBase<GameObject>
			{
			public:
				using Parametor = SeekOffsetMove_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;     //パラメータ
				vector<SaveLerpData> m_saveLerpDatas;            //Lerp移動用の初期値保存
				SaveLerpData m_coreSaveLerpData;                 //CoreのLerp移動用の初期値保存

				vector<ex_weak_ptr<SeekTarget>> m_seekTargets;   //追従コンポーネント群
				ex_weak_ptr<Enemy::SlimeCore> m_core;            //Coreデータ
				std::unique_ptr<GameTimer> m_timer = nullptr;    //タイマー管理

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				/// <param name="paramPtr">パラメータのポインタ</param>
				SeekOffsetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// Coreデータをメンバ変数に取得する。
				/// </summary>
				void SettingCore();

				/// <summary>
				/// 追従パラメータ群をメンバ変数に取得する。
				/// </summary>
				void SettingSeekTargets();

				/// <summary>
				/// 追従オフセットのアップデート管理
				/// </summary>
				/// <param name="seek">追従コンポーネント</param>
				/// <param name="seekParam">追従パラメータ</param>
				/// <param name="saveLerpData">Lerp用の保存データ</param>
				void SeekOffsetUpdate(const std::shared_ptr<SeekTarget>& seek,
					const SeekTarget::Parametor& seekParam,
					const SaveLerpData& saveLerpData);

				/// <summary>
				/// 追従オフセットの補間処理を入れたアップデート管理
				/// </summary>
				/// <param name="seek">追従コンポーネント</param>
				/// <param name="seekParam">追従パラメータ</param>
				/// <param name="saveLerpData">Lerp用の保存データ</param>
				void SeekOffsetLerpUpdate(const std::shared_ptr<SeekTarget>& seek,
					const SeekTarget::Parametor& seekParam,
					const SaveLerpData& saveLerpData);

				/// <summary>
				/// メタボールの半径をアップデートで変化させる処理。
				/// </summary>
				/// <param name="render">メタボールレンダー用コンポーネント</param>
				/// <param name="radius">メタボールの半径</param>
				/// <param name="speed">変形するスピード</param>
				/// <param name="startRadius">初期半径</param>
				void RadiusUpdate(const std::shared_ptr<basecross::Metaball::ChildrenRender>& render, 
					const float& radius, const float& speed, const float& startRadius);

				/// <summary>
				/// 補完された目的地を返す
				/// </summary>
				/// <param name="startPosition">開始位置</param>
				/// <param name="endPosition">終了位置</param>
				/// <returns>補間された目的地</returns>
				Vec3 CalcuLerpMovePosition(const Vec3& startPosition, const Vec3& endPosition);

				/// <summary>
				/// 更新速度を返す。
				/// </summary>
				/// <returns>更新速度</returns>
				float CalculateUpdateTimeSpeed();

				/// <summary>
				/// Lerp用の初期データを保存する。
				/// </summary>
				void SaveLerpDatas();

				/// <summary>
				/// 終了判断
				/// </summary>
				/// <returns>終了しているならtrue</returns>
				bool IsEnd();
			};
		}

		//--------------------------------------------------------------------------------------
		///	モーション管理クラス
		//--------------------------------------------------------------------------------------
		class MotionManager
		{
		public:
			/// <summary>
			/// タスクタイプ
			/// </summary>
			enum class TaskEnum {
				Move, //移動
			};

		private:
			ex_weak_ptr<GameObject> m_owner = nullptr; //所有者

			std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;                                //タスクリスト管理クラス
			std::shared_ptr<Task::SeekOffsetMove::Parametor> m_currentSeekOffetParametor = nullptr;  //現在の追従パラメータ
			std::queue<Task::SeekOffsetMove::Parametor> m_seekOffsetParametors;                      //追従パラメータキュー

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			MotionManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// 外部からの呼び出しする更新処理
			/// </summary>
			void OnUpdate();

		private:
			/// <summary>
			/// タスクの定義
			/// </summary>
			void DefineTask();

			/// <summary>
			/// タスクの選択
			/// </summary>
			/// <param name="task">選択したタスク</param>
			void SelectTask(const TaskEnum& task);

			/// <summary>
			/// 一つのタスクが終了した場合に呼び出す処理
			/// </summary>
			void OneTaskEndFunction();

			/// <summary>
			/// 現在の追従パラメータを変更する。
			/// </summary>
			/// <returns>変更できたらtrue</returns>
			bool ChangeCurrentParametor();

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 自分自身を所有するゲームオブジェクトの取得
			/// </summary>
			/// <returns>自分自身を所有するゲームオブジェクト</returns>
			std::shared_ptr<GameObject> GetOwner() const { return m_owner.GetShard(); }

			/// <summary>
			/// タスクの追加
			/// </summary>
			/// <param name="taskEnum">追加するタスクのタイプ</param>
			/// <param name="parametor">追従オフセットパラメータ</param>
			void AddTask(const TaskEnum& taskEnum, const Task::SeekOffsetMove_Parametor& parametor);

			/// <summary>
			/// 強制ストップ
			/// </summary>
			/// <param name="isExitFunc">終了時の関数を呼び出すかどうか</param>
			void ForceStop(const bool isExitFunction = true);

			/// <summary>
			/// 終了判断
			/// </summary>
			/// <returns>終了しているならtrue</returns>
			bool IsEnd();

		};

	}

}