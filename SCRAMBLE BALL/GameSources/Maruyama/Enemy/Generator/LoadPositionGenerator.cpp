

/*!
@file LoadPositionGenerator.cpp
@brief LoadPositionGenerator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Random.h"
#include "LoadPositionGenerator.h"

namespace basecross {

	LoadPositionGenerator_Parametor::LoadPositionGenerator_Parametor():
		LoadPositionGenerator_Parametor(L"", L"", L"")
	{}

	LoadPositionGenerator_Parametor::LoadPositionGenerator_Parametor(const wstring& objectName, const wstring& folderName, const wstring& fileName):
		objectName(objectName), folderName(folderName), fileName(fileName)
	{}

}