
/*!
@file ShootSlime_Attack.h
@brief ShootSlime_Attackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Task{
		struct Wait_Parametor;
	}
	
	namespace Enemy {

		class EnemyBase;

		namespace ShootSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// 攻撃パラメーター
				//--------------------------------------------------------------------------------------
				struct Attack_Parametor 
				{
					std::shared_ptr<Task::Preliminary_Shoot_Parametor> preliminaryParamPtr = nullptr; //予備動作パラメータ
					std::shared_ptr<Task::Shoot_Parametor> shootParamPtr = nullptr;                   //撃つパラメータ
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;          //硬直パラメータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					Attack_Parametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="preliminaryParamPtr">予備動作パラメータ</param>
					/// <param name="shootParamPtr">撃つパラメータ</param>
					/// <param name="waitParamPtr">硬直パラメータ</param>
					Attack_Parametor(
						const  std::shared_ptr<Task::Preliminary_Shoot_Parametor>& preliminaryParamPtr,
						const std::shared_ptr<Task::Shoot_Parametor>& shootParamPtr,
						const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// ショットスライムの攻撃ステート
				//--------------------------------------------------------------------------------------
				class Attack : public EnemyStateNodeBase<EnemyBase>
				{
				public:
					/// <summary>
					/// タスクタイプ
					/// </summary>
					enum class TaskEnum {
						Preliminary,  //予備動作
						Shoot,        //攻撃動作
						Wait,         //攻撃後硬直
						Reload,       //弾のリロード
					};

					using Parametor = Attack_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;               //パラメータ

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するクラス</param>
					/// <param name="paramPtr">パラメータ</param>
					Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// タスクの定義
					/// </summary>
					void DefineTask();

					/// <summary>
					/// タスクの選択
					/// </summary>
					void SelectTask();

				};

			}
		}
	}
}