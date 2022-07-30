/*!
@file Cursor.h
@brief カーソルなど
*/

#pragma once
#include "stdafx.h"

#include "PolygonTexture.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// カーソルで管理するUIのデータ構造体
	//------------------------------------------------------------------------------------------------
	struct CursorUIData
	{
		UiData uiData; //! 表示する板ポリの情報
		std::function<void()> func; //! 選択した時に実行する関数
		bool isSelectable; //! 選択可能かどうか

		//------------------------------------------------------------------------------------------------
		/**
		* @brief 実行する関数を持たないUIのコンストラクタ
		* @param[in] data 表示する板ポリの情報
		*/
		CursorUIData::CursorUIData(const UiData& data)
			: uiData(data), isSelectable(false)
		{}

		/**
		* @brief 実行する関数を持つUIのコンストラクタ
		* @param[in] data 表示する板ポリの情報
		* @param[in] func 選択した時に実行する関数
		* @param[in] isSelectable 選択可能かどうか
		*/
		CursorUIData::CursorUIData(const UiData& data, std::function<void()> func, bool isSelectable)
			: uiData(data), func(func), isSelectable(isSelectable)
		{}
		//------------------------------------------------------------------------------------------------
	};

	//------------------------------------------------------------------------------------------------
	///　カーソルオブジェクトクラス
	//------------------------------------------------------------------------------------------------
	class Cursor : public GameObject
	{
		std::vector<std::shared_ptr<PolygonTexture>> m_cursorObj; //! 配置されているUIオブジェクトのポインタ
		std::vector<CursorUIData> m_UiObj; //! UIのデータ

		int m_CursorPos = 0; //! カーソルが指しているさき
		float m_PositionOffset = 23.0f; //! UIを指すときの位置のオフセット

	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		Cursor::Cursor(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief 配置するUIの追加
		* @param[in] data 配置するUIのデータ
		*/
		void AddUiTexture(const CursorUIData& data);

		/// 配置されているUiを全て削除
		void ClearUITexture();

		/**
		* @brief 選択した時の関数実行
		*/
		void Select();

		/**
		* @brief カーソルの移動
		* @param[in] どっちの方向にどれだけ動かすか
		*/
		void MoveCursor(int pos);

		/**
		* @brief カーソルが選択可能なUIを指していないときに移動させる
		* @param pos どっち方向に戻すか
		*/
		void PositionCorrection(int pos);

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief カーソルとUIの描画状態の設定
		* @param[in] value 描画状態(描画するならtrue)
		*/
		void SetAllDrawActive(bool value)
		{
			SetDrawActive(value); // カーソルの描画状態を設定
			for (auto obj : m_cursorObj)
			{
				obj->SetDrawActive(value); // UIの描画状態を設定
			}
		}

		/**
		* @brief カーソルとUIの表示レイヤーの設定
		* @param[in] layer 表示するレイヤー番号
		*/
		void SetAllDrawLayer(int layer)
		{
			SetDrawLayer(layer);
			for (auto data : m_cursorObj)
			{
				data->SetDrawLayer(layer);
			}
		}

		/**
		* @brief カーソルの指し示す先の設定
		* @param[in] position 指すUIの配列番号
		*/
		void SetCursorPos(int position)
		{
			m_CursorPos = position;
			MoveCursor(1);
		}
		//-----------------------------------------------------------------------------------------------
	};
}
