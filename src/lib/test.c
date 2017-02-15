#include "inc/const.h"

PUBLIC	void	disp_str(char * pszInfo);

int choose(int a, int b)
{
	if(a >= b){
		disp_str("the 1st one\n");
	}
	else{
		disp_str("the 2nd one\n");
	}

	return 0;
}
