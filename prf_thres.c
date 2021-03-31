#include "prf_thres.h"
#include "ostu.h"

void prf_threshold()
{
	freopen("output_threshold.txt","a",stdout);
	int i=get_ostu_thres();
	if(i>=100)
	{
		printf("%c",'0'+i/100);
	}
	if(i>=10)
	{
		printf("%c",'0'+(i%100)/10);
	}
	printf("%c",'0'+i%10);
	printf(",");
	
	fclose(stdout);
}
