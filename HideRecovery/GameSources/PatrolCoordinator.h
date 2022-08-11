
/*!
@file PatrolCoordinator.h
@brief PatrolCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// �p�g���[�����̒�����
		//--------------------------------------------------------------------------------------
		class PatrolCoordinator : public HereOwnerCoordinatorBase<FactionCoordinator, EnemyBase>
		{
		public:
			PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~PatrolCoordinator() = default;
		};

	}
}