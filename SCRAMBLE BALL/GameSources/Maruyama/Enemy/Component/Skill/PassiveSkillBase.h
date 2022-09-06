
/*!
@file PassiveSkillBase.h
@brief PassiveSkillBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パッシブスキルクラス
	//--------------------------------------------------------------------------------------
	class PassiveSkillBase : public SkillBase
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PassiveSkillBase(const std::shared_ptr<GameObject>& objPtr);
	};

}