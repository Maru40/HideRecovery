/*!
@file I_Damaged.h
@brief I_Damagedなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// ダメージデータ
	//--------------------------------------------------------------------------------------
	struct DamageData
	{
		int value;      //ダメージ値
		std::shared_ptr<GameObject> attacker;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		DamageData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="value">ダメージ値</param>
		/// <param name="attacker">攻撃をしたオブジェクト</param>
		DamageData(const int& value, const std::shared_ptr<GameObject>& attacker);

		DamageData operator+(const DamageData& data);

		DamageData operator+=(const DamageData& data);
	};

	class I_Damaged
	{
	public:
		/// <summary>
		/// ダメージを受ける処理
		/// </summary>
		/// <param name="data">ダメージデータ</param>
		virtual void Damaged(const DamageData& data) = 0;
	};
}

//endbasecorss