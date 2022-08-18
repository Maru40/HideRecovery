/*!
@file SoundManager.h
@brief SoundManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	using namespace maru;

	//--------------------------------------------------------------------------------------
	/// サウンドマネージャー
	//--------------------------------------------------------------------------------------
	class SoundManager : public SingletonComponent<SoundManager>
	{
	public:
		enum class ID {
			SlimePressing, //スライムを潰している間
			SlimePressed,  //スライムを潰す
			ClearSE,       
			BackSE,       
			NextSE,        
			CursorSE,
			EnterSE,

			TitleBGM,
			SelectBGM,
			GameBGM,

			PlayerWalk,    //Playerの移動
			PlayerTackle,  //Playerのタックル
			JettPomp,      //ジェットポンプ

			ShotSE,
		};

	private:
		map<ID, wstring> m_soundMap;             //サウンドマップ
		map<ID, ex_weak_ptr<SoundItem>> m_items; //サウンドアイテム配列

		float m_baseVolume;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SoundManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// サウンドマップのセッティング
		/// </summary>
		void SettingSoundMap();

		/// <summary>
		/// サウンドアイテムの追加
		/// </summary>
		/// <param name="id">サウンドID</param>
		/// <param name="item">サウンドアイテム</param>
		void AddSoundItem(const ID& id, const std::shared_ptr<SoundItem>& item);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 一度鳴らす
		/// </summary>
		/// <param name="id">サウンドID</param>
		/// <param name="volume">ボリューム</param>
		/// <param name="loopCount">ループ回数</param>
		/// <returns>生成したサウンドアイテム</returns>
		std::shared_ptr<SoundItem> PlayOneShot(const ID& id, const float& volume = 0.5f, const float& loopCount = 0.0f);

		/// <summary>
		/// BGMを鳴らす
		/// </summary>
		/// <param name="id">サウンドID</param>
		/// <param name="volume">ボリューム</param>
		/// <param name="LoopCount">ループ回数</param>
		/// <returns>生成したサウンドアイテム</returns>
		std::shared_ptr<SoundItem> PlayBGM(const ID& id, const float& volume = 0.5f, const float& LoopCount = XAUDIO2_LOOP_INFINITE);

		/// <summary>
		/// 指定したサウンドを止める。
		/// </summary>
		/// <param name="id">サウンドアイテム</param>
		void Stop(const ID& id);

		/// <summary>
		/// 全てのサウンドを止める。
		/// </summary>
		void AllStop();

	};

}

//endbasecross