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
		:DamageData(20)
	{}

	DamageData::DamageData(const int& value)
		: value(value)
	{}

	DamageData DamageData::operator+(const DamageData& data) {
		this->value += data.value;
		return *this;
	}

	DamageData DamageData::operator+=(const DamageData& data) {
		return  *this + data;
	}
}

//endbasecross