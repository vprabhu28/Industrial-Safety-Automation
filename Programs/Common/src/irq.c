
#include "LPC214x.h"		
#include "type.h"
#include "irq.h"


void DefaultVICHandler (void) __irq
{
    
    VICVectAddr = 0;		
    while ( 1 );
}


void init_VIC(void) 
{
    DWORD i = 0;
    DWORD *vect_addr, *vect_cntl;
   	
    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
	vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
	vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
	*vect_addr = 0;	
	*vect_cntl = 0;
    }

    VICDefVectAddr = (DWORD)DefaultVICHandler;   
    return;
}


DWORD install_irq( DWORD IntNumber, void *HandlerAddr )
{
    DWORD i;
    DWORD *vect_addr;
    DWORD *vect_cntl;
      
    VICIntEnClr = 1 << IntNumber;	/* Disable Interrupt */
    
    for ( i = 0; i < VIC_SIZE; i++ )
    {
	

	vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
	vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
	if ( *vect_addr == (DWORD)NULL )
	{
	    *vect_addr = (DWORD)HandlerAddr;	/* set interrupt vector */
	    *vect_cntl = (DWORD)(IRQ_SLOT_EN | IntNumber);
	    break;
	}
    }
    if ( i == VIC_SIZE )
    {
	return( FALSE );		/* fatal error, can't find empty vector slot */
    }
    VICIntEnable = 1 << IntNumber;	/* Enable Interrupt */
    return( TRUE );
}


DWORD uninstall_irq( DWORD IntNumber )
{
    DWORD i;
    DWORD *vect_addr;
    DWORD *vect_cntl;
      
    VICIntEnClr = 1 << IntNumber;	/* Disable Interrupt */
    
    for ( i = 0; i < VIC_SIZE; i++ )
    {
	/* find first un-assigned VIC address for the handler */
	vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
	vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
	if ( (*vect_cntl & ~IRQ_SLOT_EN ) == IntNumber )
	{
	    *vect_addr = (DWORD)NULL;	/* clear the VIC entry in the VIC table */
	    *vect_cntl &= ~IRQ_SLOT_EN;	/* disable SLOT_EN bit */	
	    break;
	}
    }
    if ( i == VIC_SIZE )
    {
	return( FALSE );		/* fatal error, can't find interrupt number 
					in vector slot */
    }
    VICIntEnable = 1 << IntNumber;	/* Enable Interrupt */
    return( TRUE );
}


