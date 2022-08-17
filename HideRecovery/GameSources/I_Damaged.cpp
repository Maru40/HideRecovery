/*!
@file I_Damaged.cpp
@brief I_Damagedのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Damaged.h"

namespace basecross {
	DamageData::DamageData()
		:DamageData(20, nullptr)
	{}

	DamageData::DamageData(const int& value, const std::shared_ptr<GameObject>& attacker)
		: value(value),
		attacker(attacker)
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