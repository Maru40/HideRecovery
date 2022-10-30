#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class HideItem;

	class OnlineGameItemManager : public Online::OnlineComponent
	{
	public:
		/// <summary>
		/// アイテムをランダムに隠すオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t ITEM_RANDOM_HIDE_EVENT_CODE = 203;
	private:

	public:
		OnlineGameItemManager(const std::shared_ptr<GameObject>& owner);

		void OnDraw() override {}

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		void RandomHideItem(const std::shared_ptr<HideItem>& hideItem);

		void ExecuteRandomHideItem(std::uint32_t hideItemInstanceId, std::uint32_t hidePlaceInstanceId);
	};

}