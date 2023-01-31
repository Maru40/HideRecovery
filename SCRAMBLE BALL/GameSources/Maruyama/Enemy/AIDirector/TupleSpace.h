
/*!
@file TupleSpace.h
@brief TupleSpace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Damaged.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace maru {
		template<class T>
		class Action;
	}

	class I_TeamMember;
	class TargetManager;

	namespace Enemy {

		class I_FactionMember;

		namespace Tuple {

			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			class TupleSpace;

			//--------------------------------------------------------------------------------------
			/// �^�v���X�y�[�X���g���҂̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Tupler {
			public:
				_NODISCARD virtual std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �^�v���̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Tuple {
			public:
				virtual ~I_Tuple() = default;

				virtual bool operator== (const I_Tuple& other);

				//template<class T>
				//bool operator!= (const T& other) = 0;

				virtual std::shared_ptr<I_Tupler> GetRequester() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ��]�s���̊��N���X
			//--------------------------------------------------------------------------------------
			class TupleActionBase : public I_Tuple
			{
			public:
				std::weak_ptr<GameObject> acter;		//�s�������
				std::weak_ptr<GameObject> requester;	//�Ώێ�
				float value;							//�]���l

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="acter">�s�������</param>
				/// <param name="requester">�Ώێ�</param>
				/// <param name="value">�]���l</param>
				TupleActionBase(const std::shared_ptr<GameObject>& acter, const std::shared_ptr<GameObject>& requester, const float value);

				virtual ~TupleActionBase() = default;
			};

			//--------------------------------------------------------------------------------------
			/// ���N�G�X�g�̊��N���X
			//--------------------------------------------------------------------------------------
			class TupleRequestBase : public I_Tuple
			{
				std::weak_ptr<I_Tupler> m_requester;	//�v�����o������
				float m_value;							//�]���l

			public:

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="requester">�v�����o������</param>
				/// <param name="value">�]���l</param>
				TupleRequestBase(const std::shared_ptr<I_Tupler>& requester, const float value);

				virtual ~TupleRequestBase() = default;

				virtual bool operator== (const TupleRequestBase& other);

				std::shared_ptr<I_Tupler> GetRequester() const override { return m_requester.lock(); }

				float GetValue() const { return m_value; }
			};

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�����������Ƃ�m�点��^�v��
			//--------------------------------------------------------------------------------------
			class FindTarget : public TupleRequestBase
			{
				std::weak_ptr<GameObject> m_target;
			public:

				FindTarget(const std::shared_ptr<I_Tupler>& requester, const std::shared_ptr<GameObject>& target, const float value = 1.0f);

				virtual ~FindTarget() = default;

				bool operator== (const FindTarget& other);

				std::shared_ptr<GameObject> GetTarget() const { return m_target.lock(); };
			};

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�ƃo�g�����邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------
			class ButtleTarget : public TupleRequestBase 
			{
				std::weak_ptr<GameObject> target;

			public:
				ButtleTarget(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<GameObject>& target,
					const float value
				);

				virtual ~ButtleTarget() = default;

				bool operator==(const ButtleTarget& other);

				std::shared_ptr<GameObject> GetTarget() const noexcept { return target.lock(); };
			};

			//--------------------------------------------------------------------------------------
			/// �o�g���ɑJ�ڂ��邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------
			class ButtleTransition : public TupleRequestBase {
				std::weak_ptr<GameObject> m_target;

			public:
				ButtleTransition(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<GameObject>& target,
					const float value
				);

				bool operator==(const ButtleTransition& other);

				_NODISCARD std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }
			};

			//--------------------------------------------------------------------------------------
			/// �p�g���[���ɑJ�ڂ��邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------

			class PatrolTransition : public TupleRequestBase 
			{
			public:
				PatrolTransition(
					const std::shared_ptr<I_Tupler>& requester,
					const float value
				);

				bool operator==(const PatrolTransition& other);
			};

			//--------------------------------------------------------------------------------------
			/// �_���[�W���󂯂����Ƃ�`����^�v��
			//--------------------------------------------------------------------------------------
			class Damaged : public TupleRequestBase {
				DamageData m_damageData;

			public:
				Damaged(
					const std::shared_ptr<I_Tupler>& requester,
					const DamageData& data,
					const float value
				);

				bool operator ==(const Damaged& other);

				_NODISCARD const DamageData& GetDamageData() const noexcept { return m_damageData; };
			};

			//--------------------------------------------------------------------------------------
			///	Help�^�v��
			//--------------------------------------------------------------------------------------
			class HelpAction : public TupleRequestBase 
			{
				std::weak_ptr<GameObject> m_target;

			public:
				HelpAction(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<GameObject>& target,
					const float value
				);

				bool operator ==(const HelpAction& other);

				_NODISCARD std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }
			};

			//--------------------------------------------------------------------------------------
			///	FindBall�^�v��
			//--------------------------------------------------------------------------------------
			class FindBall : public TupleRequestBase 
			{
				std::weak_ptr<I_TeamMember> m_teamMember;	//�`�[�������o�[

			public:
				FindBall(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<I_TeamMember>& teamMember,
					const float value
				);

				bool operator ==(const FindBall& other);

				_NODISCARD std::shared_ptr<I_TeamMember> GetTeamMember() const noexcept;
			};

			//--------------------------------------------------------------------------------------
			///	Kill�^�v��
			//--------------------------------------------------------------------------------------
			class Kill : public TupleRequestBase
			{
				std::weak_ptr<GameObject> m_killer;	//�L�������l
				std::weak_ptr<GameObject> m_killed;	//�L�����ꂽ�l

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="requester">���M��</param>
				/// <param name="killer">�L�������l</param>
				/// <param name="killed">�L�����ꂽ�l</param>
				/// <param name="value"></param>
				Kill(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<GameObject>& killer,
					const std::shared_ptr<GameObject>& killed,
					const float value
				);

				bool operator==(const Kill& other);

				//�L�������l
				_NODISCARD std::shared_ptr<GameObject> GetKiller() const noexcept;

				//�L�����ꂽ�l
				_NODISCARD std::shared_ptr<GameObject> GetKilled() const noexcept;

			};

			//--------------------------------------------------------------------------------------
			///	�^�[�Q�b�g�������������Ƃ�ʒm
			//--------------------------------------------------------------------------------------

			class LostTarget : public TupleRequestBase
			{
				std::weak_ptr<TargetManager> m_targetManager;

			public:
				LostTarget(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<TargetManager>& targetManager,
					const float value
				);

				bool operator == (const LostTarget& other);

				//�^�[�Q�b�g�Ǘ��̎擾
				_NODISCARD std::shared_ptr<TargetManager> GetTargetManager() const noexcept { return m_targetManager.lock(); }
			};

			//--------------------------------------------------------------------------------------
			///	�^�[�Q�b�g�̌��������肢����^�v��
			//--------------------------------------------------------------------------------------
			class SearchTarget : public TupleRequestBase
			{
				std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�

			public:
				SearchTarget(
					const std::shared_ptr<I_Tupler>& requester,
					const std::shared_ptr<TargetManager>& targetManager,
					const float value
				);

				bool operator == (const SearchTarget& other);

				//�^�[�Q�b�g�Ǘ��̎擾
				_NODISCARD std::shared_ptr<TargetManager> GetTargetManager() const noexcept { return m_targetManager.lock(); }
			};

			//--------------------------------------------------------------------------------------
			/// �ʒm�p�f�[�^�Ǘ��C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_NotifyController {
			public:
				virtual ~I_NotifyController() = default;

				virtual _NODISCARD const std::shared_ptr<I_Tupler> GetRequester() const noexcept = 0;

				virtual bool operator==(const I_NotifyController& other) const { return GetRequester() == other.GetRequester(); };
			};

			//--------------------------------------------------------------------------------------
			/// �ʒm�p�f�[�^�Ǘ��N���X
			//--------------------------------------------------------------------------------------
			template<class T>
			class NotifyController : public I_NotifyController {
			private:
				const std::weak_ptr<I_Tupler> m_requester;				//���N�G�X�g�����{�l
				std::function<void(const std::shared_ptr<T>&)> func;	//�Ăяo������
				std::function<bool(const std::shared_ptr<T>&)> isCall;	//�Ăяo������

			public:
				NotifyController(
					const std::shared_ptr<I_Tupler> requester,
					const std::function<void(const std::shared_ptr<T>&)>& func,
					const std::function<bool(const std::shared_ptr<T>&)>& isCall = [](const std::shared_ptr<T>& tuple) { return true; }
				):
					m_requester(requester),
					func(func),
					isCall(isCall)
				{}

				virtual ~NotifyController() = default;

				void Invoke(const std::shared_ptr<T>& tuple) {
					if (isCall(tuple)) {
						func(tuple);
					}
				}

				_NODISCARD virtual const shared_ptr<I_Tupler> GetRequester() const noexcept { return m_requester.lock(); }
			};

			//--------------------------------------------------------------------------------------
			/// �^�v���X�y�[�X
			//--------------------------------------------------------------------------------------
			class TupleSpace
			{
			public:
				using NotifyControllers = std::vector<std::shared_ptr<I_NotifyController>>;

			private:
				//�������܂ꂽ���ꗗ
				std::unordered_map<type_index, std::vector<std::shared_ptr<I_Tuple>>> m_tuplesMap;

				//NotipyMap�ʒm��o�^����ꗗ
				std::unordered_map<type_index, NotifyControllers> m_notifysMap;

			public:
				virtual ~TupleSpace() = default;

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �������̃^�v���X�y�[�X���ǂ����𔻒f
				/// </summary>
				/// <returns>�������̃^�v���X�y�[�X�Ȃ�true</returns>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				bool IsSomeTuple(const std::shared_ptr<T>& newTuple) {
					auto typeIndex = type_index(typeid(T));

					if (m_tuplesMap.count(typeIndex) != 0) {	//Map�ɑ��݂���(���݂��Ȃ��Ȃ�A�V�K�̂��߁A�������ǂ����̔��f������Ȃ��B)
						for (auto& tuple : m_tuplesMap[typeIndex]) {
							auto tTuple = dynamic_pointer_cast<T>(tuple);
							if (!tTuple) {	//�L���X�g�ł��Ȃ��Ȃ�
								continue;
							}

							//����̏������݂Ȃ�A�������܂Ȃ�
							if (*tTuple.get() == *newTuple.get()) {
								return true;
							}
						}
					}

					return false;
				}

				/// <summary>
				/// ����Notify���ǂ���
				/// </summary>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				bool IsSomeNotify(const std::shared_ptr<I_NotifyController>& newNotify) {
					auto typeIndex = type_index(typeid(T));
					auto notifys = m_notifysMap[typeIndex];	//�^�v�����Ƃ̃}�b�v����A�o�^���ꂽNotify���擾

					for (const auto& notify : notifys) {
						//�������N�G�X�^�[�Ȃ�o�^�����Ȃ��B
						if ((*notify.get()) == (*newNotify.get())) {
							return true;
						}
					}

					return false;
				}

				/// <summary>
				/// ���b�Z�[�W����������
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

					//�������Ȃ珑�����܂Ȃ�
					if (IsSomeTuple(newTuple)) {
						//Debug::GetInstance()->Log(L"�^�v���̏d�����m�F");
						return;
					}

					//�^������B
					m_tuplesMap[typeIndex].push_back(newTuple);

					//�o�^���ꂽ�ʒm���Ăяo���B
					CallNotifys<T>(newTuple);
				}

				/// <summary>
				/// ���b�Z�[�W��ǂݍ���(���b�Z�[�W�͍폜���Ȃ�)
				/// </summary>
				/// <param name="isFunc">�擾����</param>
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
				/// ���b�Z�[�W����M���āA�폜����B
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>, 
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> Take(
					const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; }
				) {
					auto tuple = SearchTuple<T>(isFunc);
					if (!tuple) {	//�^�v�������݂��Ȃ��Ȃ�nullptr
						return nullptr;
					}

					//�폜����
					RemoveTuple<T>(tuple);

					return tuple;
				}

				/// <summary>
				/// ���b�Z�[�W�̎擾���m�肵�āA�폜����B
				/// </summary>
				/// <param name="takeTuple">�擾����^�v���̃|�C���^</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> Take(const std::shared_ptr<T>& takeTuple)
				{
					auto typeIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(typeIndex) == 0) {	//Map�ɑ��݂��Ȃ��Ȃ�nullptr��Ԃ��B
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(typeIndex);	//���̌^�̃^�v�����擾
					for (auto& tuple : tuples) {
						if (tuple == takeTuple) {	//�܂��^�v�������݂���Ȃ�
							//�폜����
							RemoveTuple<T>(takeTuple);

							return takeTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// ���b�Z�[�W��S�Ď�M���āA�폜����B
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				std::vector<std::shared_ptr<T>> Takes(
					const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; }
				) {
					std::vector<std::shared_ptr<T>> result;

					while (true) {	//�S�Ă̒ʒm���󂯎��܂ŏ���������B
						auto tuple = Take<T>(isFunc);
						if (!tuple) {
							break;	//��Ȃ炷�łɂ��̃^�v�������݂��Ȃ����߁A�I���B
						}

						result.push_back(tuple);
					}

					return result;
				}

				/// <summary>
				/// �e���v���[�g�Ɉ�v���閽�߂���M���ꂽ��A�ʒm���󂯎��B
				/// </summary>
				/// <param name="requester">�o�^��</param>
				/// <param name="func">�Ăяo����������</param>
				/// <param name="isCall">�Ăяo������</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>	//���N���X�̐���
				void Notify(
					const std::shared_ptr<I_Tupler> requester,
					const std::function<void(const std::shared_ptr<T>&)>& func, 
					const std::function<bool(const std::shared_ptr<T>&)>& isCall = [](const std::shared_ptr<T>& tuple) { return true; }
				) {
					auto typeIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					auto newNotify = std::make_shared<NotifyController<T>>(requester, func, isCall);

					if(IsSomeNotify<T>(newNotify)){	//�����ʒm�o�^�Ȃ�o�^���Ȃ��B
						return;
					}

					m_notifysMap[typeIndex].push_back(newNotify);	//Notipy�̐���
				}

				/// <summary>
				/// �����̒ʒm�������B
				/// </summary>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				bool RemoveNotify(const std::shared_ptr<I_Tupler>& tupler)
				{
					if (tupler == nullptr) {
						return false;
					}

					auto tIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_notifysMap.count(tIndex) == 0) {		//Map�ɑ��݂��Ȃ��Ȃ�false��Ԃ��B
						return false;
					}

					auto& notifys = m_notifysMap.at(tIndex);

					auto iter = notifys.begin();
					while (iter != notifys.end()) {
						auto test = *iter;
						if ((*iter)->GetRequester() == tupler) {
							iter = notifys.erase(iter);
							return true;
						}
						iter++;
					}

					return false;
				}

				/// <summary>
				/// �w�肵���^�v���X�y�[�X�g�p�҂̒ʒm��S�ď����B
				/// </summary>
				/// <param name="tupler">�^�v���X�y�[�X�g�p��</param>
				/// <returns></returns>
				bool RemoveAllNotifys(const std::shared_ptr<I_Tupler>& tupler);

				bool RemoveAllTuples(const std::shared_ptr<I_Tupler>& tupler);

			private:
				/// <summary>
				/// �o�^���ꂽ�ʒm���Ăяo���B
				/// </summary>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				void CallNotifys(const std::shared_ptr<T>& tuple) {
					auto typeIndex = type_index(typeid(T));
					auto notifys = m_notifysMap[typeIndex];

					//�S�Ă̒ʒm���Ăяo���B
					for (auto i_notify : notifys) {
						//�ʒm�̃L���X�g
						auto notify = std::dynamic_pointer_cast<NotifyController<T>>(i_notify);

						//�ʒm�̌Ăяo���B
						if (notify) {	
							notify->Invoke(tuple);
						}
					}
				}

				/// <summary>
				/// �^�v�����������ĕԂ��B
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> SearchTuple(const std::function<bool(const std::shared_ptr<T>&)>& isFunc) const
				{
					auto tIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(tIndex) == 0) {	//Map�ɑ��݂��Ȃ��Ȃ�nullptr��Ԃ��B
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(tIndex);	//���̌^�̃^�v���z����擾
					for (auto& i_tupe : tuples) {
						auto tTuple = dynamic_pointer_cast<T>(i_tupe);
						if (!tTuple) {	//�L���X�g�ł��Ȃ������珈�����΂�
							continue;
						}

						////���������ꏏ�Ȃ�B
						if (isFunc(tTuple)) {
							return tTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// �^�v���̍폜
				/// </summary>
				/// <param name="removeTuple">�폜�������^�v��</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				bool RemoveTuple(const std::shared_ptr<T>& removeTuple) 
				{
					auto tIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(tIndex) == 0) {	//Map�ɑ��݂��Ȃ��Ȃ�false��Ԃ��B
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

				bool RemoveNotify(const std::shared_ptr<I_NotifyController>& removeNotify) {
					for (auto& pair : m_notifysMap) {
						auto iter = pair.second.begin();
						while (iter != pair.second.end()) {
							if (*iter == removeNotify) {
								iter = pair.second.erase(iter);
								return true;
							}

							iter++;
						}
					}

					return false;
				}

			};

		}
	}
}