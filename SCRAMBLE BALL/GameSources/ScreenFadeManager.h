/*!
@file ScreenFadeManager.h
@brief ScreenFadeManager�w�b�_�@
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

	class FadeSpriteManager;
	enum class FadeType;

	//--------------------------------------------------------------------------------------
	/// �X�N���[���t�F�[�h�Ǘ�
	//--------------------------------------------------------------------------------------
	class ScreenFadeManager : public maru::SingletonComponent<ScreenFadeManager>
	{
		ex_weak_ptr<FadeSpriteManager> m_fadeSpriteManager; //�t�F�[�h�Ǘ�

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="fadeSpriteManager">�t�F�[�h�X�v���C�g�Ǘ��N���X</param>
		ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<FadeSpriteManager>& fadeSpriteManager);

		void OnCreate() override;

		/// <summary>
		/// �t�F�[�h�X�^�[�g
		/// </summary>
		/// <param name="type">�t�F�[�h�^�C�v</param>
		/// <param name="endFunction">�t�F�[�h�I�����C�x���g</param>
		/// <param name="fadeTime">�t�F�[�h�ɂ����鎞��</param>
		void FadeStart(const FadeType& type, const std::function<void()>& endFunction = nullptr, const float& fadeTime = 0.5f);

		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�t�F�[�h���I�����Ă���Ȃ�true</returns>
		bool IsEnd() const;

	};

}