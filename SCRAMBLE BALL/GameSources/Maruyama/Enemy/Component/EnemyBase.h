
/*!
@file EnemyBase.h
@brief EnemyBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

namespace basecross {

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	エネミーの基底クラス
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr)
				:Component(objPtr)
			{}
		};
	}
}

//endbasecross