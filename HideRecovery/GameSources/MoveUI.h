/*!
@file MoveUI.h
@brief MoveUI�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "MoveUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// UI�̈ʒu�𒲐����������ɗ��p����N���X
	//--------------------------------------------------------------------------------------
	class MoveUI : public Component
	{
		float m_speed;  //�ړ��X�s�[�h

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		MoveUI(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate();
	};

}