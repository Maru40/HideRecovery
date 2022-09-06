
/*!
@file WeponBase.cpp
@brief WeponBaseクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "WeponBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ウェポンのパラメータ
	//--------------------------------------------------------------------------------------

	WeponBase_Parametor::WeponBase_Parametor() :
		weight(4.5f),
		shotIntervalTime(0.2f),
		bulletInstanceOffset(Vec3(0.0f))
	{}

}