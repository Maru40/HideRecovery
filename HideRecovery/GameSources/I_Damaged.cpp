/*!
@file I_Damaged.cpp
@brief I_Damaged‚ÌƒNƒ‰ƒXŽÀ‘Ì
’S“–FŠÛŽR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Damaged.h"

namespace basecross {

	DamageData::DamageData()
		:DamageData(20.0f, Vec3(0.0f), 0.0f, 0.0f)
	{}

	DamageData::DamageData(const float& value)
		:DamageData(value, Vec3(0.0f), 0.0f, 0.0f)
	{}

	DamageData::DamageData(const float& value, const Vec3& knockVec, const float& knockPower, const float& slowTime)
		:value(value), knockVec(knockVec), knockPower(knockPower), slowTime(slowTime), hitPoint(Vec3(0.0f))
	{}

	DamageData DamageData::operator+(const DamageData& data) {
		this->value += data.value;
		this->knockVec += data.knockVec;
		this->knockPower += data.knockPower;
		this->slowTime += data.slowTime;

		return *this;
	}

	DamageData DamageData::operator+=(const DamageData& data) {
		return  *this + data;
	}

}

//endbasecross