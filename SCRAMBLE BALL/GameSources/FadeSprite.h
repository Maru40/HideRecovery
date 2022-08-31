/*!
@file FadeSpriteManager.h
@brief FadeSpriteManager�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	class GameTimer;

	//--------------------------------------------------------------------------------------
	/// �t�F�[�h�^�C�v
	//--------------------------------------------------------------------------------------
	enum class FadeType
	{
		In,   //�t�F�[�h�C��
		Out,  //�t�F�[�h�A�E�g
	};

	//--------------------------------------------------------------------------------------
	/// �t�F�[�h�X�s���C�g�Ǘ�
	//--------------------------------------------------------------------------------------
	class FadeSpriteManager : public Component
	{
		ex_weak_ptr<SpriteObject> m_fadeSprite = nullptr; //�t�F�[�h������SpriteObject

		std::function<void()> m_endFunction = nullptr;    //�t�F�[�h�I�����ɌĂяo���C�x���g
		FadeType m_type = FadeType::In;                   //�t�F�[�h�^�C�v

		std::unique_ptr<GameTimer> m_timer = nullptr;     //�^�C�}�[�Ǘ��N���X�B

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="spriteObject">�X�v���C�g�I�u�W�F�N�g</param>
		FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<SpriteObject>& spriteObject);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �t�F�[�h�X�V����
		/// </summary>
		void FadeUpdate();

		/// <summary>
		/// �A���t�@�l���X�V����B
		/// </summary>
		/// <param name="alpha">�X�V�������A���t�@�l</param>
		void UpdateAlpha(const float& alpha);

		/// <summary>
		/// �t�F�[�h�I�����ɌĂяo������
		/// </summary>
		void FadeEnd();

	public:
		/// <summary>
		/// �t�F�[�h�X�^�[�g
		/// </summary>
		/// <param name="type">�t�F�[�h�^�C�v</param>
		/// <param name="endFunction">�t�F�[�h�I�����ɌĂяo���C�x���g</param>
		/// <param name="fadeTime">�t�F�[�h�Ɋ|���鎞��</param>
		void FadeStart(const FadeType& type, const std::function<void()>& endFunction = nullptr, const float& fadeTime = 0.5f);
		
		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�t�F�[�h���I��������true</returns>
		bool IsEnd() const;

	};

}