#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	namespace Message {
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
	}

	class SplashMessageUI :public UIObjectBase {
		using Type = SimpleSprite::Type;
	public:
		/// <summary>
		/// 表示するメッセージ
		/// </summary>
		enum class MessageType {
			Relocation,		// ボールが再配置された
			GetBall,		// ボールを入手した
			StolenBall,		// ボールを奪われた
			CreateRoom,		// 部屋を作成した
			JoinRoom,		// 部屋に参加した
			CanNotJoin,		// 部屋に参加できなかった
			Remaining60s,	// 残り60秒
			Remaining30s,	// 残り30秒
			Goal,			// ゴール
			CannotConnect,	// インターネットに接続できません
		};
	private:
		// メッセージに対応するデータマップ
		static map<MessageType, Message::MessageData> sm_Type2DataMap;
		// メッセージオブジェクト
		shared_ptr<SimpleSprite> m_sprite;
		// 計測が始まっているか
		bool m_isStart;
		// タイマー
		TimeCounter m_timer;
		// デフォルトの色
		Col4 m_defaultColor;
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

		/// <summary>
		/// メッセージをクリア
		/// </summary>
		void ClearMessage();

		/// <summary>
		/// 色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Col4& color);

		/// <summary>
		/// 色を取得する
		/// </summary>
		/// <returns>色</returns>
		Col4 GetColor();
	};
}
