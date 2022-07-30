/*!
@file SlimeObjectBase.h
@brief SlimeObjectBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyObjectBase;

		//--------------------------------------------------------------------------------------
		/// スライムオブジェクト基底クラス
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase : public EnemyObjectBase
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスを所有するステージ</param>
			SlimeObjectBase(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		protected:

			/// <summary>
			/// モデルの生成
			/// </summary>
			virtual void CreateModel();

			/// <summary>
			/// メタボール追従セッティング
			/// </summary>
			virtual void SettingSeek();

			/// <summary>
			/// ScalePopセッティング
			/// </summary>
			virtual void SettingScalePop();

			/// <summary>
			/// 壁回避セッティング
			/// </summary>
			virtual void SettingWallEvasion();

			/// <summary>
			/// 影響マップのセッティング
			/// </summary>
			virtual void SettingSoleImpactMap();
		};
	}
}

//endbasecross