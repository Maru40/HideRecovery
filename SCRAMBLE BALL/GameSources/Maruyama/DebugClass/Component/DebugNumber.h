
/*!
@file Number.h
@brief Number�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�f�o�b�O�p����
		//--------------------------------------------------------------------------------------
		class Number : public Component
		{
		public:
			using DrawComp = PCTStaticDraw;

		private:

			int m_number;		//�\������������
			float m_width;		//����
			float m_height;		//����
			float m_size;		//�T�C�Y
			Col4 m_color;		//�F
			wstring m_texture;	//�e�N�X�`��

			std::vector<VertexPositionColorTexture> m_vertices;	//���_�z��
			std::vector<uint16_t> m_indices;					//�C���f�b�N�X�z��

			/// <summary>
			/// ���b�V���̐���
			/// </summary>
			void CreateMesh();

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="number">�\�����鐔��</param>
			Number(const std::shared_ptr<GameObject>& objPtr, const int& number) :
				Number(objPtr, number, L"Number_TX")
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="number">�\�����鐔��</param>
			/// <param name="texture">�\���Ɏg���e�N�X�`��</param>
			Number(const std::shared_ptr<GameObject>& objPtr, const int& number, const wstring& texture) :
				Component(objPtr), m_number(number),
				m_width(50.0f), m_height(100.0f),
				m_size(1.0f),
				m_color(Col4(1.0f, 1.0f, 1.0f, 0.5f)),
				m_texture(texture)
			{}

			void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �����̐ݒ�
			/// </summary>
			/// <param name="number">�\�����鐔��</param>
			void SetNumber(const int& number);

			/// <summary>
			/// �����̎擾
			/// </summary>
			/// <returns>�\�����鐔��</returns>
			int GetNumber() const noexcept {
				return m_number;
			}

			/// <summary>
			/// �T�C�Y�̐ݒ�
			/// </summary>
			/// <param name="number">�T�C�Y</param>
			void SetSize(const float& size) noexcept {
				m_size = size;
			}

			/// <summary>
			/// �T�C�Y�̎擾
			/// </summary>
			/// <returns>�T�C�Y</returns>
			float GetSize() const noexcept {
				return m_size;
			} 

			/// <summary>
			/// �e�N�X�`���̐ݒ�
			/// </summary>
			/// <param name="texture">�ݒ肵�����e�N�X�`��</param>
			void SetTexture(const wstring& texture);

			/// <summary>
			/// �e�N�X�`���̎擾
			/// </summary>
			/// <returns>�e�N�X�`��</returns>
			wstring GetTexture() const noexcept { return m_texture; }
		};

	}

}

//endbasecross