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

// C++ System headers
#include <iostream>

// Alpao SDK Header: All types and class are in the ACS namespace
#include "asdkDM.h"

using namespace std;
using namespace acs;

// Example using C++ API
int dmExample()
{
    String serialName;

    // Get serialName number
    cout << "Please enter the S/N within the following format: BXXYYY (see DM backside)" << endl;
    cin >> serialName;
    cin.ignore(10, '\n');

    // Load configuration file
    DM dm( serialName.c_str() );
    cout << "Loaded configuration file: " << dm.GetString("CfgPath") << endl;

    // Get the number of actuators
    UInt nbAct = (UInt) dm.Get( "NbOfActuator" );

    // Check errors
    if ( !dm.Check() )
    {
        return -1;
    }
    
    cout << "Number of actuators: " << nbAct << endl;

    // Initialize data
    Scalar *data = new Scalar[nbAct];
    for ( UInt i = 0 ; i < nbAct ; i++ )
    {
        data[i] = 0;
    }
    
    cout << "Send data on mirror (data LED should blink): " << endl;
    // Send value to the DM
    for ( UInt act = 0; act < nbAct && dm.Check(); act++ )
    {
        cout << ".";

        data[ act ] = 0.12;
        dm.Send( data );
        Sleep( 200 );
        data[ act ] = 0;
    }
    cout << "Done." << endl;
    
    // Release memory
    delete [] data;

    return 0;
}

// Main program
int main( int argc, char ** argv )
{
    int ret = dmExample();
    
    // Print last errors if any
    while ( !DM::Check() ) DM::PrintLastError();

    Pause();
    
    return ret;
}