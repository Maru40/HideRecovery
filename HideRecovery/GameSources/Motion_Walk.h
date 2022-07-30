
/*!
@file Motion_Walk.h
@brief Motion_Walk�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// �ړ����[�V����
			//--------------------------------------------------------------------------------------
			class Walk : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				Walk(const std::shared_ptr<EnemyBase>& owner);

			private:
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;
			};

		}
	}
}