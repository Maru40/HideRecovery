
/*!
@file BillBoard_Ex.h
@brief BillBoard_Ex�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���S�ȃr���{�[�h
	//--------------------------------------------------------------------------------------
	class BillBoard_Ex : public Component
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		BillBoard_Ex(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
	};

}