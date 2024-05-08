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

Scalar* readCsv(const char* csv_file, int max_col, int max_line) {
	Scalar* out = (Scalar*) malloc(sizeof(Scalar) * max_col * max_line);
	FILE* fid = fopen(csv_file, "r");

	const int line_max_length = max_col * 20;
	char* line = (char*) malloc(line_max_length);

	int lineId, colId;
	char* token;

	if (fid == NULL) {
		printf("Cannot open %s.\n", csv_file);
		free(line);
		return out;
	}

	lineId = 0;
	while (fgets(line, line_max_length, fid) != NULL && lineId < max_line) {

		colId = 0;

		token = strtok(line, ";");
		while (token != NULL && colId < max_col) {

			out[lineId * max_col + colId] = strtod(token, NULL);

			token = strtok(NULL, ";\n");

			colId++;
		}

		lineId++;
	}

	free(line);

	fclose(fid);

	return out;
}

/* Example using C API */
int wrapperExample()
{
	UInt nbAct, actId, idx;
	COMPL_STAT ret;
	Scalar *   data;
	Scalar     tmp;

	asdkDM * dm = NULL;

	char   serialName[128] = "";
	char* ace_path = NULL;
	char   fileName[128] = "";

	int modeId;

	/* Get serial number */
	printf("Please enter the S/N within the following format: BXXYYY (see DM backside)\n");
	scanf_s("%s", serialName, sizeof(serialName));

	/* Load configuration file */
	dm = asdkInit(serialName);
	if (dm == NULL)
	{
		return -1;
	}

	/* Get the number of actuators */
	ret = asdkGet(dm, "NbOfActuator", &tmp);
	nbAct = (UInt)tmp;

	/* Check errors */
	if (ret != SUCCESS)
	{
		return -1;
	}

	printf("Number of actuators: %d\n", nbAct);

	/* Initialize data */
	data = (Scalar*)calloc(nbAct, sizeof(Scalar));
	for (idx = 0; idx < nbAct; idx++)
	{
		data[idx] = 0;
	}


	/* Read the Z2C file (each line is a mode) */
#define NB_MODES 100 // Change this to the number of lines in the Z2C.csv file.

	ret = asdkGetString(dm, "CfgPath", &ace_path);

	//char* fileName = malloc(strlen(ace_path) + strlen(serialName) + 10);
	//sprintf(fileName, "%s\\%s-Z2C.csv", ace_path, serialName);
	sprintf(fileName, "%s-Z2C.csv", ace_path);
	Scalar* z2c = readCsv(fileName, nbAct, NB_MODES);

	/* Create the modeVector */
	Scalar modeVector[NB_MODES] = { 0 };
	modeVector[3] = 5;
	modeVector[4] = 5;

	/* Matrix product */
	for (modeId = 0; modeId < NB_MODES; modeId++) {
		for (actId = 0; actId < nbAct; actId++) {
			data[actId] += z2c[modeId * nbAct + actId] * modeVector[modeId];
		}
	}

	free(z2c);

	ret = asdkSend(dm, data);
	Sleep(500);

	printf("Done.\n");

	/* Release memory */
	free(data);

	/* Reset mirror values */
	asdkReset(dm);

	/* Release */
	asdkRelease(dm);
	dm = NULL;

	return 0;
}

/* Main program */
int main(int argc, char ** argv)
{
	int ret = wrapperExample();

	/* Print last error if any */
	asdkPrintLastError();

	Pause();
	
	return ret;
}