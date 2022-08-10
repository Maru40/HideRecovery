/*!
@file StageObjectBase.h
@brief StageObjectBase�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"
#include "Watanabe/Utility/TransformData.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�I�u�W�F�N�g�̊��N���X
	//--------------------------------------------------------------------------------------
	class StageObjectBase : public GameObject
	{
	protected:
		wstring m_name;    //�X�e�[�W�I�u�W�F�N�g�l�[��
		wstring m_texture; //�e�N�X�`���l�[��
		TransformData m_transformData; //�g�����X�t�H�[���f�[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="name">�I�u�W�F�N�g��</param>
		/// <param name="texture">�e�N�X�`����</param>
		StageObjectBase(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture = L"");

		virtual void OnPreCreate()override;
		virtual void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�u�W�F�N�g���̎擾
		/// </summary>
		/// <returns>�I�u�W�F�N�g��</returns>
		wstring GetName() const noexcept { return m_name; }

		/// <summary>
		/// �e�N�X�`�����̎擾
		/// </summary>
		/// <returns>�e�N�X�`����</returns>
		wstring GetTexture() const noexcept { return m_texture; }
	};
}

//endbasecross