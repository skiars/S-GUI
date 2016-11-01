#include "GUI_Math.h"
#include "GUI.h"

/*********************************************************************
*
*       sin / cos (internal)
*
*  Angle : 90/1024?
*  Data  : 1/1024
*/
static const u_16 aSin[] = {
	0,       /*  1/16 *90?*/
	100,     /*  1/16 *90?*/
	200,     /*  2/16 *90?*/
	297,     /*  3/16 *90?*/
	392,     /*  4/16 *90?*/
	483,     /*  5/16 *90?*/
	569,     /*  6/16 *90?*/
	650,     /*  7/16 *90?*/
	724,     /*  8/16 *90?*/
	792,     /*  9/16 *90?*/
	851,     /* 10/16 *90?*/
	903,     /* 11/16 *90?*/
	946,     /* 12/16 *90?*/
	980,     /* 13/16 *90?*/
	1004,    /* 14/16 *90?*/
	1019,    /* 15/16 *90?*/
	1024     /* 16/16 *90?*/
};

int GUI_sin(int angle)
{
	char IsNeg = 0;
	int i;
	u_16 Faktor;
	u_32 t;

	angle &= ((1 << 12) - 1);  /* reduce to 0-360 degrees */
	if (angle> 2 * GUI_90DEG) {
		angle -= 2 * GUI_90DEG;
		IsNeg = 1;
	}
	if (angle> GUI_90DEG) {  /* between 90-180 */
		angle = 2 * GUI_90DEG - angle;   /* use sine symetry */
	}
	/* Now angle is reduced to 0?<= <= 90?*/
	i = angle >> 6;
	{
		Faktor = (u_16)((1 << 6) - (angle&((1 << 6) - 1)));
		t = aSin[i] * (u_32)Faktor;
		if (Faktor != (1 << 6)) {
			t += aSin[i + 1] * ((1 << 6) - Faktor);
		}
	}
	t = (t + (1 << 5)) >> 6;   /* divide,incl. rounding */
	return (IsNeg) ? 0 - t : t;
}

int GUI_cos(int angle)
{
	return GUI_sin(angle + GUI_90DEG);
}
