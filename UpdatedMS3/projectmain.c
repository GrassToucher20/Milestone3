/******************************************************************************
 *
 * File: projectmain.c
 * Author: Nevo Aftabi
 * Student Number: 172865214
 *
 * Description: This is the main file of the project
 *
 *****************************************************************************/

#include "project.h"

int main(void) 
{
	// Each item in the array is a row in the excel sheet
	struct OlympicAthleteEventResult olympicAthleteEventResults[OLYMPIC_ATHLETE_EVENT_SIZE] = {{{ 0 }}};
	struct Country countries[COUNTRIES_SIZE] = {{{ 0 }}};

	// Variables to store input from the user
	char edition[EDITION_SIZE + 1] = { 0 };
	char season[SEASON_SIZE + 1] = { 0 };
	int year = 0;

	// Number of olympic athlete event records and if file opening was fine
	int olympicAthleteEventRecords = 0;
	int fileReadSuccess = 0;
	
	// Prompt the user for the year
	printf("Year of the olympics: ");
	scanf("%d", &year);
	clearBuffer();

	// Prompt the user for the season
	printf("Season (Summer or Winter): ");
	scanf("%6[^\n]", season);
	clearBuffer();

	// Assemble the edition string
	sprintf(edition, "%04d %s Olympics", year, season);

	// Read olympic athlete event results and read the countries
	olympicAthleteEventRecords = readOlympicAthleteEventResults(olympicAthleteEventResults, countries, edition, &fileReadSuccess);
	readOlympicsCountry(countries, &fileReadSuccess);

	// If the file didn't fail, display histogram, otherwise display error 
	if (fileReadSuccess != FILE_READ_FAIL) { 
		
		// Display the histogram
		printHistogram(countries, olympicAthleteEventResults, edition, olympicAthleteEventRecords);
	}
	else {
		printf("ERROR: The files %s and %s couldn't be opened or read correctly. Verify their location and integrity and try again.\n", 
			OLYMPIC_ATHLETE_EVENT_RESULTS_FILEPATH, OLYMPICS_COUNTRIES_FILEPATH);
	}

	return 0;
}

