
/*!
@file ItemAcquisitionManager.h
@brief ItemAcquisitionManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class ItemBase;

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------
	struct ItemAcquisitionManager_Parametor {
		float searchRange;	//�A�C�e�����G�͈�

		ItemAcquisitionManager_Parametor();

		ItemAcquisitionManager_Parametor(const float searchRange);
	};

	//--------------------------------------------------------------------------------------
	/// �A�C�e���l���Ǘ�
	//--------------------------------------------------------------------------------------
	class ItemAcquisitionManager : public Component
	{
	public:
		using Parametor = ItemAcquisitionManager_Parametor;

	private:
		Parametor m_param;	//�p�����[�^

		std::vector<ex_weak_ptr<ItemBase>> m_allFieldItems;		//�t�B�[���h�ɂ���S�ẴA�C�e����z��ɓ����B

	public:
		ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �A�C�e���l���̃{�^���������ꂽ���̏���
		/// </summary>
		void Input_ItemAcquisition();

		/// <summary>
		/// �A�C�e�����l���͈͓��ɂ��邩�ǂ������f
		/// </summary>
		/// <param name="item">�m�F�������A�C�e��</param>
		/// <returns>�l���͈͓��Ȃ�true</returns>
		bool IsAcquisitionRange(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// �I�u�W�F�N�g�̃T�[�`�͈͗p�̃g���K�[�𐶐�����B(�����{�c)
		/// </summary>
		void CreateSerchTriggerObject();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

	};

}