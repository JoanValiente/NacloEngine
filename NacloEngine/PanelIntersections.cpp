#include "PanelIntersections.h"
#include "Imgui/imgui.h"

// ---------------------------------------------------------
PanelIntersections::PanelIntersections() : Panel("Intersections")
{
	//--------------------------------------------------------------------------------------------------------
	mPlane = new Plane(float3(0.0f, 1.0f, 0.0f), 1.0f);
	mSphere = new Sphere(float3(0.0f, 1.0f, 0.0f), 1.0f);
	mCapsule = new Capsule(LineSegment(float3((0.0f, 1.0f, 0.0f)), float3(1.0f, 2.0f, 1.0f)), 3.0f);
	mAabb = new AABB(float3(0.0f, 1.0f, 0.0f), float3(3.0f, 4.0f, 3.0f));
	mFrustum = new Frustum();
	mRay = new Ray(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f).Normalized());
	mTriangle = new Triangle(float3(0.0f, 0.0f, 0.0f), float3(3.0f, 3.0f, 3.0f), float3(5.0f, 5.0f, 5.0f));
	//---------------------------------------------------------------------------------------------------------
	active = false;
}

// ---------------------------------------------------------
PanelIntersections::~PanelIntersections()
{}

// ---------------------------------------------------------
void PanelIntersections::Draw()
{
	static bool intersect_test = false;
	static char* intersection = "No intersection";
	static char* intersection_result = "NO";


	if (!ImGui::Begin("Intersection Test", &active))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::Button("Plane - Sphere"))
		{
			intersection = "Plane - Sphere";
			intersect_test = mPlane->Intersects(*mSphere);
			LOG("Plane - Sphere Button pressed");
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane - AABB"))
		{
			intersection = "Plane - AABB";
			intersect_test = mPlane->Intersects(*mAabb);
		}
		if (ImGui::Button("Plane - Triangle"))
		{
			intersection = "Plane - Triangle";
			intersect_test = mPlane->Intersects(*mTriangle);
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane - Ray"))
		{
			intersection = "Plane - Ray";
			intersect_test = mPlane->Intersects(*mRay);
		}
		if (ImGui::Button("AABB - Sphere"))
		{
			intersection = "AABB - Sphere";
			intersect_test = mAabb->Intersects(*mSphere);
		}
		ImGui::SameLine();
		if (ImGui::Button("AABB - Ray"))
		{
			intersection = "AABB - Ray";
			intersect_test = mAabb->Intersects(*mRay);
		}
		if (ImGui::Button("Capsule - Sphere"))
		{
			intersection = "Capsule - Sphere";
			intersect_test = mCapsule->Intersects(*mSphere);
		}
		ImGui::SameLine();
		if (ImGui::Button("Capsule - Plane"))
		{
			intersection = "Capsule - Plane";
			intersect_test = mCapsule->Intersects(*mPlane);
		}
		if (ImGui::Button("Capsule - Triangle"))
		{
			intersection = "Capsule - Triangle";
			intersect_test = mCapsule->Intersects(*mTriangle);
		}
		ImGui::SameLine();
		if (ImGui::Button("Capsule - AABB"))
		{
			intersection = "Capsule - AABB";
			intersect_test = mCapsule->Intersects(*mAabb);
		}
		if (ImGui::Button("AABB - Triengle"))
		{
			intersection = "AABB - Triengle";
			intersect_test = mAabb->Intersects(*mTriangle);
		}

		ImGui::NewLine();
		ImGui::Text(intersection);

		if (intersect_test == 0)
		{
			intersection_result = "No";
		}
		else
		{
			intersection_result = "Yes";
		}

		ImGui::Text(intersection_result);
		ImGui::End();
	}
}