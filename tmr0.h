/* 
 * File:   tmr0.h
 * Author: saras
 *
 * Created on July 26, 2022, 5:12 PM
 */

#ifndef TMR0_H
#define	TMR0_H

#define _tmr0_value 158 

void tmr0_init (uint8_t prescaler);
void tmr0_reload(void);

#endif	/* TMR0_H */

