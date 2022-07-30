/*!
@file SlimeRenderTarget.h
@brief SlimeRenderTarget�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �X���C���̃����_�[�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	class SlimeRenderTarget : public RenderTarget
	{
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="slimeDrawDimension">�\�����@</param>
		explicit SlimeRenderTarget(float slimeDrawDimension = 4096.0f);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~SlimeRenderTarget();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g�r���[�𓾂�
		@return	�����_�����O�^�[�Q�b�g�r���[�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11ShaderResourceView* GetShaderResourceView() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���e�N�X�`���𓾂�
		@return	�f�v�X�X�e���V���e�N�X�`���C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Texture2D* GetDepthStencil() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���r���[�𓾂�
		@return	�f�v�X�X�e���V���r���[�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilView* GetDepthStencilView() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���N���A���鉼�z�֐��i�X�N���[���S�̂��N���A����j
		@param[in]	col	�N���A�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���J�n����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartRenderTarget()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���I������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndRenderTarget()override;
	};

}