/*!
@file StageSelectSpriteManager.h
@brief StageSelectSpriteManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SelectSprite;
	class SelecterSprite;

	//--------------------------------------------------------------------------------------
	/// �X�e�[�W�Z���N�g�X�v���C�g�Ǘ��̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct StageSelectSpriteManager_Parametor
	{
		float moveRange;  //�ړ�����
		float moveSpeed;  //�ړ��X�s�[�h
		Vec3 moveDirect;  //�ړ�����

		StageSelectSpriteManager_Parametor();
		StageSelectSpriteManager_Parametor(const float& moveRange, const float& moveSpeed, const Vec3& moveDirect);

	};

	//--------------------------------------------------------------------------------------
	/// �X�e�[�W�Z���N�g�X�v���C�g�Ǘ�
	//--------------------------------------------------------------------------------------
	class StageSelectSpriteManager : public Component
	{
	public:
		using Parametor = StageSelectSpriteManager_Parametor;

	private:
		Parametor m_param;                                      //�p�����[�^
		std::vector<ex_weak_ptr<SelectSprite>> m_selectSprites; //�I���ł���X�v���C�g

		ex_weak_ptr<SelectSprite> m_LBSelect = nullptr;         //LB�Z���N�g�X�v���C�g
		ex_weak_ptr<SelectSprite> m_RBSelect = nullptr;         //RB�Z���N�g�X�v���C�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		StageSelectSpriteManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �X�e�[�W�Z���N�g�p�̃X�v���C�g�̐���
		/// </summary>
		void CreateStageSelectSprites();

		/// <summary>
		/// RB,LB�X�v���C�g�̐���
		/// </summary>
		void CreateRBAndLB();

		/// <summary>
		/// �Z���N�^�[�X�v���C�g�̐���
		/// </summary>
		void CreateSelecterSprite();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ړ��J�n
		/// </summary>
		/// <param name="direct">�ړ�����</param>
		void StartMove(const Vec3& direct);
	};

}