
/*!
@file Teleport.h
@brief Teleportなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// テレポート機能
	//--------------------------------------------------------------------------------------
	class Teleport : public Component
	{
	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

	private:


	public:
		/// <summary>
		/// マップを開く。
		/// </summary>
		void OpenMap();
	};

}