
/*!
@file SkillBase.h
@brief SkillBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	スキル基底クラス
	//--------------------------------------------------------------------------------------
	class SkillBase : public Component
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}