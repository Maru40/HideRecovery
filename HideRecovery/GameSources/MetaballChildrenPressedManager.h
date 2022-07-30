/*!
@file MetaballChildrenPressedManager.h
@brief MetaballChildrenPressedManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct PressData;

	namespace maru {
		enum class DeltaType;
	}

	namespace Metaball {

		class RenderBase;

		//--------------------------------------------------------------------------------------
		///	�ׂ����Ǘ��̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct ChildrenPressedManager_Parametor 
		{
			float stopCrashPower;       //�q�b�g�X�g�b�v���ׂ̒���
			float slowCrashPower;       //�X���[���ׂ̒���
			float stopAfterDestroyTime; //�ׂ��I�������A�����܂ł̃C���^�[�o������

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ChildrenPressedManager_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stopCrashPower">�q�b�g�X�g�b�v���ׂ̒���</param>
			/// <param name="slowCrashPower">�X���[���ׂ̒���</param>
			/// <param name="stopAfterDestroyTime">�q�b�g�X�g�b�v��ɉ��b��ɏ������邩</param>
			ChildrenPressedManager_Parametor(
				const float& stopCrashPower, 
				const float& slowCrashPower, 
				const float& stopAfterDestroyTime
			);
		};

		//--------------------------------------------------------------------------------------
		///	�ׂ����Ǘ�
		//--------------------------------------------------------------------------------------
		class ChildrenPressedManager : public Component
		{
		public:
			using Parametor = ChildrenPressedManager_Parametor;

		private:
			Parametor m_param;	//�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override {};

		private:
			/// <summary>
			/// //�X���[�v���X
			/// </summary>
			/// <param name="pressData">�ׂ��f�[�^</param>
			void SlowTimerPress(const PressData& pressData);

			/// <summary>
			/// �q�b�g�X�g�b�v�v���X
			/// </summary>
			/// <param name="pressData">�ׂ��f�[�^</param>
			void HitStopPress(const PressData& pressData);   

			/// <summary>
			/// SE�Đ�
			/// </summary>
			/// <param name="pressData">�ׂ��f�[�^</param>
			/// <param name="pressingTime">�ׂ�����</param>
			/// <param name="pressedTime">�ׂ���鎞��</param>
			/// <param name="updateTime">�X�V���x</param>
			/// <param name="deltaType">�X�V���ԃ^�C�v</param>
			void PlayOneShot(const PressData& pressData,
				const float& pressingTime, const float& pressedTime,
				const float& updateTime, const maru::DeltaType& deltaType);

		public:
			/// <summary>
			/// SlimeChild�̒Ǐ]�������ȂǁA�ׂ��ꂽ���ɌĂяo������
			/// </summary>
			/// <param name="pressData">�ׂ��f�[�^</param>
			void PressCrash(const PressData& pressData);

			/// <summary>
			/// �ׂ���鎞�Ԃ̎擾
			/// </summary>
			/// <returns>�ׂ���鎞��</returns>
			float GetPressedTime() const;

			/// <summary>
			/// �ׂ����͂̎擾
			/// </summary>
			/// <returns>�ׂ�����</returns>
			float GetPressedPower() const;

		};

	}

}

//endbasecross