
/*!
@file Cell.h
@brief Cellなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	セルパラメータ
		//--------------------------------------------------------------------------------------
		struct Cell_Parametor {
			Rect rect;

			Cell_Parametor();

			Cell_Parametor(const Rect& rect);
		};

		//--------------------------------------------------------------------------------------
		///	セル
		//--------------------------------------------------------------------------------------
		class Cell
		{
		public:
			using Parametor = Cell_Parametor;

		private:
			Parametor m_param;

		public:
			Cell(const Parametor& param);

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			void SetPosition(const Vec3& position) noexcept { m_param.rect.centerPosition = position; }

			Vec3 GetPosition() const noexcept { return m_param.rect.centerPosition; }

			void SetWidth(const float width) noexcept { m_param.rect.width = width; }

			float GetWidth() const noexcept { return m_param.rect.width; }

			void SetDepth(const float depth) noexcept { m_param.rect.depth = depth; }

			float GetDepth() const noexcept { return m_param.rect.depth; }

			void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

			Parametor GetParametor() const noexcept { return m_param; }


			//--------------------------------------------------------------------------------------
			///	デバッグ
			//--------------------------------------------------------------------------------------

			std::weak_ptr<GameObject> m_debugDrawObject;	//デバッグ

			void OnDebugDraw();
			void OnDebugDraw(const std::shared_ptr<Stage>& stage);

			void CreateDebugDrawObject(const std::shared_ptr<Stage>& stage);

			void SetDebugDrawActive(const bool isDraw);
		};

	}

}