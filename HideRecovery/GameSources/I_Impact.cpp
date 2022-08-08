
/*!
@file I_Impact.cpp
@brief I_Impactのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "EyeSearchRange.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			dangerValue(1.0f), 
			confirmationIntervalTime(30.0f), 
			isConfirmation(false)
		{}

	}
}