
/*!
@file EvadeSlime_Evade.h
@brief EvadeSlime_Evadeなど
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

	namespace Task {
		struct AroundEyeCheck_Parametor;
		struct AstarMove_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace Task {
			struct Flee_Parametor;
			
		}

		namespace EvadeSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// 逃げるステートのパラメータ
				//--------------------------------------------------------------------------------------
				struct Evade_Parametor
				{
					std::shared_ptr<Enemy::Task::Flee_Parametor> fleeParamPtr;                         //対象から真逆に逃げるパラメータ
					std::shared_ptr<basecross::Task::AstarMove_Parametor> astarMoveParamPtr;           //Astarを利用して逃げるパラメータ
					std::shared_ptr<basecross::Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr; //周囲を確認するパラメータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					Evade_Parametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="fleeParamPtr">対象から真逆に逃げるパラメータ</param>
					/// <param name="astarMoveParamPtr">Astarを利用して逃げるパラメータ</param>
					/// <param name="aroundEyeCheckParamPtr">周囲を確認するパラメータ</param>
					Evade_Parametor(
						const std::shared_ptr<Enemy::Task::Flee_Parametor>& fleeParamPtr,
						const std::shared_ptr<basecross::Task::AstarMove_Parametor>& astarMoveParamPtr,
						const std::shared_ptr<basecross::Task::AroundEyeCheck_Parametor>& aroundEyeCheckParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// 逃げるステート
				//--------------------------------------------------------------------------------------
				class Evade : public EnemyStateNodeBase<EnemyBase>
				{
				public:
					using Parametor = Evade_Parametor;

					/// <summary>
					/// タスクタイプ
					/// </summary>
					enum class TaskEnum {
						Flee,            //対象の真逆に移動する逃避行動
						AstarEvade,      //Astarを利用した逃避行動
						AroundEyeCheck,  //周りを目で確認する。
					};

				private:
					ex_weak_ptr<Parametor> m_paramPtr;              //パラメータ

					std::unique_ptr<TaskList<TaskEnum>> m_taskList; //タスク管理クラス

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="objPtr">このクラスを所有するクラス</param>
					/// <param name="paramPtr">パラメータ</param>
					Evade(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor> paramPtr);

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