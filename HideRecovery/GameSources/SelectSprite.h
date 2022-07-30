/*!
@file SelectSprite.h
@brief SelectSprite�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �I���ł���X�v���C�g
	//--------------------------------------------------------------------------------------
	class SelectSprite : public Component
	{
	private:
		//�I�𒆃A�N�V����
		maru::Action<void()> m_selectAction;
		//�I������
		maru::Action<void()> m_unSelectAction;
		//���肵���Ƃ��̃A�N�V����
		maru::Action<void()> m_decisionAction;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SelectSprite(const std::shared_ptr<GameObject>& objPtr);
		
	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�����̃C�x���g��o�^
		/// </summary>
		/// <param name="isSelect">�I�����������A�I��������������</param>
		/// <param name="function">�Ăяo�������C�x���g</param>
		void AddSelectAction(const bool isSelect, const std::function<void()>& function);

		/// <summary>
		/// �o�^�����C�x���g���Ăяo���B
		/// </summary>
		/// <param name="isSelect">�I�����������A�I��������������</param>
		void SelectInvoke(const bool isSelect);

		/// <summary>
		/// ���莞�ɌĂяo�������C�x���g
		/// </summary>
		/// <param name="function">�Ăяo���C�x���g</param>
		void AddDecisionAction(const std::function<void()>& function);

		/// <summary>
		/// �o�^�������莞�ɌĂԃC�x���g���Ăяo���B
		/// </summary>
		void DecisionInvoke();

	};

}