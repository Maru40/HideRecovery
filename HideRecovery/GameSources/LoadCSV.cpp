/*!
@file LoadCSV.cpp
@brief LoadCSV
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "LoadCSV.h"

namespace basecross {

	

	std::map<wstring, std::function<void(const CsvFile&)>> LoadCSV::sm_loadFuncMap = {
		{{L""}, { nullptr }},
		{{}, {}},
	};

}

//endbasecross