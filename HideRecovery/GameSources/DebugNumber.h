
/*!
@file Number.h
@brief Numberクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	デバッグ用数字
		//--------------------------------------------------------------------------------------
		class Number : public Component
		{
		public:
			using DrawComp = PCTStaticDraw;

		private:

			int m_number;	//表示したい数字
			float m_width;	//横幅
			float m_height;	//高さ
			float m_size;	//サイズ
			Col4 m_color;	//色

			std::vector<VertexPositionColorTexture> m_vertices;	//頂点配列
			std::vector<uint16_t> m_indices;					//インデックス配列

			/// <summary>
			/// メッシュの生成
			/// </summary>
			void CreateMesh();

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="number">表示する数字</param>
			Number(const std::shared_ptr<GameObject>& objPtr, const int& number) :
				Component(objPtr), m_number(number), 
				m_width(50.0f), m_height(100.0f), 
				m_size(1.0f),
				m_color(Col4(1.0f, 1.0f, 1.0f, 0.5f))
			{}

			void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 数字の設定
			/// </summary>
			/// <param name="number">表示する数字</param>
			void SetNumber(const int& number);

			/// <summary>
			/// 数字の取得
			/// </summary>
			/// <returns>表示する数字</returns>
			int GetNumber() const noexcept {
				return m_number;
			}

			/// <summary>
			/// サイズの設定
			/// </summary>
			/// <param name="number">サイズ</param>
			void SetSize(const float& size) noexcept {
				m_size = size;
			}

			/// <summary>
			/// サイズの取得
			/// </summary>
			/// <returns>サイズ</returns>
			float GetSize() const noexcept {
				return m_size;
			} 
		};

	}

}

//endbasecross