/*!
@file   Animator.h
@brief  アニメーション管理クラス
*/

#pragma once
#include "stdafx.h"
#include "../Shader/BoneModelDraw.h"

namespace basecross {
	/// <summary>
	/// アニメーション用の情報
	/// </summary>
	struct AnimationClip {
		/// <summary>
		/// 名前
		/// </summary>
		wstring name;
		/// <summary>
		/// 再生開始時間
		/// </summary>
		int start;
		/// <summary>
		/// 再生終了時間
		/// </summary>
		int end;
		/// <summary>
		/// ループするか
		/// </summary>
		bool isLoop;
		/// <summary>
		/// 再生速度
		/// </summary>
		float playSpeed;

		AnimationClip() :
			AnimationClip(L"", 0, 0, false)
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_name">名前</param>
		/// <param name="_start">再生開始時間</param>
		/// <param name="_end">再生終了時間</param>
		/// <param name="_isLoop">ループするか</param>
		/// <param name="_playSpeed">再生速度</param>
		AnimationClip(const wstring& _name,
			int _start, int _end, bool _isLoop, float _playSpeed = 1.0f
		) :
			name(_name), start(_start), end(_end),
			isLoop(_isLoop), playSpeed(_playSpeed)
		{}
	};

	class Animator :public Component
	{
		std::unordered_map<wstring, AnimationClip> m_animationClipMap;

	protected:
		using ModelDrawComp = BoneModelDraw;

		/// <summary>
		/// モーションデータを読み込む
		/// </summary>
		/// <param name="csvKey">CSVLoadに登録したアニメーションデータのキー</param>
		/// <returns>アニメーションクリップ（配列）</returns>
		vector<AnimationClip> LoadAnimationData(const wstring& csvKey);

		/// <summary>
		/// アニメーションクリップの登録
		/// </summary>
		/// <param name="clip">登録するアニメーションクリップ</param>
		void RegisterAnimationClip(AnimationClip clip);

		/// <summary>
		/// アニメーションの変更（継承側のクラスで呼ぶ）
		/// </summary>
		/// <param name="key">変更したいアニメーションキー</param>
		void ChangeAnimation(wstring key);

	public:
		Animator(const shared_ptr<GameObject>& owner);

		virtual void OnUpdate()override;
		// Animatorでは基本使用しない
		virtual void OnDraw()override {}

		/// <summary>
		/// アニメーションが終了しているかどうか
		/// </summary>
		/// <returns>アニメーションが終了していたらtrue</returns>
		virtual bool IsTargetAnimationEnd();

		float GetPlaySpeed();
	};
}
