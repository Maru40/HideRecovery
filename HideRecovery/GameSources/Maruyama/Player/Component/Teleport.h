
/*!
@file Teleport.h
@brief Teleport�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �e���|�[�g�@�\
	//--------------------------------------------------------------------------------------
	class Teleport : public Component
	{
	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

	private:


	public:
		/// <summary>
		/// �}�b�v���J���B
		/// </summary>
		void OpenMap();
	};

}