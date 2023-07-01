#include "keycord.h"
#include "JDY-31.h"
void main()
{
	EA=1;
	UartInit();
	while(1)
	{
		JDY31_judgement();//À¶ÑÀ¿ØÖÆ
		//keyjudgement();//°´¼ü¿ØÖÆ
	}
}