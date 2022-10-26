
/*!
@file AccessHidePlace.h
@brief AccessHidePlace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HidePlace;

	class AccessHidePlace : public Component
	{
		float m_range;				//�߂��ɗ����Ɣ��f���鋗��
		bool m_isUpdateDrawBoxUI;	//�{�b�N�X��UI�X�V�����邩�ǂ���

		std::vector<std::weak_ptr<HidePlace>> m_allHidePlaces;	//�t�B�[���h�̑S�ẲB���I�u�W�F�N�g

	public:
		AccessHidePlace(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void UpdateDrawBoxUI();

	public:
		void Access();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �߂��ɗ����Ɣ��f���鋗���̐ݒ�
		/// </summary>
		/// <param name="range">�߂��ɗ����Ɣ��f���鋗��</param>
		void SetRange(const float range) noexcept { m_range = range; }

		/// <summary>
		/// �߂��ɗ����Ɣ��f���鋗�����擾
		/// </summary>
		/// <returns>�߂��ɗ����Ɣ��f���鋗��</returns>
		float GetRange() const noexcept { return m_range; }

		/// <summary>
		/// UI�̕\�����X�V���邩�ǂ�����ݒ�
		/// </summary>
		/// <param name="isUpdate">UI�̕\�����X�V���邩�ǂ���</param>
		void SetIsUpdateDrawBoxUI(const bool isUpdate) noexcept { m_isUpdateDrawBoxUI = isUpdate; }

		/// <summary>
		/// UI�̍X�V�����邩�ǂ������擾
		/// </summary>
		/// <returns>UI���X�V����Ȃ�true</returns>
		bool IsUpdateDrawBoxUI() const noexcept { return m_isUpdateDrawBoxUI; }
	};

}