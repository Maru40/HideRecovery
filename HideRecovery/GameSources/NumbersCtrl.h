
/*!
@file NumbersCtrl.h
@brief NumbersCtrlクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "NumberObject.h"

namespace basecross {

	class NumbersCtrl : public Component 
	{
		float m_value = 0;								//表示する値
		int m_maxDigit = 3;								//最大桁数
		wstring m_texture = L"Number_TX";				//数字表示に使うテクスチャ

		vector<ex_weak_ptr<NumberObject>> m_numberObjs;	//生成したナンバーオブジェクト

		/// <summary>
		/// 表示する数だけオブジェクトを生成する
		/// </summary>
		void CreateNumberObjs();

		/// <summary>
		/// いらない0は非表示にする。
		/// </summary>
		void ChangeDrawNumbers();

		/// <summary>
		/// ナンバーのポジションをセットする。
		/// </summary>
		void SetNumbersPosition();

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="texture">数字表示表のテクスチャ</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr, const wstring& texture):
			Component(objPtr), m_texture(texture)
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="maxDigit">最大桁数</param>
		NumbersCtrl(const std::shared_ptr<GameObject>& objPtr, const int& maxDigit)
			:Component(objPtr),m_maxDigit(maxDigit)
		{}

		void OnCreate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// 整数部分の変更
		/// </summary>
		/// <param name="number">変更したい数字</param>
		void ChangeIntegerNumber(const int value);

		/// <summary>
		/// 小数点部分の表示を変更
		/// </summary>
		/// <param name="value">変更したい数字</param>
		void ChangeFloatNumber(const float value);

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 表示する値の設定
		/// </summary>
		/// <param name="value">表示する値</param>
		void SetValue(const float value);

		/// <summary>
		/// 表示する数字の取得
		/// </summary>
		/// <returns>表示する数字</returns>
		float GetValue() const noexcept;

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color">設定する色</param>
		void SetColor(const Col4& color);

		/// <summary>
		/// テクスチャの設定
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		void SetTexture(const wstring& texture);

		/// <summary>
		///	テクスチャの取得
		/// </summary>
		/// <returns>テクスチャ</returns>
		wstring GetTexture() const noexcept { return m_texture; }
	};

}

//endbasecross