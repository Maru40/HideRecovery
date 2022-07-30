/*!
@file InvolvedObjects.h
@brief �������܂��I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// �^�b�N���Ɋ������܂��I�u�W�F�N�g�N���X
	//-----------------------------------------------------------------------------------------------
	class InvolvedObjects : public StageObjectBase
	{
		Vec3 m_StartPos; //! �����ʒu

		int m_Endurance = 1; //! �Ԃ������Ƃ��ɉ���c��̉�

		Vec2 m_RangeFactor = Vec2(0.5f, 0.5f); //! �_���[�W���󂯂�͈�
		float m_RotationScope = 1.0f; //! ��]���钆�S����̍Œ዗��

		ex_weak_ptr<GameObject> m_ColObj; //! �Ԃ����Ă����Ώ�

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �R���X�g���N�^
		* @param[in] startPosition �����ʒu
		*/
		InvolvedObjects(const std::shared_ptr<Stage>& stage, const Vec3& startPosition)
			:StageObjectBase(stage, L""), m_StartPos(startPosition)
		{}

		/**
		* @brief �R���X�g���N�^
		* @param[in] name �I�u�W�F�N�g�̖��O
		* @param[in] texture �I�u�W�F�N�g�Ɏg�p����e�N�X�`��
		*/
		InvolvedObjects(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture = L"")
			:StageObjectBase(stage, name, texture), m_StartPos(Vec3(0.0f))
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �_���[�W��^����
		* @param[in] damage �^����_���[�W�̑傫��
		*/
		void Damaged(int damage);

		/// �j�󂷂�
		void Breaking();

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------
		/// �_���[�W���󂯂�傫���̎擾
		Vec2 GetRangeFactor() { return m_RangeFactor; }

		/// ��]���邽�߂̍Œ዗�����擾
		float GetRotationScope() { return m_RotationScope; }
		//-----------------------------------------------------------------------------------------------
	};
}

//endbasecross