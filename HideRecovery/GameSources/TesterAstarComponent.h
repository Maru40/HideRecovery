/*!
@file TesterAstarComponent.h
@brief TesterAstarComponent
íSìñé“ÅFä€éR óTäÏ
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

namespace basecross {

	namespace maru {
		class ImpactMap;
	}

	namespace Tester {

		class TesterAstarComponent : public Component, public maru::I_Impacter
		{
		public:
			TesterAstarComponent(const std::shared_ptr<GameObject>& objPtr):
				Component(objPtr)
			{}

			void OnUpdate() override;

		private:
			void UpdateEyeRangeImpactMap();

		public:

			std::shared_ptr<GameObject> GetImpacterObject() const noexcept override;

			std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const override;

			std::shared_ptr<maru::ImpactMap> GetImpactMap() const override ;
		};

	}
}