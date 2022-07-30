
/*!
@file SlimeStatus.h
@brief SlimeStatusなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyStatusManagerBase;

		//--------------------------------------------------------------------------------------
		/// スライムのステータス管理
		//--------------------------------------------------------------------------------------
		class SlimeStatusManager : public EnemyStatusManagerBase
		{
		public:
			SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr);
			SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status);

		};

	}
}