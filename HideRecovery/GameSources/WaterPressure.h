/*!
@file WaterPressure.h
@brief �W�F�b�g�|���v�Ȃ�
�S���F����u��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// �����W�F�b�g�̒e�N���X
	//-----------------------------------------------------------------------------------------------
	class WaterPressure : public GameObject
	{
		Vec3 m_StartPos = Vec3(0); //! ���˂̏����ʒu
		Vec3 m_MoveDir = Vec3(0); //! ��������

		float m_MovePow = 30.0f; //! ��������
		float m_PushPower = 10.0f; //! ������������

		float m_Weight = 1.0f; //! ����
		float m_Height = 1.0f; //! �c��

		ex_weak_ptr<GameObject> m_aura; //! �G�t�F�N�g�̃|�C���^

	public:
		//-----------------------------------------------------------------------------------------------
		/// ���������R���X�g���N�^
		WaterPressure(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		/**
		* @brief �R���X�g���N�^
		* @param[in] startPos �����ʒu
		* @param[in] moveDirection ��������
		*/
		WaterPressure(const std::shared_ptr<Stage>& stage, const Vec3& startPos, Vec3 moveDirection)
			:GameObject(stage), m_StartPos(startPos), m_MoveDir(moveDirection.normalize())
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		//-----------------------------------------------------------------------------------------------
		/// �ʒu�̈ړ�
		void Move();

		/// �J�����̕����Ɍ�������
		void CapturedCamera();

		///���g������		
		void Erasure();

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------
		/// �������������̎擾
		float GetPushPower() { return m_PushPower; }
		//-----------------------------------------------------------------------------------------------
	};
}
//endbasecross