
/*!
@file SlimeBase.h
@brief SlimeBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyBase.h"

#include "I_Pressed.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct PushData;

	namespace Metaball {
		class ChildrenRender;
		class RenderBase;
	}

	namespace Enemy {

		namespace Motion {
			struct KnockBack_Parametor;
		}

		//--------------------------------------------------------------------------------------
		/// �X���C���̊��N���X
		//--------------------------------------------------------------------------------------
		class SlimeBase : public EnemyBase, public I_Pressed, public I_PushedWater
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SlimeBase(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;

		protected:
			/// <summary>
			/// �g�U���鑬�x���v�Z���ĕԂ�
			/// </summary>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="childObject">���^�{�[���̈��</param>
			/// <param name="pressData">�ׂ��f�[�^</param>
			/// <returns>�g�U���鑬�x</returns>
			Vec3 CalculateSpreadVelocity(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& childObject,
				const PressData& pressData);

			/// <summary>
			/// �g�U�����̗͂�Child�ɉ�����B
			/// </summary>
			/// <param name="render">���^�{�[�������_�[�R���|�[�l���g</param>
			/// <param name="child">���^�{�[���̑S�Ă��Ǘ�����R���|�[�l���g</param>
			/// <param name="pressData">�ׂ���</param>
			void AddSpreadPower(
				const std::shared_ptr<Metaball::RenderBase>& render,
				const std::shared_ptr<Metaball::ChildrenRender>& child,
				const PressData& pressData
			);

		public:
			//--------------------------------------------------------------------------------------
			/// �C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			virtual PressedEnergy Pressed(const PressData& pressData) override;

			virtual void Push(const PushData& data) override;

			virtual void Spread(const PressData& data) override;

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �ׂ�鎞�̃A�j���[�V�����Đ��p�̃f�[�^���擾����B
			/// </summary>
			/// <param name="pressData">�ׂ��f�[�^</param>
			/// <returns>�ׂ�鎞�̃A�j���[�V�����Đ��p�̃f�[�^</returns>
			virtual Motion::KnockBack_Parametor GetPressedMotionData(const PressData& pressData);
		};

	}
}

//endbasecross