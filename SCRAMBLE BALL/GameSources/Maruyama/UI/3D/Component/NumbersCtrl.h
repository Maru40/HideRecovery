
/*!
@file NumbersCtrl.h
@brief NumbersCtrl�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "../Object/NumberObject.h"

namespace basecross {

	class NumbersCtrl : public Component 
	{
		float m_value = 0;								//�\������l
		int m_maxDigit = 3;								//�ő包��
		wstring m_texture = L"NumberWhite_TX";			//�����\���Ɏg���e�N�X�`��

		vector<ex_weak_ptr<NumberObject>> m_numberObjs;	//���������i���o�[�I�u�W�F�N�g

		/// <summary>
		/// �\�����鐔�����I�u�W�F�N�g�𐶐�����
		/// </summary>
		void CreateNumberObjs();

		/// <summary>
		/// ����Ȃ�0�͔�\���ɂ���B
		/// </summary>
		void ChangeDrawNumbers();

		/// <summary>
		/// �i���o�[�̃|�W�V�������Z�b�g����B
		/// </summary>
		void SetNumbersPosition();

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="texture">�����\���\�̃e�N�X�`��</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr, const wstring& texture):
			Component(objPtr), m_texture(texture)
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="maxDigit">�ő包��</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr, const int& maxDigit)
			:Component(objPtr),m_maxDigit(maxDigit)
		{}

		void OnCreate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// ���������̕ύX
		/// </summary>
		/// <param name="number">�ύX����������</param>
		void ChangeIntegerNumber(const int value);

		/// <summary>
		/// �����_�����̕\����ύX
		/// </summary>
		/// <param name="value">�ύX����������</param>
		void ChangeFloatNumber(const float value);

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �\������l�̐ݒ�
		/// </summary>
		/// <param name="value">�\������l</param>
		void SetValue(const float value);

		/// <summary>
		/// �\�����鐔���̎擾
		/// </summary>
		/// <returns>�\�����鐔��</returns>
		float GetValue() const noexcept;

		/// <summary>
		/// �F�̐ݒ�
		/// </summary>
		/// <param name="color">�ݒ肷��F</param>
		void SetColor(const Col4& color);

		/// <summary>
		/// �e�N�X�`���̐ݒ�
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		void SetTexture(const wstring& texture);

		/// <summary>
		///	�e�N�X�`���̎擾
		/// </summary>
		/// <returns>�e�N�X�`��</returns>
		wstring GetTexture() const noexcept { return m_texture; }
	};

}

//endbasecross