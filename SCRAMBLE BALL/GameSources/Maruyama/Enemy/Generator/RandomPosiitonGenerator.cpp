
/*!
@file RandomPositionGenerator.cpp
@brief RandomPositionGeneratorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Random.h"
#include "RandomPositionGenerator.h"

namespace basecross {

	RandomPositionGenerator_Parametor::RandomPositionGenerator_Parametor()
		:RandomPositionGenerator_Parametor(1)
	{}

	RandomPositionGenerator_Parametor::RandomPositionGenerator_Parametor(const int& numCreate)
		: RandomPositionGenerator_Parametor(numCreate, Vec3(0.0f, 2.0f, 0.0f), Vec3(7.0f, 0.0f, 7.0f))
	{}

	RandomPositionGenerator_Parametor::RandomPositionGenerator_Parametor(
		const int& numCreate,
		const Vec3& centerPosition, const Vec3& createRangeVec
	) :
		numCreate(numCreate),
		centerPosition(centerPosition), createRangeVec(createRangeVec)
	{}

}

//endbasecross