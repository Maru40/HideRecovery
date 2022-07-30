/*!
@file InvolvedManager.h
@brief �������܂��I�u�W�F�N�g�̏Փ˔���
*/

#pragma once
#include "stdafx.h"
#include "InvolvedObjects.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// �������܂��I�u�W�F�N�g�̏Փ˔���R���|�[�l���g�N���X
	//-----------------------------------------------------------------------------------------------
	class InvolvedManager : public Component
	{
		std::shared_ptr<InvolvedObjects> m_InvolObj; //! ���g�̒ǉ���̃I�u�W�F�N�g
		std::shared_ptr<Transform> m_InvolTrans; //! �ǉ���̃I�u�W�F�N�g�̃g�����X�t�H�[��
		float m_RotationPow = 5.0f; //! ��]�̑��x

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �R���X�g���N�^
		* @param[in] involObj �ǉ���̃I�u�W�F�N�g
		*/
		InvolvedManager(const std::shared_ptr<GameObject>& ptrObj, const std::shared_ptr<InvolvedObjects>& involObj)
			:Component(ptrObj), m_InvolObj(involObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCollisionEnter(const CollisionPair& pair) override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �_���[�W����
		* @param[in] point �Փ˂����ʒu
		*/
		void HitDamage(const Vec3& point);

		/**
		* @brief ��]����
		* @param[in] hitCenter �Փ˂����ʒu
		*/
		void ObjectRotation(const Vec3& hitCenter);
		//-----------------------------------------------------------------------------------------------
	};
}