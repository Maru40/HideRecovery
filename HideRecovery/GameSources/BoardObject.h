
/*!
@file BoardObject.h
@brief BoardObj�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// �I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BoardObject : public GameObject
	{
	public:
		using DrawComp = BcPCTStaticDraw;

	private:
		std::shared_ptr<Builder::VertexPCTParametor> m_paramPtr;  //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="param">���_�f�[�^�����p�p�����[�^</param>
		BoardObject(const std::shared_ptr<Stage>& stage,
			const Builder::VertexPCTParametor& param
		);

		virtual void OnCreate() override;

	protected:
		/// <summary>
		/// ���_�f�[�^�����p�p�����[�^�̎擾
		/// </summary>
		/// <returns>���_�f�[�^�����p�p�����[�^</returns>
		const std::shared_ptr<const Builder::VertexPCTParametor> GetVertexPCTParametor() const;

		/// <summary>
		/// �f�B�t���[�Y�J���[�̐ݒ�
		/// </summary>
		/// <param name="color">�f�B�t���[�Y�J���[</param>
		void SetDiffuse(const Col4& color);

		/// <summary>
		/// �f�B�t���[�Y�J���[�̎擾
		/// </summary>
		/// <returns>�f�B�t���[�Y�J���[</returns>
		Col4 GetDiffuse() const;

	};

}

//basecross