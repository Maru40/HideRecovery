/*!
@file MetaballChildrenSeekManager.h
@brief MetaballChildrenSeekManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class RenderBase;

		//--------------------------------------------------------------------------------------
		///	メタボールの追従パラメータ
		//--------------------------------------------------------------------------------------
		struct ChildrenSeekManager_Parametor
		{
			vector<SeekTarget::Parametor> seekParams; //追従パラメータ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ChildrenSeekManager_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="seekParams">追従パラメータ配列</param>
			ChildrenSeekManager_Parametor(const vector<SeekTarget::Parametor>& seekParams);
		};

		//--------------------------------------------------------------------------------------
		///	メタボールの追従管理
		//--------------------------------------------------------------------------------------
		class ChildrenSeekManager : public Component
		{
		public:
			using Parametor = ChildrenSeekManager_Parametor;

		private:
			Parametor m_param;												//パラメータ

			std::unique_ptr<SeekTargetOffsetsManager> m_seekOffsetsManager;	//追従オフセット管理

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// 追従セッティング
			/// </summary>
			void SettingSeek();

			/// <summary>
			/// 追従パラメータをセット
			/// </summary>
			/// <param name="seek">追従コンポーネント</param>
			/// <param name="index">現在のインデックス</param>
			void SetSeekParametor(const std::shared_ptr<SeekTarget>& seek, const int& index);

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 移動開始
			/// </summary>
			/// <param name="task">移動タスクタイプ</param>
			/// <param name="parametor">移動パラメータ</param>
			void StartMove(const SeekTargetOffsetsManager::TaskEnum& task, const SeekTargetOffsetsManager::Parametor& parametor);

			/// <summary>
			/// Seek状態を解除して弾けさせる
			/// </summary>
			/// <param name="velocity">弾ける速度</param>
			void Crash(const Vec3& velocity);

			/// <summary>
			/// タスクの終了判断
			/// </summary>
			/// <returns>タスクが終了していたらtrue</returns>
			bool IsEndTask();
			
		};

		//--------------------------------------------------------------------------------------
		///	Utility
		//--------------------------------------------------------------------------------------

		namespace Utility {
			//--------------------------------------------------------------------------------------
			///	追従パラメータのセッティングをできるようにするクラス
			//--------------------------------------------------------------------------------------
			class ChildrenSeekSetting
			{
			public:
				/// <summary>
				/// パラメータ配列を生成
				/// </summary>
				/// <param name="speed">スピード</param>
				/// <param name="range">範囲</param>
				/// <param name="offsets">オフセット群</param>
				/// <param name="seekType">追従タイプ</param>
				/// <returns>パラメータ配列</returns>
				static vector<SeekTarget::Parametor> CreateSeekTargetParametors(
					const float& speed, 
					const float& range,
					const std::vector<Vec3> offsets, 
					const SeekTarget::SeekType& seekType
				);

			};
		}

	}
}

//endbasecross