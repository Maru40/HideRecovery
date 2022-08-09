
/*!
@file OwnHideItemManager.h
@brief OwnHideItemManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class HidePlace;

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------
	struct OwnHideItemManager_Parametor {
		float searchHidePlaceRange;			//�B���ꏊ�Ɏ肪�͂��͈�
		float putFloorAnimationTime;		//���ɒu���A�j���[�V��������
		float putHideObjectAnimationTime;	//�B���ꏊ�ɒu���A�j���[�V��������

		OwnHideItemManager_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="searchHidePlaceRange">�B���ꏊ�Ɏ肪�͂��͈�</param>
		OwnHideItemManager_Parametor(const float searchHidePlaceRange);
	};

	//--------------------------------------------------------------------------------------
	/// �����Ă���B���A�C�e���̊Ǘ�
	//--------------------------------------------------------------------------------------
	class OwnHideItemManager : public Component 
	{
	public:
		using Parametor = OwnHideItemManager_Parametor;

	private:
		Parametor m_param;					//�p�����[�^
		std::unique_ptr<GameTimer> m_timer;	//���ԊǗ�

		bool m_isFleePut;	//�f�o�b�O�p�̂ǂ��ł��u����t���O

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// �A�C�e����u��
		/// </summary>
		void PutHideItem();

		/// <summary>
		/// �u���t�s��
		/// </summary>
		void BluffPutHideItem();

		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="putEvent">�A�j���[�V�����C�x���g</param>
		void PlayAnimation(const std::function<void()>& putEvent);

		/// <summary>
		/// ��]����
		/// </summary>
		void Rotation();

		/// <summary>
		/// �u���邩�ǂ�����Ԃ�
		/// </summary>
		/// <returns>�u����Ȃ�true</returns>
		bool IsPut() const;

		/// <summary>
		/// �B���ꏊ�̎擾
		/// </summary>
		/// <returns>�B���ꏊ���擾</returns>
		Vec3 CalculateHidePosition() const;

		/// <summary>
		/// �B���ꏊ��T���ĕԂ��B
		/// </summary>
		/// <returns>�B���ꏊ</returns>
		std::shared_ptr<HidePlace> SearchNearHidePlace() const;
	};

}