
/*!
@file RectDraw.h
@brief RectDraw�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	class RectDraw : public Component
	{
		maru::Rect m_rect;								//rect�f�[�^

		std::weak_ptr<Transform> m_selfTransform;		//�������g�̃g�����X�t�H�[��

		std::weak_ptr<GameObject> m_rectObject;			//�\���p��rect�I�u�W�F�N�g
		std::weak_ptr<Transform> m_rectObjectTransform;	//�\���p��rect�I�u�W�F�N�g�̃g�����X�t�H�[��

		std::wstring m_texture;
		Col4 m_color;

	public:
		RectDraw(
			const std::shared_ptr<GameObject>& objPtr, 
			const maru::Rect& rect, 
			const std::wstring& texture = L"",
			const Col4& color = Col4(1.0f)
		);

		void OnCreate();
		void OnUpdate();

	private:
		void CreateRectObject();
	};

}