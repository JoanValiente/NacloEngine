#ifndef __PANELRANDOMNUMBERGENERATOR_H__
#define __PANELRANDOMNUMBERGENERATOR_H__

#include "Panel.h"
#include "Time.h"
#include "PCG/pcg_variants.h"
#include "PCG/entropy.h"

class PanelRandomNumberGenerator : public Panel
{
public:
	PanelRandomNumberGenerator();
	virtual ~PanelRandomNumberGenerator();

	void Draw();

private:
	pcg32_random_t rng;
};

#endif

