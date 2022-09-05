
/*!
@file ChaseState_TargetSeek.h
@brief ChaseState_TargetSeekなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SeekTarget.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class EnemyStateNodeBase;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode
		{
			namespace ChaseState {

				//--------------------------------------------------------------------------------------
				/// ターゲット追従パラメータ
				//--------------------------------------------------------------------------------------
				struct TargetSeekParametor
				{
					std::shared_ptr<SeekTarget::Parametor> seekParamPtr = nullptr;  //追従パラメータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					TargetSeekParametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="seekParamPtr">追従パラメータ</param>
					TargetSeekParametor(const std::shared_ptr<SeekTarget::Parametor>& seekParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// ターゲット追従ステート
				//--------------------------------------------------------------------------------------
				class ChaseState_TargetSeek : public EnemyStateNodeBase<EnemyBase>
				{
				private:
					ex_weak_ptr<TargetSeekParametor> m_paramPtr = nullptr;  //パラメータ

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するクラス</param>
					/// <param name="parametor">追従パラメータ</param>
					ChaseState_TargetSeek(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<TargetSeekParametor>& parametor);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// 向きの更新
					/// </summary>
					void RotationUpdate();

				};
			}
		}
	}
}

//endbasecross