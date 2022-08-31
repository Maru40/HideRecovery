#include "stdafx.h"
#include "EfkEffect.h"

namespace basecross {
	EfkEffectResource::EfkEffectResource(const wstring& path)
		:m_filePath(path)
	{
		if (m_filePath == L"") {
			throw BaseException(
				L"エフェクトファイル名が空白です。",
				L"if (m_filePath == L\"\")",
				L"EfkEffectResource::EfkEffectResource()"
			);
		}
		// インターフェイスのインスタンスを取得
		auto iEfk = EfkInterface::GetInstance();
		// エフェクトの読み込み
		m_effect = Effekseer::Effect::Create(iEfk->GetManager(), (const char16_t*)m_filePath.c_str());

		// 失敗した場合
		if (m_effect == nullptr) {
			throw BaseException(
				L"エフェクトの生成に失敗しました。\nFileParh : " + m_filePath,
				L"if (m_effect == nullptr)",
				L"EfkEffectResource::EfkEffectResource()"
			);
		}
	}

	shared_ptr<EfkEffectResource> EfkEffectResource::RegisterEffectResource(const wstring& key, const wstring& filePath) {
		const auto& app = App::GetApp();
		// すでに存在する場合は既存のポインタを返す
		if (app->CheckResource<EfkEffectResource>(key + EfkKey)) {
			return app->GetResource<EfkEffectResource>(key + EfkKey);
		}
		auto effectPtr = ObjectFactory::Create<EfkEffectResource>(filePath);
		app->RegisterResource(key + EfkKey, effectPtr);
		return effectPtr;
	}
}