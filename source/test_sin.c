/***************************************************************************
 * Getting In Tune
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: Assignment PDF, lecture slides, Dean & White Textbook
 ***************************************************************************/

#include "test_sin.h"
#include <stdint.h>
#include "sin.h"
#include "fsl_debug_console.h"
#include <math.h>


// ---------------------------------------test_sin--------------------------------------------------
/***********************************************************************************
 * function : function to test the get sine val function
 * parameters : none
 * return : none
***********************************************************************************/
void test_sin()
{
	double act_sin;
	double exp_sin;
	double err;
	double sum_sq = 0;
	double max_err = 0;
	int i= 0;
	for (i=-TWO_PI; i <= TWO_PI; i++)
	{
		exp_sin = sin( (double)i / SCALE_FACTOR) * SCALE_FACTOR;
		act_sin = get_sin_val(i);
		err = act_sin - exp_sin;
		if (err < 0)
		{
			err = -err;
		}
		if (err > max_err)
		{
			max_err = err;
		}
		sum_sq += err*err;
	}
	PRINTF("Maximum Error=%d Sum of Square=%d\r\n\n",(int)max_err,(int)sum_sq);
}
// ---------------------------------------End--------------------------------------------------

