/*!
@file Task_Preliminary_BigSlime.h
@brief Task_Preliminary_BigSlimeなど
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

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// 巨大スライムの攻撃予備動作パラメータ
				//--------------------------------------------------------------------------------------
				struct Preliminary_Attack_Parametor {
					float time;   //どのくらいの時間で収縮する

					Preliminary_Attack_Parametor();
					Preliminary_Attack_Parametor(const float& time);
				};

				//--------------------------------------------------------------------------------------
				/// 巨大スライムの攻撃予備動作タスク
				//--------------------------------------------------------------------------------------
				class Preliminary_Attack : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Preliminary_Attack_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //パラメータ

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
					/// <param name="paramPtr">パラメータ</param>
					Preliminary_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// 向きの調整処理
					/// </summary>
					void Rotation();
				};

			}
		}
	}
}

