/*!
@file Task_Preliminary_NormalSlime.h
@brief Task_Preliminary_NormalSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace Enemy {
		class EnemyBase;

		//--------------------------------------------------------------------------------------
		/// 予備動作タスク
		//--------------------------------------------------------------------------------------
		class Task_Preliminary_NormalSlime : public TaskNodeBase<EnemyBase>
		{
		public:
			/// <summary>
			/// パラメータ
			/// </summary>
			struct Parametor
			{
				float time;   //どのくらいの時間で収縮するか
				float range;  //どのくらい収縮するか

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="time">収縮にかける時間</param>
				/// <param name="range">収縮する距離</param>
				Parametor(const float& time, const float& range);
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr; //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			/// <param name="paramPtr">パラメータ</param>
			Task_Preliminary_NormalSlime(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 向きの更新
			/// </summary>
			void Rotation();
		};

	}
}