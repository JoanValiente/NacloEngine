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
	void SetRotation(float3 rotation);
	void SetSize(float3 size);
	void SetQuaternion(Quat quaternion);
	void SetPivot(float2 pivot);
	void SetMinAnchor(float2 minAnchor);
	void SetMaxAnchor(float2 maxAnchor);
	void SetWidth(float width);
	void SetHeight(float height);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:

	float3 position = float3::zero;
	float3 rotation = float3::zero;
	Quat quaternion = Quat::identity;
	float3 size = { 1,1,1 };

	float default_width = 0;
	float default_height = 0;

	float width = 0;
	float height = 0;

	float2 pivot = float2::zero;

	float2 minAnchor = float2::zero;
	float2 maxAnchor = float2::zero;

	float4x4 localmatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif
