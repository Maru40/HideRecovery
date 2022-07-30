
/*!
@file SlimeGeneratorObject.h
@brief SlimeGeneratorObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// スライム生成オブジェクト
	//--------------------------------------------------------------------------------------
	class SlimeGeneratorObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		SlimeGeneratorObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		/// <summary>
		/// 通常スライムの生成
		/// </summary>
		void CreateNormalSlimeGenerator();

		/// <summary>
		/// ショットスライムの生成
		/// </summary>
		void CreateShootSlimeGenerator();

		/// <summary>
		/// 逃げるスライムの生成
		/// </summary>
		void CreateEscapeSlimeGenerator();

		/// <summary>
		/// 巨大スライムの生成
		/// </summary>
		void CreateBigSlimeGenerator();

		/// <summary>
		/// ファイルから生成場所を取得し、それぞれのスライムを生成する。
		/// </summary>
		void CreateLoadPostitionGenerator();

	};

}

//endbasecross