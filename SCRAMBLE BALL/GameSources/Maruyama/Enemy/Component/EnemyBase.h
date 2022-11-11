
/*!
@file EnemyBase.h
@brief EnemyBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

//#include "Watanabe/Utility/Utility.h"
//#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {

	class TargetManager;
	class OnlinePlayerSynchronizer;
	class VelocityManager;

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	エネミーの基底クラス
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember, public Tuple::I_Tupler
		{
			std::shared_ptr<Tuple::TupleSpace> m_tupleSpace;	//タプルスペース

			std::weak_ptr<TargetManager> m_targetManager;		//ターゲット管理
			std::weak_ptr<VelocityManager> m_velocityManager;	//速度管理
			std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//オンラインシンクロ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr);

			virtual ~EnemyBase() = default;

			virtual void OnCreate() override;
			virtual void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	インターフェースの実装
			//--------------------------------------------------------------------------------------

			virtual void SetTarget(const std::shared_ptr<GameObject>& target) override;
			 
			virtual std::shared_ptr<GameObject> GetTarget() const override;

			virtual std::shared_ptr<GameObject> GetSelfObject() const override;

			_NODISCARD virtual std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept override;
		};
	}
}

//endbasecross