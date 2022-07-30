/*!
@file CrushScore.h
@brief �ׂ������̐�������
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// �X���C������肭�Ԃ������̃����N�\���N���X
	//-----------------------------------------------------------------------------------------------
	class CrushScore : public GameObject
	{
		Vec3 m_StartPos = Vec3(500.0f, 0.0f, 0.0f); //! �����ʒu
		Vec3 m_MovePower = Vec3(0.0f, 230.0f, 0.0f);
		std::list<std::shared_ptr<GameObject>> m_ScoreUi; //! �X�R�A�\����UI

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �R���X�g���N�^
		*/
		CrushScore::CrushScore(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief UI�̒ǉ�
		* @param[in] texture �g�p����e�N�X�`��
		*/
		void DeployUi(const wstring& texture); //Ui�z�u

		/// UI�̈ړ�
		void MoveUi();

		/**
		* @brief UI�̃t�F�[�h�A�E�g
		* @param[in,out] UI �Ώۂ�UI�|�C���^
		* @param[in] position UI�̈ʒu
		* @param[in] delta �f���^����
		*/
		void FadeOut(const std::shared_ptr<GameObject>& UI, const Vec3& position, float delta);
		//-----------------------------------------------------------------------------------------------
	};
}
