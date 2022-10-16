/*!
@file AIPlayerBehaviorTree.h
@brief AIPlayerBehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTreeComponentBase.h"

namespace basecross {

	namespace Enemy {

		enum class EnemyBehaviorTreeType {
			FirstSelecter,		//�����Z���N�^�[

			PlowlingSelecter,	//�p�j�Z���N�^�[
				SearchBallTask,		//�{�[����T��

			ButtleSelecter,		//�o�g���Z���N�^�[
				AttackSelecter,		//�U���Z���N�^�[
		};

		class AIPlayerBehaviorTree : public BehaviorTreeComponentBase<EnemyBehaviorTreeType>
		{
		public:
			using BehaviorType = EnemyBehaviorTreeType;

		private:

		public:
			AIPlayerBehaviorTree(const std::shared_ptr<GameObject>& objPtr);

		private:
			void CreateNode() override;
			void CreateEdge() override;
		};

	}
}