
/*!
@file HidePlace.h
@brief HidePlaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HideItem;
	class OpenBoard;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------
	struct HidePlace_Parametor {
		Vec3 hidePositionOffset;	//隠す場所のオフセット
		Vec3 boardUIPositionOffset;	//UI表示の位置オフセット
		bool isOpen;

		HidePlace_Parametor();

		HidePlace_Parametor(const Vec3& hidePositionOffset);
	};

	//--------------------------------------------------------------------------------------
	/// 隠す場所
	//--------------------------------------------------------------------------------------
	class HidePlace : public Component
	{
	public:
		using Parametor = HidePlace_Parametor;

	private:
		Parametor m_param = Parametor();

		std::weak_ptr<HideItem> m_hideItem;

		static int m_objectCount;

		int m_objectId = 0;

		std::weak_ptr<SoundEmitter> m_soundEmitter;
		SoundClip m_openSoundClip;

		std::weak_ptr<OpenBoard> m_boardUIObject;

	public:
		HidePlace(const std::shared_ptr<GameObject>& objPtr);

		HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnLateStart() override;

	private:
		void CreateBoard();

	public:
		void Open();

		void Close();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 隠す場所の取得
		/// </summary>
		/// <returns>隠す場所</returns>
		Vec3 GetHidePosition() const { return transform->GetPosition() + m_param.hidePositionOffset; }

		void SetHideItem(const std::shared_ptr<HideItem>& item);

		std::shared_ptr<HideItem> GetHideItem() const;

		int GetObjectId() const { return m_objectId; }

		static void CountReset() { m_objectCount = 1; }

		static std::shared_ptr<HidePlace> GetStageHidePlace(int objectId);

		void SetDrawUI(const bool isActive);

		bool IsDrawUI() const;

		bool IsOpen() const { return m_param.isOpen; };
	};

}