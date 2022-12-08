
/*!
@file RectDraw.h
@brief RectDrawなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	class RectDraw : public Component
	{
		maru::Rect m_rect;								//rectデータ

		std::weak_ptr<Transform> m_selfTransform;		//自分自身のトランスフォーム

		std::weak_ptr<GameObject> m_rectObject;			//表示用のrectオブジェクト
		std::weak_ptr<Transform> m_rectObjectTransform;	//表示用のrectオブジェクトのトランスフォーム

	public:
		RectDraw(const std::shared_ptr<GameObject>& objPtr, const maru::Rect& rect);

		void OnCreate();
		void OnUpdate();

	private:
		void CreateRectObject();
	};

}