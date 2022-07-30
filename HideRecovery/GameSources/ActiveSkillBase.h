
/*!
@file ActiveSkillBase.h
@brief ActiveSkillBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	アクティブスキルクラス
	//--------------------------------------------------------------------------------------
	class ActiveSkillBase : public SkillBase
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ActiveSkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}