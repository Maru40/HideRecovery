#pragma once
#include "stdafx.h"

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
		float start;
		/// <summary>
		/// 再生終了時間
		/// </summary>
		float end;
		/// <summary>
		/// ループするか
		/// </summary>
		bool isLoop;
		/// <summary>
		/// 再生速度
		/// </summary>
		float playSpeed;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_name">名前</param>
		/// <param name="_start">再生開始時間</param>
		/// <param name="_end">再生終了時間</param>
		/// <param name="_isLoop">ループするか</param>
		/// <param name="_playSpeed">再生速度</param>
		AnimationClip(const wstring& _name,
			float _start, float _end, bool _isLoop, float _playSpeed = 1.0f)
			:name(_name), start(_start), end(_end),
			isLoop(_isLoop), playSpeed(_playSpeed)
		{}
	};

	class Animator :public Component {
	public:
		Animator(const shared_ptr<GameObject>& owner);

		// Animatorでは基本使用しない
		virtual void OnDraw()override {}

		bool LoadMotionData(wstring data);
	};
}
