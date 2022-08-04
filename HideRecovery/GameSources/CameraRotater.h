#pragma once

namespace basecross
{
	/// <summary>
	/// カメラの回転を担当するコンポーネント
	/// </summary>
	class CameraRotater : public Component
	{
		/// <summary>
		/// 最小の回転の角度
		/// </summary>
		float m_minRotX = -XM_PIDIV2;
		/// <summary>
		/// 最大の回転の角度
		/// </summary>
		float m_maxRotX = XM_PIDIV2;

		float m_rotX = 0.0f;
	public:
		CameraRotater(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// X軸の回転の最大値を設定する
		/// </summary>
		/// <param name="maxRotX">X軸回転の最大値</param>
		void SetMaxRotX(const float maxRotX);
		/// <summary>
		/// X軸の回転の最大値を取得する
		/// </summary>
		/// <returns>X軸の回転の最大値</returns>
		float GetMaxRotX() const;

		/// <summary>
		/// X軸の最小の回転値を設定する
		/// </summary>
		/// <param name="minRotX">X軸の最小の回転値</param>
		void SetMinRotX(const float minRotX);
		/// <summary>
		/// X軸の回転の最小値を取得する
		/// </summary>
		/// <returns>X軸の回転の最小値</returns>
		float GetMinRotX() const;

		void OnUpdate() override;
	};
}