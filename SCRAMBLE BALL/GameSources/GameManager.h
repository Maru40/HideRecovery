
/*!
@file GameManager.h
@brief GameManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class GeneratorBase;

	//--------------------------------------------------------------------------------------
	/// ゲームマネージャー
	//--------------------------------------------------------------------------------------
	class GameManager : public maru::SingletonComponent<GameManager>
	{
	public:
		/// <summary>
		/// ゲームマネージャーのステートタイプ
		/// </summary>
		enum class State 
		{
			Start,     //スタート
			Game,      //ゲーム中
			Clear,     //クリア
			Result,    //result
			GameOver,  //ゲームオーバー
		};

		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_paramPtr = nullptr;  //パラメータ

		vector<ex_weak_ptr<GeneratorBase>> m_generators;  //全てのジェネレータを保存

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスが所有するゲームオブジェクト</param>
		GameManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ゲームクリアするか判断
		/// </summary>
		/// <returns>クリア条件を満たしたらtrue</returns>
		bool IsGameClear();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ステートの変更
		/// </summary>
		/// <param name="state">変更したいステート</param>
		void ChangeState(const State state);

		/// <summary>
		/// 現在のステートを取得
		/// </summary>
		/// <returns>現在のステート</returns>
		State GetCurrentState() const;

		/// <summary>
		/// ゲーム終了するか判断する
		/// </summary>
		void CheckClear();

	};

}

//endbasecross