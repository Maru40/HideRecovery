
/*!
@file Utility.h
@brief Utilityクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru{

		//--------------------------------------------------------------------------------------
		///	便利関数まとめ
		//--------------------------------------------------------------------------------------
		class Utility
		{
		public:
			static constexpr float FOWARD_ADJUST = -XM_PIDIV2;	//Mayaのデータを正面に向かせる角度

			/// <summary>
			/// ステージの取得
			/// </summary>
			/// <returns>ステージ</returns>
			static std::shared_ptr<Stage> GetStage();

			/// <summary>
			/// 対象が正面にいるかどうか
			/// </summary>
			/// <param name="target">対象のオブジェクト</param>
			/// <param name="self">自分自身のオブジェクト</param>
			/// <returns>正面ならtrue</returns>
			static bool IsTargetInFront(const std::shared_ptr<GameObject>& target, const std::shared_ptr<GameObject>& self);

			/// <summary>
			/// 対象が正面にいるかどうか
			/// </summary>
			/// <param name="targetPosition">対象の位置</param>
			/// <param name="selfPosition">自分自身の位置</param>
			/// <param name="selfForward">自分自身の正面方向</param>
			/// <returns></returns>
			static bool IsTargetInFront(const Vec3& targetPosition, const Vec3& selfPosition, const Vec3& selfForward);

			/// <summary>
			/// ターゲットが円範囲内にいるかどうか
			/// </summary>
			/// <param name="selfTarget">自分自身</param>
			/// <param name="target">ターゲット</param>
			/// <param name="range">範囲</param>
			/// <returns>ターゲットが範囲内ならtrue</returns>
			static bool IsInTargetCircleRange(const std::shared_ptr<GameObject>& selfTarget, const std::shared_ptr<GameObject>& target, const float& range);

			//一つ目の引数の方向に向く
			//※y軸に対応してないため、yは0で渡してください。
			//対応次第コメ書き消します。
			static void SetLookDirect(const Vec3& direct, const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// ターゲットに向かうベクトルを計算してくれる。
			/// </summary>
			/// <param name="self">自分自身のポインタ</param>
			/// <param name="target">ターゲットのポインタ</param>
			/// <returns>ターゲットに向かうベクトル</returns>
			static Vec3 CalcuToTargetVec(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target);

			/// <summary>
			/// 複数の方向の中で一番近い方向を取得
			/// </summary>
			/// <param name="stdDirect">基準となる方向</param>
			/// <param name="directs"></param>
			/// <returns></returns>
			static Vec3 CalculateNearDirect(const Vec3& stdDirect, const std::vector<Vec3>& directs);
			static Vec3 CalculateNearDirect(const Vec3& stdDreict,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets);

			/// <summary>
			/// 複数ターゲットの中で範囲方向のターゲットを取得
			/// </summary>
			/// <param name="stdDirect">基準となる方向</param>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="targets">検索したいターゲット配列</param>
			/// <param name="degree">範囲とするデグリー角</param>
			/// <returns>範囲内のターゲットを配列で取得</returns>
			static std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsDegree(const Vec3& stdDirect,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
				const float& degree);

			/// <summary>
			/// 複数ターゲットの中で範囲方向のターゲットを取得
			/// </summary>
			/// <param name="stdDirect">基準となる方向</param>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="targets">検索したいターゲット配列</param>
			/// <param name="degree">範囲とするラジアン角</param>
			/// <returns>範囲内のターゲットを配列で取得</returns>
			static std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsRadian(const Vec3& stdDirect,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
				const float& radian);

			/// <summary>
			/// 二つのベクトルの角度差が範囲内かどうか判断
			/// </summary>
			/// <param name="first">ベクトル</param>
			/// <param name="second">ベクトル</param>
			/// <param name="degree">デグリー角度差</param>
			/// <returns>角度差が範囲内ならtrue</returns>
			static bool IsInAngleDegree(const Vec3& first, const Vec3& second, const float& degree);

			/// <summary>
			/// 二つのベクトルの角度差が範囲内かどうか判断
			/// </summary>
			/// <param name="first">ベクトル</param>
			/// <param name="second">ベクトル</param>
			/// <param name="degree">ラジアン角度差</param>
			/// <returns>角度差が範囲内ならtrue</returns>
			static bool IsInAngleRadian(const Vec3& first, const Vec3& second, const float& radian);

			/// <summary>
			/// 正面方向に合わせたoffset変換
			/// </summary>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="offset">変換したいオフセット</param>
			/// <returns>正面方向に合わせて変換したoffset</returns>
			static Vec3 ConvertForwardOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset);

			/// <summary>
			/// 正面方向に合わせたoffset変換
			/// </summary>
			/// <param name="forward">正面方向</param>
			/// <param name="offset">変換したいオフセット</param>
			/// <returns>正面方向に合わせて変換したoffset</returns>
			static Vec3 ConvertForwardOffset(const Vec3& forward, const Vec3& offset);

			/// <summary>
			/// 指定したoffset分に回転させたQuatを返す
			/// </summary>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="offset">オフセット値</param>
			/// <returns>回転させたQuat</returns>
			static Quat ConvertRotationOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset);

			/// <summary>
			/// 指定した方向を向くQuatを返す。
			/// </summary>
			/// <param name="forward">向きたい方向</param>
			/// <param name="nowPosition">現在のポジション</param>
			/// <returns>指定した方向を向くQuat</returns>
			static Quat ConvertForwardToQuat(const Vec3& forward, const Vec3& nowPosition);

			/// <summary>
			/// ArrowObjectを生成する
			/// </summary>
			/// <param name="startPosition">Arrowの開始位置</param>
			/// <param name="endPosition">Arrowの終了位置</param>
			/// <param name="stage">生成するステージ</param>
			/// <returns>生成したArrowObject</returns>
			static std::shared_ptr<GameObject> CreateArrow(const Vec3& startPosition, const Vec3& endPosition, const std::shared_ptr<Stage>& stage);

			/// <summary>
			/// スタックのクリア
			/// </summary>
			/// <param name="clearStack">クリアしたいスタック</param>
			template<class T>
			static void StackClear(std::stack<T>& clearStack) {
				while (!clearStack.empty()) {
					clearStack.pop();
				}
			}

			/// <summary>
			/// Rayを飛ばしたとき、障害物があったらtrueを返す。
			/// </summary>
			/// <param name="startObj">Rayの開始位置のオブジェクト</param>
			/// <param name="endObj">Rayの終了位置のオブジェクト</param>
			/// <returns>障害物があったらtrue</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj);

			/// <summary>
			/// Rayを飛ばしたとき、障害物があったらtrueを返す。
			/// </summary>
			/// <param name="startPos">Rayの開始位置</param>
			/// <param name="endPos">Rayの終了位置</param>
			/// <returns>障害物があったらtrue</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos);

			/// <summary>
			/// 対象のオブジェクトまでに障害物があるかどうか
			/// </summary>
			/// <param name="startObj">Rayのスタート</param>
			/// <param name="endObj">Rayを飛ばす対象</param>
			/// <param name="excluteObj">障害物として省く対象</param>
			/// <returns>障害物が合ったらtrue</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj,
				const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// 対象のオブジェクトまでに障害物があるかどうか
			/// </summary>
			/// <param name="startPos">Rayのスタート</param>
			/// <param name="endPos">Rayを飛ばす対象</param>
			/// <param name="excluteObjs">障害物として省く対象</param>
			/// <returns>障害物が合ったらtrue</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// 対象オブジェクトまでに障害物があるかどうか
			/// </summary>
			/// <param name="startObj">Rayのスタート</param>
			/// <param name="endObj">対象オブジェクト</param>
			/// <param name="obstacleObjs">障害物の対象になるオブジェクト</param>
			/// <param name="excluteObjs">障害物の対象になるオブジェクトの中でも特定のオブジェクトを対象外にしたい場合に記述</param>
			/// <returns>障害物が合ったらtrue</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj,
				const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// 対象オブジェクトまでに障害物があるかどうか
			/// </summary>
			/// <param name="startPos">Rayのスタート</param>
			/// <param name="endPos">対象オブジェクト</param>
			/// <param name="obstacleObjs">障害物の対象になるオブジェクト</param>
			/// <param name="excluteObjs">障害物の対象になるオブジェクトの中でも特定のオブジェクトを対象外にしたい場合に記述</param>
			/// <returns>障害物が合ったらtrue</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// templateで指定されたクラスを覗いて、障害物判定を取る関数。
			/// </summary>
			/// <param name="startObj">Rayの開始位置のオブジェクト</param>
			/// <param name="endObj">Rayの終了位置のオブジェクト</param>
			/// <returns>障害物があったらtrue</returns>
			template<class ExcluteClass>
			static bool IsRayObstacleExclute(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj)
			{
				auto exclutes = maru::Utility::GetComponents<ExcluteClass>();
				return IsRayObstacle(startObj, endObj, exclutes);
			}

			/// <summary>
			/// templateで指定したコンポーネントのついているオブジェクトを指定障害物判定して探す。
			/// </summary>
			/// <param name="startObj">Rayの開始位置のオブジェクト</param>
			/// <param name="endObj">Rayの終了位置のオブジェクト</param>
			/// <returns>障害物があったらtrue</returns>
			template<class ObstacleClass>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj)
			{
				auto obstacleObjs = maru::Utility::GetComponents<ObstacleClass>();

				return IsRayObstacle(startObj, endObjs, obstacleObjs, vector<shared_ptr<GameObject>>());
			}

			/// <summary>
			/// 特定のオブジェクトがRay状に合ったらtrueを返す。
			/// </summary>
			/// <param name="startObj">Rayの開始</param>
			/// <param name="endObj">Rayの終わり</param>
			/// <param name="obstacleObj">間にあるかどうか判断したいオブジェクト</param>
			/// <returns>Ray状に合ったらtrue</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj,
				const std::shared_ptr<GameObject>& obstacleObj);
			
			/// <summary>
			/// 対象の障害物が二つのRayの間に合ったらtrue
			/// </summary>
			/// <param name="startPos">Rayの開始位置</param>
			/// <param name="endPos">Rayの終了位置</param>
			/// <param name="obstacleObj">障害物のオブジェクト</param>
			/// <returns></returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const std::shared_ptr<GameObject>& obstacleObj);

			/// <summary>
			/// 対象のオブジェクトが対象外かどうか判断する
			/// </summary>
			/// <param name="targetObj">対象のオブジェクト</param>
			/// <param name="excluteObjs">対象外となる配列オブジェクト</param>
			/// <returns>対象外ならtrue</returns>
			static bool IsExclute(const shared_ptr<GameObject>& targetObj ,const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// 対象のポジションからどれだけ離れているかを返す
			/// </summary>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="targetPosition">ターゲットのポジション</param>
			/// <returns>ターゲットからどれだけ離れているか</returns>
			static float CalcuSubRange(const std::shared_ptr<GameObject>& selfObject, const Vec3& targetPosition);

			/// <summary>
			/// カメラに合わせた方向に合わせる
			/// </summary>
			/// <param name="input"></param>
			/// <param name="camera"></param>
			/// <returns></returns>
			static Vec3 CalcuCameraVec(const Vec3& input, const std::shared_ptr<Camera>& camera, const std::shared_ptr<GameObject>& selfObject);

			/// <summary>
			/// World座標をScrren座標に変換
			/// </summary>
			/// <param name="view">ビューポインタ</param>
			/// <param name="position">変換したいポジション</param>
			/// <returns></returns>
			Vec3 ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position);

			/// <summary>
			/// 近くのオブジェクトを探す。
			/// </summary>
			/// <param name="objects">検索するオブジェクト配列</param>
			/// <param name="selfPosition">自分自身のオブジェクト</param>
			/// <returns>一番近いオブジェクトの取得</returns>
			template<class T>
			static T CalcuNearObject(const vector<T>& objects, const Vec3& selfPosition) {
				float minLength = 9999999.0f;
				T reObject;

				for (auto& object : objects) {
					auto toVec = object->GetComponent<Transform>()->GetPosition() - selfPosition;
					auto newLength = toVec.length();

					if (newLength < minLength) {  //小さかったら
						reObject = object;
						minLength = newLength;
					}
				}

				return reObject;
			}

			/// <summary>
			/// ゲームオブジェクトVecから指定のオブジェクトのみ取得
			/// 最初に取得できたものだけ返す。
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindGameObject() {
				auto stage = GetStage();

				return FindGameObject<T>(stage);
			}

			/// <summary>
			/// ゲームオブジェクトVecから指定のオブジェクトのみ取得
			/// 最初に取得できたものだけ返す。
			/// </summary>
			/// <param name="stage">検索するステージ</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindGameObject(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			
			/// <summary>
			/// ゲームオブジェクトVecから指定のオブジェクトのみ取得
			/// ベクター配列として見つけたもの全て返す。
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindGameObjects() {
				auto stage = GetStage();

				return FindGameObjects<T>(stage);
			}

			
			/// <summary>
			/// ゲームオブジェクトVecから指定のオブジェクトのみ取得
			/// ベクター配列として見つけたもの全て返す。
			/// </summary>
			/// <param name="stage">検索するステージ</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindGameObjects(const std::shared_ptr<Stage>& stage) {
				vector<shared_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// 指定したコンポーネントを見つける
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindComponent() {
				auto stage = GetStage();

				return FindComponent<T>(stage);
			}

			/// <summary>
			/// 指定したコンポーネントを見つける
			/// </summary>
			/// <param name="stage">検索するステージ</param>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindComponent(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			/// <summary>
			/// 指定したコンポ―ネントを複数見つける
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindComponents() {
				auto stage = GetStage();
				return FindComponents<T>(stage);
			}

			/// <summary>
			/// 指定したコンポ―ネントを複数見つける
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindComponents(const std::shared_ptr<Stage>& stage) {
				vector<shared_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// コンポーネントのアタッチされたオブジェクトを取得する
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
				static vector<shared_ptr<GameObject>> FindComponentObjects() {
				auto stage = GetStage();

				vector<shared_ptr<GameObject>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t->GetGameObject());
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// 指定したコンポーネントを全て見つけて、weak_ptrにして返す
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<std::weak_ptr<T>> FindWeakPtrComponents() {
				auto stage = GetStage();

				vector<std::weak_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// 指定したコンポーネントを全て見つけて、ex_weak_ptrにして返す
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
				static vector<ex_weak_ptr<T>> FindExWeakPtrComponents() {
				auto stage = GetStage();

				vector<ex_weak_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// 指定したインターフェースを見つける
			/// </summary>
			/// <param name="stage">検索するステージ</param>
			template<class T>
			static std::shared_ptr<T> FindInterface(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			/// <summary>
			/// 渡された配列にtemplateで指定したオブジェクト型のオブジェクトを追加する。
			/// </summary>
			/// <param name="addVec">追加したいオブジェクトの配列</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static void AddObjects(vector<shared_ptr<GameObject>>& addVec) {
				auto stage = GetStage();

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						addVec.push_back(t);  //オブジェクトの追加
					}
				}
			}

			/// <summary>
			/// 渡された配列にtemplateで指定したコンポーネントを持つオブジェクトを追加する。
			/// </summary>
			/// <param name="addVec">追加したいオブジェクトの配列</param>
			template<class T ,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static void AddComponents(vector<shared_ptr<GameObject>>& addVec) {
				auto stage = GetStage();

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						addVec.push_back(obj);  //オブジェクトの追加
					}
				}
			}

			/// <summary>
			/// ベクター配列から特定のオブジェクトを削除する関数。
			/// </summary>
			/// <param name="vec">削除したいデータのある配列</param>
			/// <param name="removePtr">削除したいデータ</param>
			template<class T>
			static bool RemoveVec(vector<T>& vec, const T& removePtr) {
				auto iter = vec.begin();
				while (iter != vec.end()) {
					if (*iter == removePtr) {
						iter = vec.erase(iter);
						return true;
						//break;
					}
					iter++;
				}

				return false;
			}

			/// <summary>
			/// すでに配列に入っているか判断
			/// </summary>
			/// <returns>すでに同じものが入っているならtrue</returns>
			template<class T>
			static bool IsSameVec(const vector<T>& vec, const T& newPtr) {
				for (auto& t : vec) {
					if (t == newPtr) {
						return true;
					}
				}

				return false;
			}

			/// <summary>
			/// 配列を別の型にキャストして入れなおして返す。
			/// </summary>
			/// <param name="vector">入れなおしたい配列</param>
			template<class ConvertType, class OriginalType>
			static std::vector<std::shared_ptr<ConvertType>> ConvertArrayType(
				const std::vector<std::shared_ptr<OriginalType>>& vector
			) {
				std::vector<std::shared_ptr<ConvertType>> result;

				for (const auto& value : vector) {
					auto convert = dynamic_pointer_cast<ConvertType>(value);
					if (convert) {
						result.push_back(convert);
					}
				}

				return result;
			}

			/// <summary>
			/// shared_ptrの配列をweak_ptr配列に変換する
			/// </summary>
			/// <param name="vector">変換したい配列</param>
			template<class T>
			static std::vector<std::weak_ptr<T>> ConvertArraySharedToWeak(const std::vector<std::shared_ptr<T>>& vector) {
				std::vector<std::weak_ptr<T>> result;

				for (auto& value : vector) {
					result.push_back(value);
				}

				return result;
			}

			/// <summary>
			/// shared_ptrの配列をweak_ptr配列に変換する
			/// </summary>
			/// <param name="vector">変換したい配列</param>
			template<class T>
			static std::vector<ex_weak_ptr<T>> ConvertArraySharedToExweak(const std::vector<std::shared_ptr<T>>& vector) {
				std::vector<ex_weak_ptr<T>> result;

				for (auto& value : vector) {
					result.push_back(value);
				}

				return result;
			}

			/// <summary>
			/// Arrayの長さを知る
			/// </summary>
			template<class T, std::size_t SIZE>
			static std::size_t Array_Length(const T(&array)[SIZE])
			{
				return SIZE;
			}
		};
	}

	

}

//endbasecross