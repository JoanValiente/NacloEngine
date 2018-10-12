#include "Imgui/imgui.h"
#include "PanelRandomNumberGenerator.h"

PanelRandomNumberGenerator::PanelRandomNumberGenerator(): Panel("Random Number Generator")
{
	srand(time(NULL));
	uint64_t seeds[2];
	seeds[0] = rand();
	seeds[1] = rand();
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);
	active = false;
}

PanelRandomNumberGenerator::~PanelRandomNumberGenerator()
{
}

void PanelRandomNumberGenerator::Draw()
{
	static double random1 = 0.0f;
	static int random2 = 0;

	if (!ImGui::Begin(("Random Number Generator"), &active))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Generate a random number between 0 - 1");
		if (ImGui::Button("Generate!"))
		{
			random1 = ldexp(pcg32_random_r(&rng), -32);
		}
		ImGui::SameLine();
		ImGui::Text("number = %f", random1);

		ImGui::Text("Generate a random number between 0 - 100");
		if (ImGui::Button("Generate"))
		{
			random2 = (int)pcg32_boundedrand_r(&rng, 101);
		}
		ImGui::SameLine();
		ImGui::Text("number = %i", random2);

		ImGui::End();
	}
}
