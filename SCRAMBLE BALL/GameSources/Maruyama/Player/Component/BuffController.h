
/*!
@file BuffController.h
@brief BuffController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �o�t�p�����[�^
	//--------------------------------------------------------------------------------------

	struct BuffController_Parametor {
		float speed;
	};

	//--------------------------------------------------------------------------------------
	/// �o�t�̃R���g���[���[
	//--------------------------------------------------------------------------------------
	class BuffController : public Component
	{
	public:
		using Parametor = BuffController_Parametor;

	private:
		Parametor m_param;	//�p�����[�^

	public:
		BuffController(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;

	public:



	};

}