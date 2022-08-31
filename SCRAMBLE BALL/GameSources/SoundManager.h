/*!
@file SoundManager.h
@brief SoundManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	using namespace maru;

	//--------------------------------------------------------------------------------------
	/// �T�E���h�}�l�[�W���[
	//--------------------------------------------------------------------------------------
	class SoundManager : public SingletonComponent<SoundManager>
	{
	public:
		enum class ID {
			SlimePressing, //�X���C����ׂ��Ă����
			SlimePressed,  //�X���C����ׂ�
			ClearSE,       
			BackSE,       
			NextSE,        
			CursorSE,
			EnterSE,

			TitleBGM,
			SelectBGM,
			GameBGM,

			PlayerWalk,    //Player�̈ړ�
			PlayerTackle,  //Player�̃^�b�N��
			JettPomp,      //�W�F�b�g�|���v

			ShotSE,
		};

	private:
		map<ID, wstring> m_soundMap;             //�T�E���h�}�b�v
		map<ID, ex_weak_ptr<SoundItem>> m_items; //�T�E���h�A�C�e���z��

		float m_baseVolume;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SoundManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �T�E���h�}�b�v�̃Z�b�e�B���O
		/// </summary>
		void SettingSoundMap();

		/// <summary>
		/// �T�E���h�A�C�e���̒ǉ�
		/// </summary>
		/// <param name="id">�T�E���hID</param>
		/// <param name="item">�T�E���h�A�C�e��</param>
		void AddSoundItem(const ID& id, const std::shared_ptr<SoundItem>& item);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ��x�炷
		/// </summary>
		/// <param name="id">�T�E���hID</param>
		/// <param name="volume">�{�����[��</param>
		/// <param name="loopCount">���[�v��</param>
		/// <returns>���������T�E���h�A�C�e��</returns>
		std::shared_ptr<SoundItem> PlayOneShot(const ID& id, const float& volume = 0.5f, const float& loopCount = 0.0f);

		/// <summary>
		/// BGM��炷
		/// </summary>
		/// <param name="id">�T�E���hID</param>
		/// <param name="volume">�{�����[��</param>
		/// <param name="LoopCount">���[�v��</param>
		/// <returns>���������T�E���h�A�C�e��</returns>
		std::shared_ptr<SoundItem> PlayBGM(const ID& id, const float& volume = 0.5f, const float& LoopCount = XAUDIO2_LOOP_INFINITE);

		/// <summary>
		/// �w�肵���T�E���h���~�߂�B
		/// </summary>
		/// <param name="id">�T�E���h�A�C�e��</param>
		void Stop(const ID& id);

		/// <summary>
		/// �S�ẴT�E���h���~�߂�B
		/// </summary>
		void AllStop();

	};

}

//endbasecross