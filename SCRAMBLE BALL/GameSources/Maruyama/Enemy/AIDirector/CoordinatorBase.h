
/*!
@file CoordinatorBase.h
@brief CoordinatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "TupleSpace.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class I_FactionMember;

		namespace Tuple {
			class TupleSpace;
		}

		//--------------------------------------------------------------------------------------
		/// 調整者の基底クラス
		//--------------------------------------------------------------------------------------
		template<class T>
		class CoordinatorBase : public std::enable_shared_from_this<CoordinatorBase<T>>
		{
		private:
			std::vector<std::weak_ptr<T>> m_members;			//登録されたメンバー配列

			std::shared_ptr<Tuple::TupleSpace> m_tupleSpace;	//タプルスペース

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
				m_members(members),
				m_tupleSpace(new Tuple::TupleSpace())
			{}
			
			virtual ~CoordinatorBase() = default;

			/// <summary>
			/// 生成時に呼び出す処理
			/// </summary>
			virtual void OnCreate() {}

			/// <summary>
			/// 開始時に呼び出す処理(現在は生成時に呼び出している。)
			/// </summary>
			virtual void OnStart() {}

			virtual bool OnUpdate() = 0;

			/// <summary>
			/// 終了時に呼び出したい処理
			/// </summary>
			virtual void OnExit() {}

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// タプルスペースの取得
			/// </summary>
			/// <returns>タプルスペース</returns>
			std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept { return m_tupleSpace; }

			/// <summary>
			/// メンバーの配列を取得
			/// </summary>
			/// <returns>メンバー配列</returns>
			std::vector<std::weak_ptr<T>> GetMembers() const noexcept { return m_members; }

			/// <summary>
			/// メンバーの追加
			/// </summary>
			/// <param name="member">メンバー</param>
			virtual void AddMember(const std::shared_ptr<T>& member) { m_members.push_back(member); }

			/// <summary>
			/// メンバーの削除
			/// </summary>
			/// <param name="member">メンバー</param>
			/// <returns>削除に成功したらtrue</returns>
			virtual bool RemoveMember(const std::shared_ptr<T>& member) {
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

			template<class T>
			std::shared_ptr<T> GetThis() {
				auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
				if (Ptr) {
					return Ptr;
				}
				else {
					wstring str(L"thisを");
					str += Util::GetWSTypeName<T>();
					str += L"型にキャストできません";
					throw BaseException(
						str,
						L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
						L"ObjectInterface::GetThis()"
					);
				}
				return nullptr;
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

			virtual ~HereOwnerCoordinatorBase() = default;

			virtual void OnCreate() override {  
				for (const std::weak_ptr<MemberType>& member : GetMembers()) {
					SettingMember(member.lock());
				}
			}

		protected:

			virtual void SettingMember(const std::shared_ptr<I_FactionMember>& member) {
				member->SetFactionCoordinator(GetOwner());
				member->SetAssignFaction(GetThis<CoordinatorBase<MemberType>>());
			}

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 所有者の取得
			/// </summary>
			/// <returns>所有者</returns>
			std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); } 

			/// <summary>
			/// メンバーの追加
			/// </summary>
			/// <param name="member">追加したいメンバー</param>
			virtual void AddMember(const std::shared_ptr<MemberType>& member) override {
				CoordinatorBase::AddMember(member);
				SettingMember(member);
			}
		};

	}
}