
/*!
@file TupleSpace.h
@brief TupleSpaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace Tuple {

			//--------------------------------------------------------------------------------------
			/// タプルのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Tuple {
			public:
				virtual ~I_Tuple() = default;
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
			};

			//--------------------------------------------------------------------------------------
			/// タプルスペース
			//--------------------------------------------------------------------------------------
			class TupleSpace
			{
			private:
				//書き込まれた情報一覧
				std::unordered_map<type_index, std::vector<std::shared_ptr<I_Tuple>>> m_tuplesMap;

			public:
				virtual ~TupleSpace() = default;

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// メッセージを書き込む
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				void Write(Ts&&... params) 
				{
					auto tIndex = type_index(typeid(T));
					auto tuple = std::make_shared<T>(params...);

					//型が一致するか判断

					//型を入れる。
					m_tuplesMap[tIndex].push_back(tuple);
				}

				/// <summary>
				/// メッセージを読み込む(メッセージは削除しない)
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				const std::shared_ptr<const T> Read(
						const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; }) const
				{
					return SearchTuple<T>(isFunc);
				}

				/// <summary>
				/// メッセージを受信して、削除する。
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
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
					auto tIndex = type_index(typeid(T));	//型インデックスの取得

					if (m_tuplesMap.count(tIndex) == 0) {	//Mapに存在しないならnullptrを返す。
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(tIndex);	//その型のタプルを取得
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
				/// テンプレートに一致する命令が受信されたら、通知を受け取る(現在仕様不可)
				/// </summary>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				void Notify() 
				{

				}

			private:
				/// <summary>
				/// タプルを検索して返す。
				/// </summary>
				/// <param name="isFunc">取得条件</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> SearchTuple(const std::function<bool(const std::shared_ptr<T>&)>& isFunc) const
				{
					auto tIndex = type_index(typeid(T));	//型インデックスの取得

					if (m_tuplesMap.count(tIndex) == 0) {		//Mapに存在しないならnullptrを返す。
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(tIndex);		//その型のタプルを取得
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