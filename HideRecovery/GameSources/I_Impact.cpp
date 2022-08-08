
/*!
@file I_Impact.cpp
@brief I_ImpactÌNXÀÌ
SFÛRTì
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "EyeSearchRange.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	e¿f[^
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			dangerValue(1.0f), 
			occupancyValue(0.5f),
			occupancyType(OccupancyType::Empty)
		{}

	}
}