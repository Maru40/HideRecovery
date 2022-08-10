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
		int numStartOwn;	//�J�n���̏�����
		int numMaxOwn;	//���Ă�ő吔

		ItemBag_Parametor();

		ItemBag_Parametor(const int numStartOwn, const int numMaxOwn);
	};

	//--------------------------------------------------------------------------------------
	/// �A�C�e���o�b�O
	//--------------------------------------------------------------------------------------
	class ItemBag : public Component
	{
	public:
		using Parametor = ItemBag_Parametor;

	private:
		Parametor m_param;							//�p�����[�^

		std::vector<ex_weak_ptr<ItemBase>> m_items;	//�����Ă���A�C�e���ꗗ

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
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

		/// <summary>
		/// �B���A�C�e���̎擾
		/// </summary>
		/// <returns>�B���A�C�e��</returns>
		std::shared_ptr<HideItem> GetHideItem() const;
	};

}