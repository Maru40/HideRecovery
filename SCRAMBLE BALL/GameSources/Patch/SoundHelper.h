#pragma once
#include<x3daudio.h>
#include"SaveHelper.h"

namespace basecross
{
	struct SoundSettingData
	{
		float bgmVolume;
		float seVolume;
	};

	class SoundSetting : public SavableBase<SoundSettingData>
	{
		SoundSettingData m_soundSettingData;

	public:
		itbs::Utility::Delegate<void()> changeEvent;

		SoundSetting(const std::wstring& filePath);

		void SetBGMVolume(const float bgmVolume);
		void SetSEVolume(const float seVolume);

		void AddBGMVolume(const float addVolume);
		void AddSEVolume(const float addVolume);

		float GetBGMVolume() const;
		float GetSEVolume() const;
	};

	/// <summary>
	/// サウンド再生用データ
	/// </summary>
	struct SoundClip
	{
		/// <summary>
		/// 再生音のキー
		/// </summary>
		std::wstring soundName;
		/// <summary>
		/// ループさせるかどうか
		/// </summary>
		bool isLoop;
		/// <summary>
		/// 音量
		/// </summary>
		float volume;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		SoundClip();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="soundName">再生音のキー</param>
		/// <param name="isLoop">ループさせるか</param>
		/// <param name="volume">音量</param>
		SoundClip(const std::wstring& soundName, const bool isLoop, const float volume);
	};

	/// <summary>
	/// SoundItemと音量のペア
	/// </summary>
	struct SoundData
	{
		/// <summary>
		/// サウンドアイテム
		/// </summary>
		ex_weak_ptr<SoundItem> soundItem;
		/// <summary>
		/// 音量
		/// </summary>
		float volume;
		/// <summary>
		/// 音素材のキー
		/// </summary>
		std::wstring soundKey;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		SoundData();
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="soundItem">サウンドアイテム</param>
		/// <param name="volume">音量</param>
		/// <param name="soundKey">音素材のキー</param>
		SoundData(const std::shared_ptr<SoundItem>& soundItem, const float volume, const std::wstring& soundKey);
	};


	/// <summary>
	/// 簡単なSEやBGM管理クラス
	/// </summary>
	class SimpleSoundManager
	{
		/// <summary>
		/// 自動でSettingされるかどうか
		/// </summary>
		static bool m_canSetting;

		/// <summary>
		/// BGMのサウンドアイテムの音量
		/// </summary>
		static SoundData m_bgmSoundData;

		/// <summary>
		/// SEのサウンドアイテムと音量構造体
		/// </summary>
		static std::vector<SoundData> m_seSoundDatas;

		/// <summary>
		/// サウンドアイテムを作成する
		/// </summary>
		/// <param name="soundClip">サウンドアイテムを作るのに必要なデータ構造体</param>
		/// <returns>サウンドアイテム</returns>
		static std::shared_ptr<SoundItem> CreateSoundItem(const SoundClip& soundClip);

		/// <summary>
		/// 管理しているサウンドアイテムの音量をすべて更新する
		/// </summary>
		static void UpdateSoundItemVolume();

		/// <summary>
		/// サウンドアイテムの音量を更新する
		/// </summary>
		/// <param name="soundItemAndVolume">更新するサウンドアイテムと音量構造体</param>
		/// <param name="baseVolume">更新する音量の基本音量</param>
		static void SettingSoundVolume(const SoundData& soundItemAndVolume, const float baseVolume);

		/// <summary>
		/// SEの管理データ配列を関数で更新する
		/// </summary>
		/// <param name="func">呼びたい関数</param>
		static void UpdateSESoundItemAndVolumes(void(*func)(SoundData&));
	public:
		/// <summary>
		/// BGMを切り替える
		/// </summary>
		/// <param name="bgmName">BGM名</param>
		/// <param name="volume">音量</param>
		static void ChangeBGM(const std::wstring& bgmName,const float volume = 1.0f);

