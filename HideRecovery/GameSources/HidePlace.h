
/*!
@file HidePlace.h
@brief HidePlace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HideItem;

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

		std::weak_ptr<HideItem> m_hideItem;

		std::weak_ptr<SoundEmitter> m_soundEmitter;
		SoundClip m_openSoundClip;

	public:
		HidePlace(const std::shared_ptr<GameObject>& objPtr);

		HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate();
		void OnLateStart();

	public:
		void Open();

		void Close();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �B���ꏊ�̎擾
		/// </summary>
		/// <returns>�B���ꏊ</returns>
		Vec3 GetHidePosition() const { return transform->GetPosition() + m_param.hidePositionOffset; }

		void SetHideItem(const std::shared_ptr<HideItem>& item);

		std::shared_ptr<HideItem> GetHideItem() const;
	};

}