
/*!
@file PassiveSkillBase.h
@brief PassiveSkillBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�p�b�V�u�X�L���N���X
	//--------------------------------------------------------------------------------------
	class PassiveSkillBase : public SkillBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PassiveSkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}