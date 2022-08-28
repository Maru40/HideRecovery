#include "stdafx.h"
#include "Utility.h"

namespace basecross {
	namespace Utility {
		const Vec2 GetTextureSize(const wstring& key)
		{
			auto resource = App::GetApp()->GetResource<TextureResource>(key);
			ID3D11Resource* id3d11resource;
			resource->GetShaderResourceView()->GetResource(&id3d11resource);
			D3D11_TEXTURE2D_DESC textureDesc;
			auto texture = static_cast<ID3D11Texture2D*>(id3d11resource);
			texture->GetDesc(&textureDesc);
			float width = (float)textureDesc.Width;
			float height = (float)textureDesc.Height;
			return Vec2(width, height);
		}

		const Vec2 ConvertToUVCoordinate(const Vec2& coordinate, const wstring& textureName) {
			return ConvertToUVCoordinate(coordinate, GetTextureSize(textureName));
		}

		const Vec2 ConvertToUVCoordinate(const Vec2& coordinate, const Vec2& textureSize) {
			Vec2 uv;
			uv.x = coordinate.x / textureSize.x;
			uv.y = coordinate.y / textureSize.y;
			return uv;
		}

		void ConvertToUVCoordinates(const vector<Vec2>& coordinates, const wstring& textureName, vector<Vec2>& uv) {
			ConvertToUVCoordinates(coordinates, GetTextureSize(textureName), uv);
		}

		void ConvertToUVCoordinates(const vector<Vec2>& coordinates, const Vec2& textureSize, vector<Vec2>& uv) {
			for (auto& coordinate : coordinates) {
				float x = coordinate.x / textureSize.x;
				float y = coordinate.y / textureSize.y;
				uv.push_back(Vec2(x, y));
			}
		}

		Vec3 ConvertDegVecToRadVec(const Vec3& deg) {
			Vec3 rad(
				XMConvertToRadians(deg.x),
				XMConvertToRadians(deg.y),
				XMConvertToRadians(deg.z)
			);
			return rad;
		}
		Vec3 ConvertRadVecToDegVec(const Vec3& rad) {
			Vec3 deg(
				XMConvertToDegrees(rad.x),
				XMConvertToDegrees(rad.y),
				XMConvertToDegrees(rad.z)
			);
			return deg;
		}

		Vec2 ConvertDegVecToRadVec(const Vec2& deg) {
			Vec2 rad(
				XMConvertToRadians(deg.x),
				XMConvertToRadians(deg.y)
			);
			return rad;
		}
		Vec2 ConvertRadVecToDegVec(const Vec2& rad) {
			Vec2 deg(
				XMConvertToDegrees(rad.x),
				XMConvertToDegrees(rad.y)
			);
			return deg;
		}

		bool MatchAngle(float deg1, float deg2) {
			if (ClampAngle(0.0f, 360.0f, deg1) == ClampAngle(0.0f, 360.0f, deg2)) {
				return true;
			}
			else {
				return false;
			}
		}

		float ClampAngle(float min, float max, float angle) {
			if (min >= max) {
				throw BaseException(
					L"引数が逆です。",
					L"min:" + to_wstring(min) + L" ,max:" + to_wstring(max),
					L"Utility::ClampAngle(float min, float max, float angle);"
				);
			}
			if (max - min != 360) {
				throw BaseException(
					L"maxとminの差が360ではありません",
					L"max - min != 360",
					L"Utility::ClampAngle(float min, float max, float angle);"
				);
			}

			if (angle > max) {
				angle -= max;
				angle += min;
			}
			else if (angle < min) {
				angle += max;
				angle -= min;
			}

			return angle;
		}

		bool WStrToBool(wstring str) {
			if (str == L"TRUE" || str == L"True" || str == L"true") {
				return true;
			}
			else if (str == L"FALSE" || str == L"False" || str == L"false") {
				return false;
			}
			else {
				throw BaseException(
					L"不正な文字列です。",
					L"str : " + str,
					L"Utility::WStrToBool(wstring str)"
				);
				return false;
			}
		}

