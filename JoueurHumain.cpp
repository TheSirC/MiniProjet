#include "../Headers/JoueurHumain.h"

void JoueurHumain::ChoixCible(int CibleX, int CibleY)
{
	if (CibleX <= 0)
		CibleX_ = 1;
	else
		CibleX_ = CibleX;
	if (CibleY <= 0)
		CibleY_ = 1;
	else
		CibleY_ = CibleY;
}