		/// <summary>
		/// BGMを一時停止する
		/// </summary>
		static void PauseBGM();
		/// <summary>
		/// BGMを再開する
		/// </summary>
		static void UnPauseBGM();
		/// <summary>
		/// BGMを終了停止する
		/// </summary>
		static void StopBGM();

		/// <summary>
		/// 渡されたキーのBGMが流れているか
		/// </summary>
		/// <param name="bgmName">BGMのキー</param>
		/// <returns>流れているか</returns>
		static bool IsPlayingBGM(const std::wstring& bgmName);

		/// <summary>
		/// SEを一回再生する
		/// </summary>
		/// <param name="soundName">SE名</param>
		/// <param name="volume">音量</param>
		static void OnePlaySE(const std::wstring& soundName,const float volume = 1.0f);

		/// <summary>
		/// SEを一時停止する
		/// </summary>
		static void PauseSE();

		/// <summary>
		/// SEを再開する
		/// </summary>
		static void UnPauseSE();

		/// <summary>
		/// SEを終了する
		/// </summary>
		static void StopSE();
	};

	/// <summary>
	/// 3D音響用 音を聞くオブジェクトにつけるコンポーネント(1ステージに一つまで)
	/// </summary>
	class SoundListener : public Component
	{
		/// <summary>
		/// 初期化されているかどうか
		/// </summary>
		static bool m_isInited;
		/// <summary>
		/// X3DAudioのインスタンスハンドル
		/// </summary>
		static X3DAUDIO_HANDLE m_X3DAudioHandle;
		/// <summary>
		/// 3D音響の計算した結果が入っている構造体
		/// </summary>
		static X3DAUDIO_DSP_SETTINGS m_dspSettring;

		/// <summary>
		/// 初期化されていなかったら初期化をする
		/// </summary>
		static void CheckInit();

		/// <summary>
		/// リスナー構造体を作成する
		/// </summary>
		/// <returns>リスナー構造体</returns>
		X3DAUDIO_LISTENER CreateListener() const;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		SoundListener(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// 距離や位置などから音の情報を計算して返す
		/// </summary>
		/// <param name="emitter">エミッター構造体</param>
		/// <returns>音情報が入った構造体</returns>
		const X3DAUDIO_DSP_SETTINGS& SoundCalculate(const X3DAUDIO_EMITTER& emitter) const;
	};

	/// <summary>
	/// 3D音響用 音を発するオブジェクトにつけるコンポーネント
	/// </summary>
	class SoundEmitter : public Component
	{
		/// <summary>
		/// リスナーの参照
		/// </summary>
		ex_weak_ptr<SoundListener> m_listener;

		/// <summary>
		/// SoundItem管理配列
		/// </summary>
		std::vector<SoundData> m_soundItemAndVolumes;

		/// <summary>
		/// 音響計算で使うオブジェクトのエミッター構造体を作成する
		/// </summary>
		/// <param name="soundItem">サウンドアイテム</param>
		/// <returns>エミッター構造体</returns>
		X3DAUDIO_EMITTER CreateEmitter(const ex_weak_ptr<SoundItem>& soundItem) const;

		/// <summary>
		/// リスナーの参照が無ければリスナーを探索して参照する
		/// </summary>
		void CheckListener();

		/// <summary>
		/// サウンドアイテムを更新する
		/// </summary>
		/// <param name="soundItemAndVolome">サウンドアイテム</param>
		/// <param name="dspSetting">更新用のデータ構造体</param>
		void UpdateSoundItemAndVolume(const SoundData& soundItemAndVolome, const X3DAUDIO_DSP_SETTINGS& dspSetting) const;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		SoundEmitter(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// 音を再生する
		/// </summary>
		/// <param name="soundClip">再生するサウンドデータ</param>
		void PlaySoundClip(const SoundClip& soundClip);

		void OnStart() override;

		void OnUpdate() override;

		void OnEnable() override;

		void OnDisable() override;
	};
}