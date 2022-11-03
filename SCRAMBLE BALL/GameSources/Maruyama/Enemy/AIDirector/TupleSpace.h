
/*!
@file TupleSpace.h
@brief TupleSpaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {
		template<class T>
		class Action;
	}

	namespace Enemy {

		class I_FactionMember;

		namespace Tuple {

			//--------------------------------------------------------------------------------------
			/// タプルのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Tuple {
			public:
				virtual ~I_Tuple() = default;

				virtual bool operator== (const I_Tuple& other);
			};

			//--------------------------------------------------------------------------------------
			/// 希望行動の基底クラス
			//--------------------------------------------------------------------------------------
			class TupleActionBase : public I_Tuple
			{
			public:
				std::weak_ptr<GameObject> acter;		//行動する者
				std::weak_ptr<GameObject> requester;	//対象者
				float value;							//評価値

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="acter">行動する者</param>
				/// <param name="requester">対象者</param>
				/// <param name="value">評価値</param>
				TupleActionBase(const std::shared_ptr<GameObject>& acter, const std::shared_ptr<GameObject>& requester, const float value);

				virtual ~TupleActionBase() = default;
			};

			//--------------------------------------------------------------------------------------
			/// リクエストの基底クラス
			//--------------------------------------------------------------------------------------
			class TupleRequestBase : public I_Tuple
			{
			public:
				std::weak_ptr<GameObject> requester;	//要請を出した者
				float value;							//評価値

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="requester">要請を出した者</param>
				/// <param name="value">評価値</param>
				TupleRequestBase(const std::shared_ptr<GameObject>& requester, const float value);

				virtual ~TupleRequestBase() = default;

				virtual bool operator== (const TupleRequestBase& other);
			};

			//--------------------------------------------------------------------------------------
			/// ターゲットを見つけたことを知らせるタプル
			//--------------------------------------------------------------------------------------
			class FindTarget : public TupleRequestBase
			{
			public:
				std::weak_ptr<GameObject> target;

				FindTarget(const std::shared_ptr<GameObject>& requester, const std::shared_ptr<GameObject>& target, const float value = 1.0f);

				virtual ~FindTarget() = default;

				bool operator== (const FindTarget& other);
			};

			//--------------------------------------------------------------------------------------
			/// ターゲットとバトルすることをリクエストするタプル
			//--------------------------------------------------------------------------------------
			class ButtleTarget : public TupleRequestBase 
			{
				std::weak_ptr<GameObject> target;

			public:
				ButtleTarget(
					const std::shared_ptr<GameObject>& requester,
					const std::shared_ptr<GameObject>& target,
					const float value
				);
			};

			//--------------------------------------------------------------------------------------
			/// 通知用データ管理クラス
			//--------------------------------------------------------------------------------------
			class NotifyController {
			private:
				std::function<void()> func;		//呼び出す処理
				std::function<bool()> isCall;	//呼び出す条件

			public:
				NotifyController(
					const std::function<void()>& func,
					const std::function<bool()>& isCall = []() { return true; }
				);

				void Invoke();
			};

			//--------------------------------------------------------------------------------------
			/// タプルスペース
			//--------------------------------------------------------------------------------------
			class TupleSpace
			{
			public:
				using NotifyControllers = std::vector<std::shared_ptr<NotifyController>>;

			private:
				//書き込まれた情報一覧
				std::unordered_map<type_index, std::vector<std::shared_ptr<I_Tuple>>> m_tuplesMap;

				//NotipyMap通知を登録する一覧
				std::unordered_map<type_index, NotifyControllers> m_notifysMap;

			public:
				virtual ~TupleSpace() = default;

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// メッセージを書き込む
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T> &&
						std::is_constructible_v<T, Ts...>,
					std::nullptr_t> = nullptr
				>
				void Write(Ts&&... params) 
				{
					auto typeIndex = type_index(typeid(T));
					auto newTuple = std::make_shared<T>(params...);

					//同じ情報なら書き込まない
					if (m_tuplesMap.count(typeIndex) != 0) {	//Mapに存在する(存在しないなら、新規のため、同じかどうかの判断がいらない。)
						for (auto& tuple : m_tuplesMap[typeIndex]) {
							auto tTuple = dynamic_pointer_cast<T>(tuple);
							if (!tTuple) {	//キャストできないなら
								continue;
							}

							//同一の書き込みなら、書き込まない
							if (*tTuple.get() == *newTuple.get()) {
								return;
							}
						}
					}

					//型を入れる。
					m_tuplesMap[typeIndex].push_back(newTuple);

					//登録された通知を呼び出す。
					CallNotifys(typeIndex);
				}

				/// <summary>
				/// メッセージを読み込む(メッセージは削除しない)
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				const std::shared_ptr<const T> Read(
						const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; } ) const
				{
					return SearchTuple<T>(isFunc);
				}

				/// <summary>
				/// メッセージを受信して、削除する。
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>, 
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> Take(
						const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; }
				) {
					auto tuple = SearchTuple<T>(isFunc);
					if (!tuple) {	//タプルが存在しないならnullptr
						return nullptr;
					}

					//削除する
					RemoveTuple<T>(tuple);

					return tuple;
				}

				/// <summary>
				/// メッセージの取得を確定して、削除する。
				/// </summary>
				/// <param name="takeTuple">取得するタプルのポインタ</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> Take(const std::shared_ptr<T>& takeTuple)
				{
					auto typeIndex = type_index(typeid(T));	//型インデックスの取得

					if (m_tuplesMap.count(typeIndex) == 0) {	//Mapに存在しないならnullptrを返す。
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(typeIndex);	//その型のタプルを取得
					for (auto& tuple : tuples) {
						if (tuple == takeTuple) {	//まだタプルが存在するなら
							//削除する
							RemoveTuple<T>(takeTuple);

							return takeTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// テンプレートに一致する命令が受信されたら、通知を受け取る。
				/// </summary>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>	//基底クラスの制約
				void Notify(
					const std::function<void()>& func, 
					const std::function<bool()>& isCall = []() { return true; }
				) {
					auto typeIndex = type_index(typeid(T));	//型インデックスの取得

					auto newNotify = std::make_shared<NotifyController>(func, isCall);
					m_notifysMap[typeIndex].push_back(newNotify);	//Notipyの生成
				}

			private:
				/// <summary>
				/// 登録された通知を呼び出す。
				/// </summary>
				/// <param name="typeIndex">タイプインデックス</param>
				void CallNotifys(const type_index typeIndex);

				/// <summary>
				/// 登録された通知を呼び出す。
				/// </summary>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				void CallNotifys() {
					auto typeIndex = type_index(typeid(T));
					CallNotifys(typeIndex);
				}

				/// <summary>
				/// タプルを検索して返す。
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> SearchTuple(const std::function<bool(const std::shared_ptr<T>&)>& isFunc) const
				{
					auto tIndex = type_index(typeid(T));	//型インデックスの取得

					if (m_tuplesMap.count(tIndex) == 0) {	//Mapに存在しないならnullptrを返す。
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(tIndex);	//その型のタプル配列を取得
					for (auto& i_tupe : tuples) {
						auto tTuple = dynamic_pointer_cast<T>(i_tupe);
						if (!tTuple) {	//キャストできなかったら処理を飛ばす
							continue;
						}

						////条件式が一緒なら。
						if (isFunc(tTuple)) {
							return tTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// タプルの削除
				/// </summary>
				/// <param name="removeTuple">削除したいタプル</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				bool RemoveTuple(const std::shared_ptr<T>& removeTuple) 
				{
					auto tIndex = type_index(typeid(T));	//型インデックスの取得

					if (m_tuplesMap.count(tIndex) == 0) {		//Mapに存在しないならfalseを返す。
						return false;
					}

					auto& tuples = m_tuplesMap.at(tIndex);

					auto iter = tuples.begin();
					while (iter != tuples.end()) {
						if (*iter == removeTuple) {
							iter = tuples.erase(iter);
							return true;
						}
						iter++;
					}

					return false;
				}

			};

		}
	}
}