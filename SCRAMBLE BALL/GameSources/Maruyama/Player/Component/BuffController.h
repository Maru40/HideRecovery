
/*!
@file BuffController.h
@brief BuffControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// バフパラメータ
	//--------------------------------------------------------------------------------------

	struct BuffController_Parametor {
		float speed;
	};

	//--------------------------------------------------------------------------------------
	/// バフのコントローラー
	//--------------------------------------------------------------------------------------
	class BuffController : public Component
	{
	public:
		using Parametor = BuffController_Parametor;

	private:
		Parametor m_param;	//パラメータ

	public:
		BuffController(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;

	public:



	};

}