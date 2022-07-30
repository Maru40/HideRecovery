#pragma once

namespace basecross
{
	/// <summary>
	/// 素材のロードを行うステージ
	/// </summary>
	class LoadStage : public Stage
	{
		/// <summary>
		/// リソースの読み込み用データ構造体
		/// </summary>
		struct ResourceLoadData
		{
			/// <summary>
			/// リソースのキー
			/// </summary>
			std::wstring resourceKey;
			/// <summary>
			/// 読み込むファイル名
			/// </summary>
			std::wstring filename;

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="resourceKey">リソースのキー</param>
			/// <param name="filename">読み込むファイル名</param>
			ResourceLoadData(const std::wstring& resourceKey, const std::wstring& filename);
		};

		/// <summary>
		/// モデルのタイプ
		/// </summary>
		enum class ModelType
		{
			/// <summary>
			/// スタティックモデル
			/// </summary>
			Static,
			/// <summary>
			/// ボーンモデル
			/// </summary>
			Bone,
			StaticMulti,
			BoneMulti,
		};

		/// <summary>
		/// モデル用リソース読み込み用データ構造体
		/// </summary>
		struct ModelResourceLoadData : public ResourceLoadData
		{
			/// <summary>
			/// モデルのタイプ
			/// </summary>
			ModelType modelType;
			/// <summary>
			/// モデルが入っているフォルダ
			/// </summary>
			std::wstring modelDir;
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="resourceKey">リソースのキー</param>
			/// <param name="filename">読み込むファイルの名前</param>
			/// <param name="modelType">モデルのタイプ</param>
			/// <param name="modelDir">モデルが入っているフォルダ名</param>
			ModelResourceLoadData(const std::wstring& resourceKey,
				const std::wstring& filename, const ModelType modelType,const std::wstring& modelDir);
		};

		/// <summary>
		/// ミューテックス
		/// </summary>
		static std::mutex m_mtx;
		/// <summary>
		/// ロードが終了したかどうか
		/// </summary>
		static bool m_isLoadFinish;

		/// <summary>
		/// 次のステージのキー
		/// </summary>
		std::wstring m_nextStageKey;

		/// <summary>
		/// 本来のステージの背景色
		/// </summary>
		Col4 m_stageBackColor;

		/// <summary>
		/// テクスチャロード用データ配列
		/// </summary>
		static const std::vector<ResourceLoadData> m_texureResourceLoadDatas;
		/// <summary>
		/// SEロード用配列
		/// </summary>
		static const std::vector<ResourceLoadData> m_seResourceLoadDatas;
		/// <summary>
		/// BGMロード用配列
		/// </summary>
		static const std::vector<ResourceLoadData> m_bgmResourceLoadDatas;
		/// <summary>
		/// モデルロード用配列
		/// </summary>
		static const std::vector<ModelResourceLoadData> m_modelResourceLoadDatas;
		/// <summary>
		/// ビュー作成
		/// </summary>
		void CreateViewLight();

		/// <summary>
		/// 素材のロードをする
		/// </summary>
		static void LoadResource();

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="nextStageKey">次のステージのキー</param>
		LoadStage(const std::wstring& nextStageKey);

		void OnCreate() override;

		void OnUpdate() override;
	};
}