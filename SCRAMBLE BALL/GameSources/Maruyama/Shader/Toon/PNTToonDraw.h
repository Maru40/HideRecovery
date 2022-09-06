/*!
@file PNTToonDraw.h
@brief PNTToonDraw
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �g�D�[���`��R���|�\�l���g
	//--------------------------------------------------------------------------------------
	class PNTToonDraw : public BcBaseDraw
	{
		//�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

		std::shared_ptr<TextureResource> m_rampTextureResource = nullptr; //�����v�e�N�X�`�����\�[�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PNTToonDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="data">���b�V���f�[�^</param>
		void DrawStatic2(const MeshPrimData& data);

		/// <summary>
		/// �����v�e�N�X�`���̐ݒ�
		/// </summary>
		/// <param name="textureResource">�����v�e�N�X�`�����\�[�X</param>
		void SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource);

	public:
		/// <summary>
		/// �����v�e�N�X�`���̐ݒ�
		/// </summary>
		/// <param name="key">�ݒ肵�����e�N�X�`����</param>
		void SetRampTextureResource(const wstring& key);

		/// <summary>
		/// �����v�e�N�X�`�����\�[�X�̎擾
		/// </summary>
		/// <returns>�����v�e�N�X�`�����\�[�X</returns>
		std::shared_ptr<TextureResource> GetRampTextureResource() const;

	};

}