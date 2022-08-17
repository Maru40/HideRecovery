
/*!
@file I_TeamMember.h
@brief I_TeamMemberなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPoint.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class OwnArea;

	//--------------------------------------------------------------------------------------
	/// チームメンバーのインターフェース
	//--------------------------------------------------------------------------------------
	class I_TeamMember
	{
		std::weak_ptr<OwnArea> m_ownArea;

	public:
		virtual ~I_TeamMember() = default;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 自陣エリアの設定
		/// </summary>
		/// <param name="area">自陣エリア</param>
		void SetOwnArea(const std::shared_ptr<OwnArea>& area);

		/// <summary>
		/// 自陣エリアの取得
		/// </summary>
		/// <returns>自陣エリア</returns>
		std::shared_ptr<OwnArea> GetOwnArea() const;

		/// <summary>
		/// ゲームオブジェクトを取得
		/// </summary>
		/// <returns>ゲームオブジェクト</returns>
		virtual std::shared_ptr<GameObject> GetGameObject() = 0;
	};

}