/*!
@file Factory_ImpactCellMap.h
@brief Factory_ImpactCellMap‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		struct Factory_ImpactCellMap_Parametor
		{

		};

		class Factory_ImpactCellMap
		{
		public:
			using Parametor = Factory_ImpactCellMap_Parametor;

		private:

		public:
			static void CreateMap(const Parametor& param);
		};

	}

}