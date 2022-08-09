
/*!
@file PlayerAnimationCtrl.h
@brief PlayerAnimationCtrl.hクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "AnimationCtrl.h"

namespace basecross {

	template<class T>
	struct AddAnimeParam {
		wstring name;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;

		function<void(T&)> func;

		AddAnimeParam(const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const function<void(T&)> func
		) :
			AddAnimeParam(name, startTime, endTime, loop, 1.0f, func)
		{}

		AddAnimeParam(const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const function<void(T&)> func
		) :
			name(name),
			startTime(startTime),
			loop(loop),
			timeParSecond(parSecond),
			func(func)
		{
			timeLength = endTime - startTime;
		}
	};

	class PlayerAnimationCtrl : public AnimationCtrl
	{
		map<wstring, std::function<void(PlayerAnimationCtrl&)>> m_animations;

		float m_time = 0.0f;

		bool m_isMoveLock;
		bool m_isCameraLook;

		float CalucGravityVelocityY();
		bool IsJump();

	public:

		PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr)
			:AnimationCtrl(objPtr),
			m_isMoveLock(false),
			m_isCameraLook(false)
		{}

		void OnCreate() override;
		void OnUpdate() override;

	private:

		void Wait();
		void Walk();
		void ReturnWalk();
		void Jump();
		void Turn();
		void HundShake();
		void LookOut();
		void Appeal();
		void Fall();
		void Landing();  //着地
		void Dest();
		void TurnFixed();

		void DefaultPlay(const float speed = 15.0f);  //デフォの再生

	public:

		//アクセッサ
		void SetAnimaiton(const wstring& animeName, const function<void(PlayerAnimationCtrl&)> func);

		void ChangeAnimation(const wstring& animeType);//,const bool isCameraLook = false, const bool isLock = false);
		void ChangeAnimation(const wstring& animeType,const bool isCameraLook, const bool isLock );

		wstring GetCurrentAnimaiton() const {
			auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
			return draw->GetCurrentAnimation();
		}
	};

}

//endbasecross