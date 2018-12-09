#ifndef _ComponentRectTransform_H_
#define _ComponentRectTransform_H_

#include "MathGeoLib/MathGeoLib.h"

class ComponentRectTransform : public Component
{
public:
	ComponentRectTransform(GameObject* container);

	void ShowInspector();
	void DrawGuizmos();

	void UpdateMatrix();

	void SetPosition(float3 position);
	void SetPivot(float2 pivot);
	void SetMinAnchor(float2 minAnchor);
	void SetMaxAnchor(float2 maxAnchor);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:

	float3 position = float3::zero;
	float3 rotation = float3::zero;
	Quat quaternion = Quat::identity;
	float3 size = float3::zero;

	float width = 0;
	float height = 0;

	float2 pivot = float2::zero;

	float2 minAnchor = float2::zero;
	float2 maxAnchor = float2::zero;

	float4x4 localmatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif
