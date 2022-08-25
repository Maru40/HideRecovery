#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class SplashMessageUI :public UIObjectBase {
	public:
		/// <summary>
		/// 表示するメッセージ
		/// </summary>
		enum class MessageType {
			Relocation,	// ボールが再配置された
			GetBall,	// ボールを入手した
			StolenBall,	// ボールを奪われた
		};
	private:
		struct MessageData {
			SimpleSprite::Type Type;
			wstring Key;
			MessageData()
				:MessageData(SimpleSprite::Type::None, L"")
			{}
			MessageData(SimpleSprite::Type type, const wstring& key)
				:Type(type), Key(key)
			{}
		};
		static map<MessageType, MessageData> sm_Type2DataMap;
		shared_ptr<SimpleSprite> m_sprite;
		bool m_isStart;
		TimeCounter m_timer;
	public:
		SplashMessageUI(const shared_ptr<Stage>& stage);
		SplashMessageUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;

		/// <summary>
		/// メッセージをセット
		/// </summary>
		/// <param name="type">メッセージタイプ</param>
		void SetMessage(MessageType type);
	};
}
