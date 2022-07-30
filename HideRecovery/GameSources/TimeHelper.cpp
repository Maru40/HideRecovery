
/*!
@file TimeHelper.cpp
@brief TimeHelperクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TimeHelper.h"

namespace basecross {

	namespace maru {

		float TimeHelper::GetElapsedTime(const DeltaType& type) {
			switch (type) {
			case DeltaType::Normal:
				return App::GetApp()->GetElapsedTime();
				break;

			case DeltaType::Real:
				//小さすぎるときの処理
				if (static_cast<float>(App::GetApp()->GetStepTimer().GetDefaultElapsedSeconds()) < 0.01f) {
					return 0.01f;
				}
				else {
					return static_cast<float>(App::GetApp()->GetStepTimer().GetDefaultElapsedSeconds());
				}
				break;
			}

			return App::GetApp()->GetElapsedTime();
		}

	}
}
