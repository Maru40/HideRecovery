
/*!
@file HidePlace.h
@brief HidePlace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------
	struct HidePlace_Parametor {
		Vec3 hidePositionOffset;	//�B���ꏊ�̃I�t�Z�b�g

		HidePlace_Parametor();

		HidePlace_Parametor(const Vec3& hidePositionOffset);
	};

	//--------------------------------------------------------------------------------------
	/// �B���ꏊ
	//--------------------------------------------------------------------------------------
	class HidePlace : public Component
	{
	public:
		using Parametor = HidePlace_Parametor;

	private:
		Parametor m_param = Parametor();

	public:
		HidePlace(const std::shared_ptr<GameObject>& objPtr);

		HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �B���ꏊ�̎擾
		/// </summary>
		/// <returns>�B���ꏊ</returns>
		Vec3 GetHidePosition() const { return transform->GetPosition() + m_param.hidePositionOffset; }
	};

}