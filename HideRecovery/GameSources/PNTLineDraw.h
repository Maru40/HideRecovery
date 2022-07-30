/*!
@file PNTLineDraw.h
@brief PNTLineDraw
íSìñé“ÅFä€éR óTäÏ
*/

#pragma once

#include "stdafx.h"

namespace basecross {

	class PNTLineDraw : public SmBaseDraw
	{
	public:
		PNTLineDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDraw() override;

	private:
		void DrawStatic2(const MeshPrimData& data);

	};

}