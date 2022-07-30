
/*!
@file SlimeCore.h
@brief SlimeCoreなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct SeekTarget_Parametor;
	class SeekTarget;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 目のオブジェクト
		//--------------------------------------------------------------------------------------
		class EyeObject : public GameObject
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			EyeObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

			/// <summary>
			/// ディフューズカラーの設定
			/// </summary>
			/// <param name="color">ディフューズカラー</param>
			void SetDiffuse(const Col4& color);
		};

		//--------------------------------------------------------------------------------------
		/// スライムのコア
		//--------------------------------------------------------------------------------------
		class SlimeCore : public Component
		{
			ex_weak_ptr<GameObject> m_parentObject; //目玉の親オブジェクト
			ex_weak_ptr<EyeObject> m_inObject;      //目玉の黒い部分
			ex_weak_ptr<EyeObject> m_outObject;     //目玉の白い部分

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SlimeCore(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;
			void OnDestroy() override;
			void OnDraw() override;

			void OnDrawFalse() override;

		private:
			/// <summary>
			/// コアの生成
			/// </summary>
			void CreateCore();

			/// <summary>
			/// コアの黒目を生成
			/// </summary>
			void CreateInObject();

			/// <summary>
			/// コアの眼球を生成
			/// </summary>
			void CreateOutObject();

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 黒目のディフューズの設定
			/// </summary>
			/// <param name="color">ディフューズカラー</param>
			void SetEyeDiffuse(const Col4& color);

			/// <summary>
			/// 眼球のディフューズの設定
			/// </summary>
			/// <param name="color">ディフューズカラー</param>
			void SetEyeBallDiffuse(const Col4& color);

			/// <summary>
			/// ParentObjectのSeekTargetを取得する。
			/// </summary>
			/// <returns>ParentObjectのSeekTarget</returns>
			std::shared_ptr<SeekTarget> GetSeekTargetPtr() const;
			
			/// <summary>
			/// 追従パラメータの設定
			/// </summary>
			/// <param name="seekParam">追従パラメータ</param>
			void SetSeekParametor(const SeekTarget_Parametor& seekParam);

			/// <summary>
			/// 追従パラメータを取得
			/// </summary>
			/// <returns>追従パラメータ</returns>
			SeekTarget_Parametor GetSeekParametor() const;
		}; 

	}
}
