
/*!
@file CoordinatorBase.h
@brief CoordinatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 調整者の基底クラス
		//--------------------------------------------------------------------------------------
		template<class T>
		class CoordinatorBase
		{
		private:
			std::vector<std::weak_ptr<T>> m_members;	//登録されたメンバー配列

		public:
			CoordinatorBase():
				CoordinatorBase(std::vector<std::weak_ptr<T>>())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="member">所属するメンバー</param>
			CoordinatorBase(const std::shared_ptr<T>& member):
				CoordinatorBase(std::vector<std::weak_ptr<T>>(1, member))
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="members">所属するメンバー一覧</param>
			CoordinatorBase(const vector<std::weak_ptr<T>>& members) :
				m_members(members)
			{}
			
			virtual ~CoordinatorBase() = default;

			virtual void OnStart() {}
			virtual void OnUpdate() {}
			virtual void OnExit() {}

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// メンバーの配列を取得
			/// </summary>
			/// <returns>メンバー配列</returns>
			std::vector<std::weak_ptr<T>> GetMembers() const noexcept { return m_members; }

			/// <summary>
			/// メンバーの追加
			/// </summary>
			/// <param name="member">メンバー</param>
			void AddMember(const std::shared_ptr<T>& member) { m_members.push_back(member); }

			/// <summary>
			/// メンバーの削除
			/// </summary>
			/// <param name="member">メンバー</param>
			/// <returns>削除に成功したらtrue</returns>
			bool RemoveMember(const std::shared_ptr<T>& member) {
				auto iter = m_members.begin();
				while (iter != m_members.end()) {
					if (iter->lock() == member) {
						iter = m_members.erase(iter);
						return true;
					}
					iter++;
				}

				return false;
			}

		};


		//--------------------------------------------------------------------------------------
		/// 所有者が存在する調整者の基底クラス
		//--------------------------------------------------------------------------------------
		template<class OwnerType, class MemberType>
		class HereOwnerCoordinatorBase : public CoordinatorBase<MemberType>
		{
			std::weak_ptr<OwnerType> m_owner;	//所有者

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner):
				HereOwnerCoordinatorBase(owner, std::vector<std::weak_ptr<MemberType>>())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			/// <param name="member">アサインするメンバー</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner, const std::shared_ptr<MemberType>& member) :
				HereOwnerCoordinatorBase(owner, std::vector<std::weak_ptr<OwnerType>>(1, member))
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			/// <param name="members">アサインするメンバー一覧</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner, const vector<std::weak_ptr<MemberType>>& members) :
				CoordinatorBase(members), m_owner(owner)
			{}

			~HereOwnerCoordinatorBase() = default;

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 所有者の取得
			/// </summary>
			/// <returns>所有者</returns>
			std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); }
		};

	}
}