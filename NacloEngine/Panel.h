#ifndef __PANEL_H__
#define __PANEL_H__

#include "Globals.h"
#include <string>

class Panel
{
public:
	Panel(const char* name);
	virtual ~Panel();

	void SwitchActive();
	bool IsActive() const;

	virtual void Draw() = 0;

public:
	bool active = true;
	int width, height, posx, posy;

protected:
	std::string name;
};

#endif // __PANEL_H__
