
/*!
@file AccessHidePlace.h
@brief AccessHidePlace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HidePlace;

	class AccessHidePlace : public Component
	{
		float m_range;

		std::vector<std::weak_ptr<HidePlace>> m_allHidePlaces;	//�t�B�[���h�̑S�ẲB���I�u�W�F�N�g

	public:
		AccessHidePlace(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	public:
		void Access();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		float GetRange() const { return m_range; }
	};

}