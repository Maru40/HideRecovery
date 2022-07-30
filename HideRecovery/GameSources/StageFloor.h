#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	class StageFloor : public StageObjectBase
	{
	public:
		StageFloor(const std::shared_ptr<Stage>& stage)
			:StageObjectBase(stage, L"", L"")
		{ }

		StageFloor(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture);

		void OnCreate() override;
		void OnUpdate() override;
	};

}
