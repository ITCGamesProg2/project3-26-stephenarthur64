#include "Exit.h"

static bool close = false;

void Exit::closeGame()
{
	close = true;
}

bool Exit::isGameClosed()
{
	return close;
}
