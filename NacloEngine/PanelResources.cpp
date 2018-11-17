#include "PanelResources.h"
#include "ModuleResources.h"
#include "Application.h"
#include "ModuleFileSystem.h"

PanelResources::PanelResources() : Panel("Resources")
{
	active = true;
}

PanelResources::~PanelResources()
{	
}

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active);

	if (ImGui::IsItemHoveredRect() && ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("File Manager");
		if (ImGui::BeginPopup("File Manager"))
		{
			if (ImGui::BeginMenu("Files"))
			{
				LOG("Create new folder");
			}
			ImGui::EndPopup();
		}
	}

	App->fs->GetFiles("Assets");

	DrawResourceType(RESOURCE_MATERIAL);
	DrawResourceType(RESOURCE_MESH);

	ImGui::End();
}

UID PanelResources::DrawResourceType(Resource_Type type)
{
	UID selected = 0;
	std::vector<const Resource*> resources;

	const char* titles[] = { "Materials", "Meshes", "Others" };

	if (ImGui::TreeNodeEx(titles[type], 0))
	{
		App->resources->CollectResourcesByType(resources, type);
		for (vector<const Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		{
			const Resource* info = (*it);
			if (ImGui::TreeNodeEx(info->GetExportedFile(), ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked())
					selected = info->GetUID();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("UID: %llu", info->GetUID());
					ImGui::Text("Source: %s", info->GetFile());
					ImGui::Text("References: %u", info->CountReferences());
					ImGui::EndTooltip();
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	return selected;
}

