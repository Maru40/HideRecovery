
/*!
@file I_Impact.cpp
@brief I_Impact�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "EyeSearchRange.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			dangerValue(1.0f), 
			confirmationIntervalTime(30.0f), 
			isConfirmation(false)
		{}

	}
}