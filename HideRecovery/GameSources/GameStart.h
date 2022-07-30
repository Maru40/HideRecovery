/*!
@file GameStart.h
@brief �J�n���o
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// �Q�[���X�^�[�g���̉��o�N���X
	//-----------------------------------------------------------------------------------------------
	class GameStart : public GameObject
	{
		float m_ReadyMaxPosY = 500.0f; //! Ready�̏I�_�ʒu
		float m_ReadyMoveSpeed = 500.0f; //! Ready�̓�������

		Vec3 m_GoMaxScale = Vec3(2.0f, 2.0f, 1.0f); //! Go�̍ő�̑傫��
		float m_GoGrowthSpeed = 12.0f; //! Go�̑傫���Ȃ鑬��

		std::shared_ptr<GameObject> m_Ready; //! Ready�̔|��
		std::shared_ptr<GameObject> m_Go; //! Go�̔|��
		std::shared_ptr<Transform> m_ReadyTrans; //! Ready�̃g�����X�t�H�[��
		std::shared_ptr<Transform> m_GoTrans; //! Go�̃g�����X�t�H�[��
		std::shared_ptr<Component> m_TimerComp; //! Go�̃g�����X�t�H�[��

	public:
		//-----------------------------------------------------------------------------------------------
		/// �R���X�g���N�^
		GameStart::GameStart(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/// Ready�𓮂�������
		void Ready();

		/// Go�𓮂�������
		void Go();

		/**
		* @brief �X�^�[�g���o�̏I������
		* @datalis ��~���Ă����I�u�W�F�N�g�𓮂������g������
		*/
		void End();
		//-----------------------------------------------------------------------------------------------
	};
}
