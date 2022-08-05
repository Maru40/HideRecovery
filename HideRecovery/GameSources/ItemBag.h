/*!
@file ItemBag.h
@brief ItemBag�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class ItemBase;
	class HideItem;

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------
	struct ItemBag_Parametor {
		uint32 numMaxOwn;	//���Ă�ő吔

		ItemBag_Parametor();

		ItemBag_Parametor(const uint32 numMaxOwn);
	};

	//--------------------------------------------------------------------------------------
	/// �A�C�e���o�b�O
	//--------------------------------------------------------------------------------------
	class ItemBag : public Component
	{
	public:
		using Parametor = ItemBag_Parametor;

	private:
		Parametor m_param;

		std::vector<ex_weak_ptr<ItemBase>> m_items;	//�����Ă���A�C�e���ꗗ

	public:
		ItemBag(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �A�C�e���̒ǉ�
		/// </summary>
		/// <param name="item">�ǉ��������A�C�e��</param>
		void AddItem(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// �A�C�e���̍폜
		/// </summary>
		/// <param name="item">�폜�������A�C�e��</param>
		void RemoveItem(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// �����œn���ꂽ�A�C�e�����擾�ł��邩�ǂ���
		/// </summary>
		/// <param name="item">��ɓ��ꂽ���A�C�e��</param>
		/// <returns>�擾�ł���Ȃ�true</returns>
		bool IsAcquisition(const std::shared_ptr<ItemBase>& item);

		std::shared_ptr<HideItem> GetHideItem() const;
	};

}