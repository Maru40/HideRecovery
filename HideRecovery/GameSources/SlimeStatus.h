
/*!
@file SlimeStatus.h
@brief SlimeStatus�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyStatusManagerBase;

		//--------------------------------------------------------------------------------------
		/// �X���C���̃X�e�[�^�X�Ǘ�
		//--------------------------------------------------------------------------------------
		class SlimeStatusManager : public EnemyStatusManagerBase
		{
		public:
			SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr);
			SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status);

		};

	}
}