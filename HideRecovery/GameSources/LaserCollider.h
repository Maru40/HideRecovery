
/*!
@file LaserCollider.h
@brief LaserCollider�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���[�U�[�p�̃R���C�_�[
	//--------------------------------------------------------------------------------------
	class LaserCollider : public Component
	{
		ex_weak_ptr<GameObject> m_collisionObject;  //�R���C�_�[�I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		LaserCollider(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		/// <summary>
		/// �R���C�_�[��`�悷�邩�ݒ�
		/// </summary>
		/// <param name="isDraw">�`�悷�邩�ǂ���</param>
		void SetColliderDrawActive(const bool isDraw);

	private:
		/// <summary>
		/// �����X�V
		/// </summary>
		void HeightUpdate();
	};

}