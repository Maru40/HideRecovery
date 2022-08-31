
/*!
@file BillBoard.h
@brief BillBoard�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�r���{�[�h
	//--------------------------------------------------------------------------------------
	class BillBoard : public Component 
	{
		Vec3 m_rotationOffset;  //�����I�t�Z�b�g
		Vec3 forward;           //���ʕ���

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		BillBoard(const std::shared_ptr<GameObject>& objPtr):
			BillBoard(objPtr, Vec3(0.0f))
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="rotationOffset">�����I�t�Z�b�g</param>
		BillBoard(const std::shared_ptr<GameObject>& objPtr, const Vec3& rotationOffset):
			Component(objPtr), m_rotationOffset(rotationOffset), forward(Vec3(0.0f))
		{}

		void OnUpdate() override;

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �����I�t�Z�b�g�̐ݒ�
		/// </summary>
		/// <param name="offset">�����I�t�Z�b�g</param>
		void SetRotationOffset(const Vec3& offset) noexcept {
			m_rotationOffset = offset;
		}

	};

}

//endbasecross