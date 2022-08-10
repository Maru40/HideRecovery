/*!
@file TesterAstarComponent.h
@brief TesterAstarComponent
’S“–ÒFŠÛR —TŠì
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

namespace basecross {

	namespace Tester {

		class TesterAstarComponent : public Component, public maru::I_Impacter
		{
		public:
			TesterAstarComponent(const std::shared_ptr<GameObject>& objPtr):
				Component(objPtr)
			{}

			void OnUpdate() override;

			std::shared_ptr<GameObject> GetImpacterObject() const noexcept override;

			std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const override;
		};

	}
}