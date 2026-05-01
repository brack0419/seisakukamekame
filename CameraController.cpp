#include <imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <algorithm>

CameraController::CameraController()
{
	// 起動時はカメラモード
	mouseCameraActive = true;

	// 最初からカーソル非表示
	ShowCursor(FALSE);

	// 初期角度
	angle = { 0.0f, 0.0f, 0.0f };

	// 注視点
	target = { 0.0f, 0.0f, 0.0f };

	// 距離
	range = 7.0f;
}
// 更新処理
void CameraController::Update(float elapsedTime)
{
	ImGuiIO& io = ImGui::GetIO();

	// ===============================
	// 右クリックでカメラ操作トグル
	// ===============================
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !io.WantCaptureMouse)
	{
		mouseCameraActive = !mouseCameraActive;

		if (mouseCameraActive)
		{
			ShowCursor(FALSE);

			// カーソルを画面中央へ移動
			RECT rect;
			GetClientRect(GetActiveWindow(), &rect);

			POINT center;
			center.x = (rect.right - rect.left) / 2;
			center.y = (rect.bottom - rect.top) / 2;

			ClientToScreen(GetActiveWindow(), &center);
			SetCursorPos(center.x, center.y);
		}
		else
		{
			ShowCursor(TRUE);
		}
	}

	// カメラ操作していないなら通常カーソル
	if (!mouseCameraActive)
	{
		return;
	}

	// ===============================
	// マウス位置取得（画面中央との差分）
	// ===============================
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);

	POINT center;
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	POINT mousePos;
	GetCursorPos(&mousePos);

	POINT screenCenter = center;
	ClientToScreen(GetActiveWindow(), &screenCenter);

	float deltaX = float(mousePos.x - screenCenter.x);
	float deltaY = float(mousePos.y - screenCenter.y);

	// ===============================
	// 回転
	// ===============================
	angle.y += deltaX * mouseSensitivity;
	angle.x += deltaY * mouseSensitivity;

	// 上下制限
	angle.x = std::clamp(angle.x, minAngleX, maxAngleX);

	// 左右360度回転
	if (angle.y < -DirectX::XM_PI)
		angle.y += DirectX::XM_2PI;
	if (angle.y > DirectX::XM_PI)
		angle.y -= DirectX::XM_2PI;

	// ===============================
	// 毎フレーム中央へ戻す
	// ===============================
	SetCursorPos(screenCenter.x, screenCenter.y);

	// ===============================
	// カメラ位置計算
	// ===============================
	DirectX::XMMATRIX transform =
		DirectX::XMMatrixRotationRollPitchYaw(
			angle.x,
			angle.y,
			angle.z
		);

	DirectX::XMVECTOR frontVec = transform.r[2];

	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, frontVec);

	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	Camera::Instance().SetLookAt(
		eye,
		{ target.x, target.y + 2, target.z },
		DirectX::XMFLOAT3(0, 1, 0)
	);

	// ===============================
	// ホイールズーム
	// ===============================
	if (io.MouseWheel != 0.0f)
	{
		const float zoomSpeed = 1.0f;
		range -= io.MouseWheel * zoomSpeed;
		range = std::clamp(range, 0.5f, 50.0f);
	}
}

// デバッグ用GUI描画
void CameraController::DrawDebugGUI()
{
	if (!ImGui::Begin("Camera"))
	{
		ImGui::End();
		return;
	}

	// =========================
	// カメラ操作 ON / OFF
	// =========================
	ImGui::Checkbox("Mouse Camera Active", &mouseCameraActive);

	ImGui::Separator();

	// =========================
	// マウス感度
	// =========================
	ImGui::SliderFloat(
		"Mouse Sensitivity",
		&mouseSensitivity,
		0.0f,
		0.02f,
		"%.3f"
	);

	ImGui::Separator();

	// =========================
	// 詳細設定
	// =========================
	if (ImGui::CollapsingHeader("CameraController", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// -------- 距離 --------
		ImGui::SliderFloat(
			"Range",
			&range,
			0.5f,
			50.0f
		);

		// -------- 上制限 --------
		float maxAngleDeg = DirectX::XMConvertToDegrees(maxAngleX);

		ImGui::SliderFloat(
			"Max Angle X (deg)",
			&maxAngleDeg,
			0.0f,
			89.0f
		);

		maxAngleX = DirectX::XMConvertToRadians(maxAngleDeg);
	}

	ImGui::End();
}