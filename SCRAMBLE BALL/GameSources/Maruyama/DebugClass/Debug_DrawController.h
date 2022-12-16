
/*!
@file Debug_DrawController.h
@brief Debug_DrawControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// 表示用データ
		//--------------------------------------------------------------------------------------
		struct Debug_DrawData {
			maru::Rect rect;

			Debug_DrawData();
			Debug_DrawData(const maru::Rect& rect);
		};

		//--------------------------------------------------------------------------------------
		/// デバッグ用のドローコントローラー
		//--------------------------------------------------------------------------------------
		class Debug_DrawController
		{
		public:
			using DrawData = Debug_DrawData;

		private:
			std::weak_ptr<GameObject> m_drawObject;	//表示用オブジェクト

			Debug_DrawData m_data;						//表示用データ

		public:
			Debug_DrawController();
			Debug_DrawController(const Debug_DrawData& data);

			void OnDraw(const Vec3& position);

		private:
			void CreateDebugDrawObject(const Vec3& position);

			void SetDebugDrawActive(const bool isDraw);

		public:

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			std::shared_ptr<GameObject> GetDrawObject() const noexcept { return m_drawObject.lock(); }
		};


	}
}