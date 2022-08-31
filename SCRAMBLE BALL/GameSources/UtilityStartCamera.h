
#pragma once
/*!
@file UtilityStartCamera.h
@brief UtilityStartCameraクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "StartCamera.h"

namespace basecross {
	
	class UtilityStartCamera
	{
		static map<wstring, vector<StartCamera::ShowParametor>> sm_startCameraParams; //カメラ演出パラメータマップ

	public:

		/// <summary>
		/// スタートカメラ演出用のパラメータ群を取得する。
		/// </summary>
		/// <param name="name">ステージの名前</param>
		/// <returns>スタートカメラ演出用のパラメータ群</returns>
		static vector<StartCamera::ShowParametor> GetStartCameraShowParams(const wstring& name) {
			if (sm_startCameraParams.count(name) == 0) {
				return vector<StartCamera::ShowParametor>();
			}

			return sm_startCameraParams.at(name);
		}

	};

}

//endbasecross