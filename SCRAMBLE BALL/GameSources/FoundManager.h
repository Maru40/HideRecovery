
/*!
@file FoundManager.h
@brief FoundManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	見つかる者管理
	//--------------------------------------------------------------------------------------
	class FoundManager : public Component
	{
	public:

		/// <summary>
		/// 見つかる者のデータ
		/// </summary>
		struct Data
		{
			ex_weak_ptr<GameObject> gameObject;  //ゲームオブジェクト

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Data();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="gameObject">ゲームオブジェクト</param>
			Data(const std::shared_ptr<GameObject>& gameObject);
		};

	private:
		Data m_data = Data(); //見つかる者のデータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		FoundManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 見つかる者のデータの取得
		/// </summary>
		/// <returns>データ</returns>
		Data GetData() const noexcept { return m_data; }

		/// <summary>
		/// 見つかる者のデータの参照を取得
		/// </summary>
		/// <returns>見つかる者のデータの参照</returns>
		const Data& GetRefData() const{ return m_data; }
	};

}

//endbasecross