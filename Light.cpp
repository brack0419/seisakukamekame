#include	"Graphics/Graphics.h"
#include	"Light.h"

Light::Light(LightType lightType)
	: lightType(lightType)
{
}

void Light::PushRenderContext(RenderContext& rc) const
{
	// ìoò^Ç≥ÇÍÇƒÇ¢ÇÈåıåπÇÃèÓïÒÇê›íË
	switch (lightType)
	{
	case	LightType::Directional:
	{
		rc.directionalLightData.direction.x = direction.x;
		rc.directionalLightData.direction.y = direction.y;
		rc.directionalLightData.direction.z = direction.z;
		rc.directionalLightData.direction.w = 0.0f;
		rc.directionalLightData.color = color;
		break;
	}
	case	LightType::Point:
	{
		if (rc.pointLightCount >= PointLightMax)
			break;
		rc.pointLightData[rc.pointLightCount].position.x = position.x;
		rc.pointLightData[rc.pointLightCount].position.y = position.y;
		rc.pointLightData[rc.pointLightCount].position.z = position.z;
		rc.pointLightData[rc.pointLightCount].position.w = 1.0f;
		rc.pointLightData[rc.pointLightCount].color = color;
		rc.pointLightData[rc.pointLightCount].range = range;
		rc.pointLightData[rc.pointLightCount].influence = influence;
		++rc.pointLightCount;
		break;
	}
	case	LightType::Spot:
	{
		if (rc.spotLightCount >= SpotLightMax)
			break;

		rc.spotLightData[rc.spotLightCount].position.x = position.x;
		rc.spotLightData[rc.spotLightCount].position.y = position.y;
		rc.spotLightData[rc.spotLightCount].position.z = position.z;
		rc.spotLightData[rc.spotLightCount].position.w = 1.0f;
		rc.spotLightData[rc.spotLightCount].direction.x = direction.x;
		rc.spotLightData[rc.spotLightCount].direction.y = direction.y;
		rc.spotLightData[rc.spotLightCount].direction.z = direction.z;
		rc.spotLightData[rc.spotLightCount].direction.w = 0.0f;
		rc.spotLightData[rc.spotLightCount].color = color;
		rc.spotLightData[rc.spotLightCount].range = range;
		rc.spotLightData[rc.spotLightCount].innerCorn = innerCorn;
		rc.spotLightData[rc.spotLightCount].outerCorn = outerCorn;
		rc.spotLightData[rc.spotLightCount].influence = influence;
		++rc.spotLightCount;
		break;
	}
	}
}

void Light::DrawDebugGUI()
{
	static constexpr const char* lightTypeName[] =
	{
		"Directional",
		"Point",
		"Spot",
	};

	if (ImGui::TreeNode(lightTypeName[static_cast<int>(lightType)]))
	{
		switch (lightType)
		{
		case LightType::Directional:
		{
			if (ImGui::SliderFloat3("Direction", &direction.x, -1.0f, 1.0f))
			{
				float len = sqrtf(
					direction.x * direction.x +
					direction.y * direction.y +
					direction.z * direction.z
				);

				if (len > 0.0f)
				{
					direction.x /= len;
					direction.y /= len;
					direction.z /= len;
				}
			}

			ImGui::ColorEdit3("Color", &color.x);
			break;
		}

		case LightType::Point:
		{
			ImGui::SliderFloat3("Position", &position.x, -20.0f, 20.0f);
			ImGui::ColorEdit3("Color", &color.x);
			ImGui::SliderFloat("Range", &range, 0.0f, 100.0f);
			ImGui::SliderFloat("Influence", &influence, 1.0f, 10.0f);
			break;
		}

		case LightType::Spot:
		{
			ImGui::SliderFloat3("Position", &position.x, -20.0f, 20.0f);

			if (ImGui::SliderFloat3("Direction", &direction.x, -1.0f, 1.0f))
			{
				DirectX::XMStoreFloat3(
					&direction,
					DirectX::XMVector3Normalize(
						DirectX::XMLoadFloat3(&direction)
					)
				);
			}

			ImGui::ColorEdit3("Color", &color.x);
			ImGui::SliderFloat("Range", &range, 0.0f, 100.0f);
			ImGui::SliderFloat("Inner Cone", &innerCorn, 0.0f, 1.0f);
			ImGui::SliderFloat("Outer Cone", &outerCorn, 0.0f, 1.0f);
			ImGui::SliderFloat("Influence", &influence, 1.0f, 10.0f);
			break;
		}
		}

		ImGui::TreePop();
	}
}

void Light::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();
	switch (lightType)
	{
	case LightType::Directional:
	{
		//	ïΩçsåıåπÇÕï\é¶ÇµÇ»Ç¢ÅB
		break;
	}
	case LightType::Point:
	{
		//	ì_åıåπÇÕëSï˚à Ç…åıÇï˙éÀÇ∑ÇÈåıåπÇ»ÇÃÇ≈ãÖëÃÇï`âÊÇ∑ÇÈÅB
		debugRenderer->DrawSphere(position, range, color);
		break;
	}
	case LightType::Spot:
	{
		DirectX::XMVECTOR	Direction = DirectX::XMLoadFloat3(&direction);
		float len;
		DirectX::XMStoreFloat(&len, DirectX::XMVector3Length(Direction));
		if (len <= 0.00001f)
			break;
		Direction = DirectX::XMVector3Normalize(Direction);

		//	é≤éZèo
		DirectX::XMFLOAT3 dir;
		DirectX::XMStoreFloat3(&dir, Direction);
		DirectX::XMVECTOR Work = fabs(dir.y) == 1 ? DirectX::XMVectorSet(1, 0, 0, 0)
			: DirectX::XMVectorSet(0, 1, 0, 0);
		DirectX::XMVECTOR	XAxis = DirectX::XMVector3Cross(Direction, Work);
		DirectX::XMVECTOR	YAxis = DirectX::XMVector3Cross(XAxis, Direction);
		XAxis = DirectX::XMVector3Cross(Direction, YAxis);

		static constexpr int SplitCount = 16;
		for (int u = 0; u < SplitCount; u++)
		{
			float s = static_cast<float>(u) / static_cast<float>(SplitCount);
			float r = -DirectX::XM_PI + DirectX::XM_2PI * s;
			// âÒì]çsóÒéZèo
			DirectX::XMMATRIX	RotationZ = DirectX::XMMatrixRotationAxis(Direction, r);
			// ê¸ÇéZèo
			DirectX::XMFLOAT3	OldPoint;
			{
				DirectX::XMVECTOR	Point = Direction;
				DirectX::XMMATRIX	Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(outerCorn))
					* RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3	pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(position, color);
				lineRenderer->AddVertex(pos, color);
				OldPoint = pos;
			}
			// ãÖñ ÇéZèo
			for (int v = 0; v <= SplitCount; ++v)
			{
				float s = static_cast<float>(v) / static_cast<float>(SplitCount);
				float a = outerCorn + (1.0f - outerCorn) * s;
				DirectX::XMVECTOR	Point = Direction;
				DirectX::XMMATRIX	Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(a))
					* RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3	pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(OldPoint, color);
				lineRenderer->AddVertex(pos, color);
				OldPoint = pos;
			}
		}
		break;
	}
	}
}