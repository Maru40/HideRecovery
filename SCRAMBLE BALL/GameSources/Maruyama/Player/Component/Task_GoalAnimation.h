
/*!
@file Task_GoalAnimation.h
@brief Task_GoalAnimation�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {
	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �W�����v��̌��̈ʒu�ɖ߂鏈���̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct ReturnJump_Parametor {
			Vec3 returnDirect;
			float jumpRad;
			float jumpUpPower;
			float jumpHorizontalPower;

			ReturnJump_Parametor();
		};

		//--------------------------------------------------------------------------------------
		/// �W�����v��̌��̈ʒu�ɖ߂鏈��
		//--------------------------------------------------------------------------------------
		class ReturnJump : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = ReturnJump_Parametor;

		private:
			std::weak_ptr<Parametor> m_param;

		public:
			ReturnJump(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			Vec3 CalculateJumpVec();

		public:
			std::shared_ptr<Parametor> GetParametor() const { return m_param.lock(); };
		};

	}
}