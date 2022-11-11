/*!
@file Targeted.cpp
@brief Targetedなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "Targeted.h"

namespace basecross {

	Targeted::Targeted(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr)
	{}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	Vec3 Targeted::GetPosition() const {
		return transform->GetPosition() + m_param.offset;
	}

	void Targeted::SetOffset(const Vec3& offset) {
		m_param.offset = offset;
	}

	Vec3 Targeted::GetOffset() const {
		return m_param.offset;
	}

	bool Targeted::CanTarget() const {
		//条件式が一つもないなら、無条件でターゲットに指定できる。
		if (m_canTargetFunctions.empty()) {
			return true;
		}

		//一つでもターゲット指定できない条件が合ったら、falseを返す。
		for (auto& canFunction : m_canTargetFunctions) {
			if (!canFunction()) {
				return false;
			}
		}

		//全ての条件を達成しているためtrue
		return true;
	}

	void Targeted::AddCanTargetFunction(const std::function<bool()>& canTargetFunction) {
		m_canTargetFunctions.push_back(canTargetFunction);
	}
}