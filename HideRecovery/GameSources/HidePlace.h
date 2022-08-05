
/*!
@file HidePlace.h
@brief HidePlaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------
	struct HidePlace_Parametor {
		Vec3 hidePositionOffset;	//隠す場所のオフセット

		HidePlace_Parametor();

		HidePlace_Parametor(const Vec3& hidePositionOffset);
	};

	//--------------------------------------------------------------------------------------
	/// 隠す場所
	//--------------------------------------------------------------------------------------
	class HidePlace : public Component
	{
	public:
		using Parametor = HidePlace_Parametor;

	private:
		Parametor m_param = Parametor();

	public:
		HidePlace(const std::shared_ptr<GameObject>& objPtr);

		HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 隠す場所の取得
		/// </summary>
		/// <returns>隠す場所</returns>
		Vec3 GetHidePosition() const { return transform->GetPosition() + m_param.hidePositionOffset; }
	};

}