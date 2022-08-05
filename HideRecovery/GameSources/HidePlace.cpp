
/*!
@file HidePlace.h
@brief HidePlaceΘΗ
SFΫRTμ
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlace.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// p[^
	//--------------------------------------------------------------------------------------

	HidePlace_Parametor::HidePlace_Parametor():
		HidePlace_Parametor(Vec3(0.0f))
	{}

	HidePlace_Parametor::HidePlace_Parametor(const Vec3& hidePositionOffset):
		hidePositionOffset(hidePositionOffset)
	{}

	//--------------------------------------------------------------------------------------
	/// B·κ
	//--------------------------------------------------------------------------------------

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr) :
		HidePlace(objPtr, Parametor())
	{}

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr), m_param(parametor)
	{}

}