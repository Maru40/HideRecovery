
/*!
@file Motion_BodyBlow.h
@brief Motion_BodyBlow�Ȃ�
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

		namespace NormalSlime {

			namespace Motion {

				//--------------------------------------------------------------------------------------
				/// �{�f�B�[�u���[���[�V����
				//--------------------------------------------------------------------------------------
				class BodyBlow : public maru::MetaballMotionBase<EnemyBase>
				{
				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					BodyBlow(const std::shared_ptr<EnemyBase>& owner);

				private:
					vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;

				};
			}
		}

	}
}