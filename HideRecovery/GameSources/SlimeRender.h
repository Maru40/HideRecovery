/*!
@file SlimeRender.h
@brief SlimeRenderなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw;
	class RayMarchingFilterObject;
	struct PressData;

	namespace maru {
		enum class DeltaType;
	}

	namespace Metaball {
		class ChildrenRender;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// スライムのレンダークラス
		//--------------------------------------------------------------------------------------
		class SlimeRender : public basecross::Metaball::RenderBase
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SlimeRender(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="numCreate">生成する数</param>
			SlimeRender(const std::shared_ptr<GameObject>& objPtr, const int& numCreate);
		};

	}
}