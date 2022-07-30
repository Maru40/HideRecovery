/*!
@file LBRBSpriteObject.h
@brief LBRBSpriteObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	struct SpriteData;

	//--------------------------------------------------------------------------------------
	/// LB,RBスプライトタイプ
	//--------------------------------------------------------------------------------------
	enum class LBRBSpriteObject_Type {
		RB,
		LB
	};

	//--------------------------------------------------------------------------------------
	/// LBRBスプライトオブジェクト
	//--------------------------------------------------------------------------------------
	class LBRBSpriteObject : public GameObject
	{
	public:
		using Type = LBRBSpriteObject_Type;

	private:
		ex_weak_ptr<SpriteObject> m_back;  //バックスプライト
		ex_weak_ptr<SpriteObject> m_main;  //メインスプライト

		std::vector<ex_weak_ptr<SpriteObject>> m_sprites;  //全てのスプライトの配列

		Type m_type = Type::LB;            //スプライトタイプ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		LBRBSpriteObject(const std::shared_ptr<Stage>& stage);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="type">スプライトタイプ</param>
		LBRBSpriteObject(const std::shared_ptr<Stage>& stage, const Type& type);

		void OnCreate() override;

	private:
		/// <summary>
		/// スプライトの複数生成
		/// </summary>
		/// <param name="datas">生成用データ配列</param>
		void CreateSprites(std::vector<SpriteData>& datas);

		/// <summary>
		/// スプライトのデータを複数生成
		/// </summary>
		/// <param name="type">スプライトタイプ</param>
		/// <returns>生成したスプライトのデータ</returns>
		std::vector<SpriteData> CreateSpriteDatas(const LBRBSpriteObject::Type& type);

	};

}