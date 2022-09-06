/*!
@file I_KnockBack.cpp
@brief I_KnockBackのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_KnockBack.h"

namespace basecross {

	KnockBackData::KnockBackData()
		:KnockBackData(0.0f, Vec3(0.0f), Vec3(0.0f))
	{}

	KnockBackData::KnockBackData(const float& power, const Vec3& hitPoint, const Vec3& direct)
		: power(power), hitPoint(hitPoint), direct(direct)
	{}

}

//endbasecorss