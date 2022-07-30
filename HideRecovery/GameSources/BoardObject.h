
/*!
@file BoardObject.h
@brief BoardObjクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// 板オブジェクト
	//--------------------------------------------------------------------------------------
	class BoardObject : public GameObject
	{
	public:
		using DrawComp = BcPCTStaticDraw;

	private:
		std::shared_ptr<Builder::VertexPCTParametor> m_paramPtr;  //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="param">頂点データ生成用パラメータ</param>
		BoardObject(const std::shared_ptr<Stage>& stage,
			const Builder::VertexPCTParametor& param
		);

		virtual void OnCreate() override;

	protected:
		/// <summary>
		/// 頂点データ生成用パラメータの取得
		/// </summary>
		/// <returns>頂点データ生成用パラメータ</returns>
		const std::shared_ptr<const Builder::VertexPCTParametor> GetVertexPCTParametor() const;

		/// <summary>
		/// ディフューズカラーの設定
		/// </summary>
		/// <param name="color">ディフューズカラー</param>
		void SetDiffuse(const Col4& color);

		/// <summary>
		/// ディフューズカラーの取得
		/// </summary>
		/// <returns>ディフューズカラー</returns>
		Col4 GetDiffuse() const;

	};

}

//basecross