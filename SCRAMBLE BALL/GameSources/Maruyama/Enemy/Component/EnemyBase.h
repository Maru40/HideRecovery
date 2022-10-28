
/*!
@file EnemyBase.h
@brief EnemyBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

//#include "Watanabe/Utility/Utility.h"
//#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {

	class TargetManager;

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	エネミーの基底クラス
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember
		{
			//team::TeamType m_teamType;						//チームタイプ

			std::weak_ptr<TargetManager> m_targetManager;	//ターゲット管理

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr) :
				Component(objPtr)
			{ }

			virtual void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	インターフェースの実装
			//--------------------------------------------------------------------------------------

			virtual void SetTarget(const std::shared_ptr<GameObject>& target) override;
			 
			virtual std::shared_ptr<GameObject> GetTarget() const override;

			virtual std::shared_ptr<GameObject> GetSelfObject() const override;
		};
	}
}

//endbasecross