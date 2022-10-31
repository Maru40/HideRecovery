#pragma once
#include "stdafx.h"
#include "IPostEffect.h"

namespace basecross {
	class PostProcess {
		vector<shared_ptr<IPostEffect>> m_postEffects;

	public:
		PostProcess();

		template<class T, class... Args>
		void AddPostEffect(Args... args) {
			m_postEffects.push_back(make_shared<T>(args...));
		}

		template<class T>
		void RemovePostEffect() {
			auto it = m_postEffects.begin();

			while (it != m_postEffects.end()) {
				auto ptr = dynamic_cast<T*>(it->get());
				if (ptr) {
					m_postEffects.erase(it);
					break;
				}
				++it;
			}
		}

		void OnPostDraw();
	};
}
