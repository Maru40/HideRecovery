/*!
@file ClearUIObject.h
@brief ClearUIObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class SpriteObject;

	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	///	クリアUIオブジェクト
	//--------------------------------------------------------------------------------------
	class ClearUIObject : public GameObject
	{
	private:
		ex_weak_ptr<SpriteObject> m_back;					 //backのUI
		ex_weak_ptr<SpriteObject> m_clear;					 //clearUI

		std::vector<ex_weak_ptr<SpriteObject>> m_allObjects; //管理する全てのUIオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		ClearUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		/// <summary>
		/// バックUIの生成
		/// </summary>
		void CreateBackUI();

		/// <summary>
		/// クリアUIの生成
		/// </summary>
		void CreateClearUI();

		/// <summary>
		/// UIの生成
		/// </summary>
		/// <param name="vertexParam">UI生成用パラメータ</param>
		/// <param name="position">生成位置</param>
		/// <returns>生成したUI</returns>
		std::shared_ptr<SpriteObject> CreateUI(const Builder::VertexPCTParametor& vertexParam, const Vec3& position);

	public:
		/// <summary>
		/// バックUIの取得
		/// </summary>
		/// <returns>バックUI</returns>
		std::shared_ptr<SpriteObject> GetBackUI() const;

		/// <summary>
		/// クリアUIの取得
		/// </summary>
		/// <returns>クリアUI</returns>
		std::shared_ptr<SpriteObject> GetClearUI() const;

	};

}