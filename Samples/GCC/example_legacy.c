/* System Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sleep function is not defined in Linux, define it
// with available one usleep() from unistd.
#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#define Sleep(ms) usleep(1000*ms);
#define scanf_s(_Format, _X1, _X2) scanf(_Format, _X1);
#endif

/* Wait for human action */
void Pause() {
    printf( "Press ENTER to exit... \n" );
	  fflush(stdin); getchar();
}

/* Alpao SDK Legacy Header */
#include "acedev5.h"

#define N_DM 2

/* Example using legacy code and multiple mirrors */
int legacyExample()
{
    int nbAct = 0, act, idx;
    acecsCOMPL_STAT ret;
    
    const int nDm = N_DM;
    int       dmIds[N_DM];

    double * data;
    int    i_tmp[N_DM];

	  char   serialName[128] = "";
	
	  /* Get serial number */
    printf( "Please enter the S/N within the following format: BXXYYY (see DM backside)\n" );
    scanf_s( "%s", serialName, sizeof(serialName) );

    /* Load configuration file */
	  /* NOTE: For this example, we simulate the control of two mirrors using the same serial number,
	   you should use one serial number per mirror. To control a single mirror use the following parameters:
			   ret = acedev5Init( 1, dmIds, serialName ); */
    ret = acedev5Init( nDm, dmIds, serialName, serialName );

    if ( ret != acecsSUCCESS )
    {
        return -1;
    }
        
    /* Get the number of actuators */
    ret = acedev5GetNbActuator( nDm, dmIds, i_tmp );
	  for ( idx = 0; idx < N_DM; idx++ )
		  nbAct += i_tmp[ idx ];

    /* Check errors */
    if ( ret != acecsSUCCESS )
    {
        return -1;
    }
    
    printf( "Total number of actuators: %d\n", nbAct );

    /* Initialize data */
    data = (double*) calloc( nbAct, sizeof( double ) );
    for ( idx = 0 ; idx < nbAct ; idx++ )
    {
        data[idx] = 0;
    }

    /* Send value to the DM */
    printf( "Send data on mirrors (data LED should blink):\n" );
    for ( act = 0; act < nbAct && ret == acecsSUCCESS; act++ )
    {
		printf(".");

        data[ act ] = 0.10;
        ret = acedev5Send( nDm, dmIds, data );
        Sleep(200);
        data[ act ] = 0;
    }
	  printf( "Done.\n" );

    /* Release memory */
    free( data );

    /* Release */
    acedev5Release( nDm, dmIds );

    return 0;
}

/* Main program */
int main( int argc, char ** argv )
{
    int ret = legacyExample();
    
    /* Print last error if any */
    acecsErrDisplay();

    Pause();
    return ret;
}