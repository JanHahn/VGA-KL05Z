#ifndef KLAW_H
#define KLAW_H

#include "MKL05Z4.h"
#define S1	7		//PTB 7
#define S2	6		//PTB 6
#define S1_MASK	(1<<7)		
#define S2_MASK	(1<<6)


void Klaw_Init(void);
void Klaw_IQR_Int(void);

#endif  /* KLAW_H */
