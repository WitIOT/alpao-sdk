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
  printf("Press ENTER to exit... \n");
  fflush(stdin); getchar();
}

/* Alpao SDK C Header */
#include "asdkWrapper.h"

/* Example using C API */
int wrapperExample()
{
    UInt nbAct, act, idx;
    COMPL_STAT ret;
    Scalar *   data;
    Scalar     tmp;

    asdkDM * dm = NULL;
    
	char   serialName[128] = "";

  /* Get serial number */
    printf("Please enter the S/N within the following format: BXXYYY (see DM backside)\n");
    scanf_s("%s", serialName, sizeof(serialName));

    /* Load configuration file */
    dm = asdkInit(serialName);
    if (dm == NULL) {
      return -1;
    }
	
    char* cfgPath;
    asdkGetString(dm, "CfgPath", &cfgPath);
    printf("Loaded configuration file: %s\n", cfgPath);
    free(cfgPath);

    /* Get the number of actuators */
    ret = asdkGet( dm, "NbOfActuator", &tmp );
    nbAct = (UInt) tmp;

    /* Check errors */
    if ( ret != SUCCESS )
    {
        return -1;
    }
    
    printf( "Number of actuators: %d\n", nbAct );

    /* Initialize data */
    data = (Scalar*) calloc( nbAct, sizeof( Scalar ) );
    for ( idx = 0 ; idx < nbAct ; idx++ )
    {
        data[idx] = 0;
    }

    /* Send value to the DM */
    printf( "Send data on mirror (data LED should blink):\n" );
    for ( act = 0; act < nbAct && ret == SUCCESS; act++ )
    {
		printf( "." );

        data[ act ] = 0.10;
        ret = asdkSend( dm, data );
        Sleep(200);
        data[ act ] = 0;
    }
	printf( "Done.\n" );

    /* Release memory */
    free( data );

    /* Reset mirror values */
    asdkReset( dm );

    /* Release */
    asdkRelease( dm );
    dm = NULL;

    return 0;
}

/* Main program */
int main( int argc, char ** argv )
{
    int ret = wrapperExample();
    
    /* Print last error if any */
    asdkPrintLastError();

    Pause();
    
    return ret;
}