
/*!
@file ActiveSkillBase.h
@brief ActiveSkillBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�A�N�e�B�u�X�L���N���X
	//--------------------------------------------------------------------------------------
	class ActiveSkillBase : public SkillBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		ActiveSkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}