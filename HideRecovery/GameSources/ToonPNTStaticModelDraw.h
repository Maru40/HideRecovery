#pragma once
/*!
@file ToonPNTStaticModelDraw.h
@brief ToonPNTStaticModelDraw
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �g�D�[�����f���`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class ToonPNTStaticModelDraw : public BcBaseDraw
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
		ToonPNTStaticModelDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// ���f���̃h���[
		/// </summary>
		/// <param name="data">���b�V���f�[�^</param>
		void DrawModel2(const MeshPrimData& data);

		/// <summary>
		/// �����v�e�N�X�`�����\�[�X�̐ݒ�
		/// </summary>
		/// <param name="textureResource">�e�N�X�`�����\�[�X</param>
		void SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource);

	public:
		/// <summary>
		/// �����v�e�N�X�`�����\�[�X�̐ݒ�
		/// </summary>
		/// <param name="key">�e�N�X�`����</param>
		void SetRampTextureResource(const wstring& key);

		/// <summary>
		/// �e�N�X�`�����\�[�X�̎擾
		/// </summary>
		/// <returns>�e�N�X�`�����\�[�X</returns>
		std::shared_ptr<TextureResource> GetRampTextureResource() const;

	};

}