

/*!
@file LoadPositionGenerator.cpp
@brief LoadPositionGeneratorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MyRandom.h"
#include "LoadPositionGenerator.h"

namespace basecross {

	LoadPositionGenerator_Parametor::LoadPositionGenerator_Parametor():
		LoadPositionGenerator_Parametor(L"", L"", L"")
	{}

	LoadPositionGenerator_Parametor::LoadPositionGenerator_Parametor(const wstring& objectName, const wstring& folderName, const wstring& fileName):
		objectName(objectName), folderName(folderName), fileName(fileName)
	{}

}