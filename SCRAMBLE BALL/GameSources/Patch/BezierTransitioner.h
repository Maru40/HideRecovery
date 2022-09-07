#pragma once
#include"Bezier.h"
#include<functional>

using namespace itbs::Math;

namespace basecross
{
	/// <summary>
	/// グラフデータを用い、登録された関数に遷移データを入れるコンポーネント
	/// </summary>
	class BezierTransitioner : public Component
	{
		/// <summary>
		/// 登録関数型
		/// </summary>
		using func_type = std::function<void(const float)>;

		/// <summary>
		/// 呼ぶ関数とそのデータマネージャーのペア
		/// </summary>
		struct FunctionPointPair
		{
			/// <summary>
			/// 関数データ
			/// </summary>
			func_type function;
			/// <summary>
			/// ベジェデータマネージャー
			/// </summary>
			BezierManager pointManager;

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="function">関数データ</param>
			/// <param name="manager">ベジェデータマネージャー</param>
			FunctionPointPair(func_type function, const BezierManager& manager);
		};

		/// <summary>
		/// 管理ペア配列
		/// </summary>
		std::vector<FunctionPointPair> m_pointManagers;

		/// <summary>
		/// 再生中かどうか
		/// </summary>
		bool m_isPlaying = true;
		
		/// <summary>
		/// 現在の再生位置時間
		/// </summary>
		float m_time = 0;
		/// <summary>
		/// ループ開始時間
		/// </summary>
		float m_loopTime = 0;

		/// <summary>
		/// ループするかどうか
		/// </summary>
		bool m_isLoop = false;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		BezierTransitioner(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// 管理するベジェデータマネージャーを登録する
		/// </summary>
		/// <param name="function">呼ばれる関数</param>
		/// <param name="manager">ベジェデータマネージャー</param>
		void AddManager(func_type function, const BezierManager& manager);

		/// <summary>
		/// 再生開始
		/// </summary>
		void Play();

		/// <summary>
		/// 再生停止
		/// </summary>
		void Stop();

		/// <summary>
		/// 時間を飛ばす
		/// </summary>
		/// <param name="skipTime">飛ばす時間</param>
		void Skip(const float skipTime);

		/// <summary>
		/// 任意時間に移動
		/// </summary>
		/// <param name="time">時間</param>
		void Move(const float time);

		/// <summary>
		/// 初めに移動
		/// </summary>
		void MoveStart();

		/// <summary>
		/// 終了位置まで移動
		/// </summary>
		void MoveEnd();

		/// <summary>
		/// ループするかどうかを設定する
		/// </summary>
		/// <param name="isLoop">ループするかどうか</param>
		void SetIsLoop(const bool isLoop);

		/// <summary>
		/// 初期化関数
		/// </summary>
		void OnCreate() override {}

		/// <summary>
		/// 更新関数
		/// </summary>
		void OnUpdate() override;

		/// <summary>
		/// 描画関数
		/// </summary>
		void OnDraw() override {}
	};
}