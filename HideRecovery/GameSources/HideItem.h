
/*!
@file HideItem.h
@brief HideItem�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "ItemBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �B���A�C�e��
	//--------------------------------------------------------------------------------------
	class HideItem : public ItemBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		HideItem(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:


	};

}