		Col4 ConvertColorZeroToOne(const Col4& color) {
			return color / 255.0f;
		}

		Vec3 ChangeVectorLength(const Vec3& vector, float length) {
			auto vec = vector;
			auto normalizedVec = vec.normalize();
			return normalizedVec * length;
		}

		Vec3 ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position) {
			auto camera = view->GetTargetCamera();
			const auto& viewport = view->GetTargetViewport();
			const float& w = viewport.Width * 0.5f; //画面の幅の半分
			const float& h = viewport.Height * 0.5f; //画面の高さの半分

			const auto& mView = camera->GetViewMatrix(); //ビュー行列
			const auto& mProj = camera->GetProjMatrix(); //プロジェクション行列
			Mat4x4 mScreen( //スクリーン行列(BaseCross用)
				{ w, 0, 0, 0 },
				{ 0, h, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			);

			Mat4x4 mVPS = mView * mProj * mScreen;
			return Vec3(XMVector3TransformCoord(position, mVPS));
		}

		float GetTwoVectorAngle(const Vec3& vector1, const Vec3& vector2) {
			auto dot = vector1.dot(vector2);
			return XMConvertToDegrees(acosf(dot));
		}

		Vec3 ClampVector3(const Vec3& value, const Vec3& _min, const Vec3& _max) {
			auto x = Clamp(value.x, _min.x, _max.x);
			auto y = Clamp(value.y, _min.y, _max.y);
			auto z = Clamp(value.z, _min.z, _max.z);
			return Vec3(x, y, z);
		}

		float Remap(float val, float inMin, float inMax, float outMin, float outMax) {
			return Clamp(outMin + (val - inMin) * (outMax - outMin) / (inMax - inMin), outMin, outMax);
		}

		Vec3 QuaternionToEulerAngle(const Quat& quat) {
			float x = quat.x;
			float y = quat.y;
			float z = quat.z;
			float w = quat.w;

			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;

			float xy = x * y;
			float xz = x * z;
			float yz = y * z;
			float wx = w * x;
			float wy = w * y;
			float wz = w * z;

			// 1 - 2y^2 - 2z^2
			float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);

			// 2xy + 2wz
			float m01 = (2.0f * xy) + (2.0f * wz);

			// 2xy - 2wz
			float m10 = (2.0f * xy) - (2.0f * wz);

			// 1 - 2x^2 - 2z^2
			float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);

			// 2xz + 2wy
			float m20 = (2.0f * xz) + (2.0f * wy);

			// 2yz+2wx
			float m21 = (2.0f * yz) - (2.0f * wx);

			// 1 - 2x^2 - 2y^2
			float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

			float tx, ty, tz;

			if (m21 == 1.0f) {
				tx = XM_PI / 2.0f;
				ty = 0;
				tz = atan2(m10, m00);
			}
			else if (m21 == -1.0f) {
				tx = -XM_PI / 2.0f;
				ty = 0;
				tz = atan2(m10, m00);
			}
			else {
				tx = asin(-m21);
				ty = atan2(m20, m22);
				tz = atan2(m01, m11);
			}
			return Vec3(tx, ty, tz);
		}

		Vec3 ParentRelativePositionToWorldPosition(const shared_ptr<Transform>& transPtr, const Vec3& relativePosition) {
			// 親のワールド行列を取得
			auto matParent = transPtr->GetWorldMatrix();
			// 行列のスケーリングを1.0に
			matParent.scaleIdentity();

			// 親からの相対位置を行列化
			Mat4x4 mat;
			mat.affineTransformation(
				Vec3(1.0),
				Vec3(0.0),
				Vec3(0.0),
				relativePosition
			);

			// 作成した行列に親の行列を掛ける
			mat *= matParent;

			return mat.transInMatrix();
		}
	}
}