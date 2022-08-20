#pragma once
#include "stdafx.h"

// 参考：https://gintenlabo.hatenablog.com/entry/20100423/1272041386
namespace basecross {
	/**
	 * @brief シングルトンクラス（純C++クラス用）
	 */
	template <class T>
	class BaseSingleton {
	protected:
		// カスタム削除子。インスタンスの削除はここからのみ行なう。
		struct Deleter {
			void operator()(T const* const p) const {
				delete p;
			}
		};

		// unique_ptr は削除子を指定出来る
		static unique_ptr<T, Deleter> m_instance;

		BaseSingleton() {}
		~BaseSingleton() {}
	public:
		/**
		 * @brief インスタンスの生成
		 *
		 * @return インスタンス
		 */
		static unique_ptr<T, Deleter>& CreateInstance() {
			m_instance.reset(new T);
			return m_instance;
		}

		/**
		 * @brief インスタンスが生成されているか
		 *
		 * @return trueなら生成されている
		 */
		static bool CheckInstance() {
			return m_instance.get() != 0;
		}

		/**
		 * @brief インスタンスの取得（必要があれば生成する）
		 *
		 * @param exceptionActive インスタンスが生成されていなかったときに例外を出すか
		 * @return インスタンス
		 */
		static unique_ptr<T, Deleter>& GetInstance(bool exceptionActive = false) {
			if (!CheckInstance()) {
				if (!exceptionActive) {
					CreateInstance();
				}
				else {
					throw BaseException(
						L"インスタンスが生成されていません。",
						L"if (!m_instance)",
						L"unique_ptr<T, Deleter>& BaseSingleton::GetInstance()"
					);
				}
			}
			return m_instance;
		}

		/**
		 * @brief インスタンスの削除
		 */
		static void DeleteInstance() {
			if (m_instance.get()) {
				m_instance.reset();
			}
		}
	};

	/**
	 * @brief シングルトンクラス（ゲームオブジェクト用）
	 */
	template <class T>
	class BaseSingletonGameObject : public GameObject {
	private:
		// 自身のインスタンス
		static shared_ptr<T> m_ownInstance;

	protected:
		/**
		 * @brief インスタンスの生成
		 */
		void CreateInstance() {
			m_ownInstance = nullptr;
			m_ownInstance = GetThis<T>();
		}

	public:
		// ゲームオブジェクトの関係上コンストラクタはpublic
		BaseSingletonGameObject(const shared_ptr<Stage>& stage)
			: GameObject(stage) {}

		/**
		 * @brief インスタンスを取得
		 *
		 * @return インスタンス
		 */
		static shared_ptr<T> GetInstance() {
			if (m_ownInstance.get() == 0) {
				throw BaseException(
					L"インスタンスが生成されていません",
					L"if (m_ownInstance.get() == 0)",
					L"BaseSingletonGameObject::GetInstance()");
			}
			return m_ownInstance;
		}
	};
}