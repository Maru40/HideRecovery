
/*!
@file SkillBase.h
@brief SkillBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�X�L�����N���X
	//--------------------------------------------------------------------------------------
	class SkillBase : public Component
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}