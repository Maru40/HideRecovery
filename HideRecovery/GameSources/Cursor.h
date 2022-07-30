/*!
@file Cursor.h
@brief �J�[�\���Ȃ�
*/

#pragma once
#include "stdafx.h"

#include "PolygonTexture.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// �J�[�\���ŊǗ�����UI�̃f�[�^�\����
	//------------------------------------------------------------------------------------------------
	struct CursorUIData
	{
		UiData uiData; //! �\������|���̏��
		std::function<void()> func; //! �I���������Ɏ��s����֐�
		bool isSelectable; //! �I���\���ǂ���

		//------------------------------------------------------------------------------------------------
		/**
		* @brief ���s����֐��������Ȃ�UI�̃R���X�g���N�^
		* @param[in] data �\������|���̏��
		*/
		CursorUIData::CursorUIData(const UiData& data)
			: uiData(data), isSelectable(false)
		{}

		/**
		* @brief ���s����֐�������UI�̃R���X�g���N�^
		* @param[in] data �\������|���̏��
		* @param[in] func �I���������Ɏ��s����֐�
		* @param[in] isSelectable �I���\���ǂ���
		*/
		CursorUIData::CursorUIData(const UiData& data, std::function<void()> func, bool isSelectable)
			: uiData(data), func(func), isSelectable(isSelectable)
		{}
		//------------------------------------------------------------------------------------------------
	};

	//------------------------------------------------------------------------------------------------
	///�@�J�[�\���I�u�W�F�N�g�N���X
	//------------------------------------------------------------------------------------------------
	class Cursor : public GameObject
	{
		std::vector<std::shared_ptr<PolygonTexture>> m_cursorObj; //! �z�u����Ă���UI�I�u�W�F�N�g�̃|�C���^
		std::vector<CursorUIData> m_UiObj; //! UI�̃f�[�^

		int m_CursorPos = 0; //! �J�[�\�����w���Ă��邳��
		float m_PositionOffset = 23.0f; //! UI���w���Ƃ��̈ʒu�̃I�t�Z�b�g

	public:
		//-----------------------------------------------------------------------------------------------
		/// �R���X�g���N�^
		Cursor::Cursor(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �z�u����UI�̒ǉ�
		* @param[in] data �z�u����UI�̃f�[�^
		*/
		void AddUiTexture(const CursorUIData& data);

		/// �z�u����Ă���Ui��S�č폜
		void ClearUITexture();

		/**
		* @brief �I���������̊֐����s
		*/
		void Select();

		/**
		* @brief �J�[�\���̈ړ�
		* @param[in] �ǂ����̕����ɂǂꂾ����������
		*/
		void MoveCursor(int pos);

		/**
		* @brief �J�[�\�����I���\��UI���w���Ă��Ȃ��Ƃ��Ɉړ�������
		* @param pos �ǂ��������ɖ߂���
		*/
		void PositionCorrection(int pos);

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �J�[�\����UI�̕`���Ԃ̐ݒ�
		* @param[in] value �`����(�`�悷��Ȃ�true)
		*/
		void SetAllDrawActive(bool value)
		{
			SetDrawActive(value); // �J�[�\���̕`���Ԃ�ݒ�
			for (auto obj : m_cursorObj)
			{
				obj->SetDrawActive(value); // UI�̕`���Ԃ�ݒ�
			}
		}

		/**
		* @brief �J�[�\����UI�̕\�����C���[�̐ݒ�
		* @param[in] layer �\�����郌�C���[�ԍ�
		*/
		void SetAllDrawLayer(int layer)
		{
			SetDrawLayer(layer);
			for (auto data : m_cursorObj)
			{
				data->SetDrawLayer(layer);
			}
		}

		/**
		* @brief �J�[�\���̎w��������̐ݒ�
		* @param[in] position �w��UI�̔z��ԍ�
		*/
		void SetCursorPos(int position)
		{
			m_CursorPos = position;
			MoveCursor(1);
		}
		//-----------------------------------------------------------------------------------------------
	};
}
