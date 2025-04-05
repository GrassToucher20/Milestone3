/******************************************************************************
 *
 * File: project.c
 * Author: Nevo Aftabi
 * Student Number: 172865214
 *
 * Description: This file contains implementations of functions from project.h
 *
 *****************************************************************************/

#include "project.h"

int readOlympicAthleteEventResults(struct OlympicAthleteEventResult eventResults[], struct Country countries[], const char* edition, int* fileReadSuccess)
{
    // Open the file for reading
    FILE* fp = fopen(OLYMPIC_ATHLETE_EVENT_RESULTS_FILEPATH, "r");
    *fileReadSuccess = FILE_READ_SUCCESS;
    int totalRecords = 0;

    // If the file pointer is valid, begin reading, otherwise set flag to fail
    if (fp != NULL) {
        // Variables needed for input and iteration
        char line[LINE_SIZE + 1];
        int i = 0;
        int j = 0;
        int keepLooking = 1;
        int parsed;
        
        struct OlympicAthleteEventResult tmpResult = {{ 0 }};

        // Get rid of the first line 
        fgets(line, LINE_SIZE, fp);

        // Loop through the file and put data into the array
        while (i < OLYMPIC_ATHLETE_EVENT_SIZE && fgets(line, LINE_SIZE, fp) && *fileReadSuccess != FILE_READ_FAIL) {
            // Read data into the eventResults array
            parsed = sscanf(line, "%24[^,],%*d,%3[^,],%*[^\"]\"%*[^\"]\",%*d,%*[^,],%*d,%*[^,],%10[^,],%*[^,],%*d",
                tmpResult.edition,
                tmpResult.countryNoc,
                tmpResult.medal);

            // If parsing fails, set flag
            if (parsed != NUM_OLYMPIC_ATHLETE_EVENT_RESULT_FIELDS) {
                *fileReadSuccess = FILE_READ_FAIL;
            }
            else {
                if (strcmp(tmpResult.edition, edition) == 0) {
                    eventResults[totalRecords++] = tmpResult;

                    // Look for matching countries matching the code, add to their medals
                    for (j = 0; j < COUNTRIES_SIZE && keepLooking; j++) {
                        if (strcmp(countries[j].noc, eventResults[i].countryNoc) == 0) {
                            if (strcmp(eventResults[i].medal, "Gold") == 0) {
                                // Add to gold medals and total metals for that country
                                countries[j].goldMedals++;
                                countries[j].totalMedals++;
                            }
                            else if (strcmp(eventResults[i].medal, "Silver") == 0) {
                                // Add to silver medals and total metals for that country
                                countries[j].silverMedals++;
                                countries[j].totalMedals++;
                            }
                            else if (strcmp(eventResults[i].medal, "Bronze") == 0) {
                                // Add to bronze medals and total metals for that country
                                countries[j].bronzeMedals++;
                                countries[j].totalMedals++;
                            }

                            // Set a flag to make the loop stop
                            keepLooking = 0;
                        }
                    }
                }

                i++;
            }
        }

        // Clean up
        fclose(fp);
        fp = NULL;
    }
    else {
        *fileReadSuccess = FILE_READ_FAIL;
    }

    return totalRecords;
}

int readOlympicsCountry(struct Country olympicsCountries[], int* fileReadSuccess)
{
    // The number of records read from the file
    int totalRecords = 0;

    // If the first read failed, stop 
    if (*fileReadSuccess != FILE_READ_FAIL) {
        // Open the file for reading
        FILE* fp = fopen(OLYMPICS_COUNTRIES_FILEPATH, "r");
        int totalRecords = 0;

        // If the file is valid, start reading
        if (fp != NULL) {
            // Variables needed for input from the file and iteration
            char line[LINE_SIZE + 1];
            int i = 0;
            int parsed;

            // Get rid of the first line
            fgets(line, LINE_SIZE, fp);

            // Read in remaining lines of the file
            while (i < COUNTRIES_SIZE && fgets(line, LINE_SIZE, fp) && *fileReadSuccess != FILE_READ_FAIL) {
                // Scan the line for country noc and country name
                parsed = sscanf(line, "%3[^,],%37s", olympicsCountries[i].noc, olympicsCountries[i].country);

                // If it couldn't be parsed, set the flag to fail, otherwise increment i
                if (parsed != NUM_COUNTRY_FIELDS) {
                    *fileReadSuccess = FILE_READ_FAIL;
                }
                else {
                    // Add to the totalRecords and increment i
                    totalRecords++;
                    i++;
                }
            }

            // Close the file handle
            fclose(fp);
            fp = NULL;
        }
        else {
            *fileReadSuccess = FILE_READ_FAIL;
        }
    }

    return totalRecords; 
}

int compareCountries(struct Country* country1, struct Country* country2)
{
    int result;

    /*
    First comparison: Do they have the same number of total medals?
    If they don't, break ties with gold, silver, and bronze medal count 
    Returns the result 
    */

    if (country1->totalMedals != country2->totalMedals) {
        // Compare based on the total medals
        result = country2->totalMedals - country1->totalMedals;
    }
    else if (country1->goldMedals != country2->goldMedals) {
        // Compare based on the number of gold medals
        result = country2->goldMedals - country1->goldMedals;
    }
    else if (country1->silverMedals != country2->silverMedals) {
        // Compare based on the number of silver medals
        result = country2->silverMedals - country1->silverMedals;
    }
    else {
        // Compare based on the number of bronze medals
        result = country2->bronzeMedals - country1->bronzeMedals;
    }

    return result;
}

void printHistogram(struct Country countries[], struct OlympicAthleteEventResult eventResults[], const char* edition, int olympicAthleteEventResultsSize)
{
    // Countries which participated in the edition given
    struct Country filteredCountries[COUNTRIES_SIZE] = {{{0}}};
    int filteredCount = 0;
    int i, j;

    // Count medals only for the specified editon that has a medal
    for (i = 0; i < olympicAthleteEventResultsSize; i++) {
        // If the edition matches and there is a medal
        if (strcmp(eventResults[i].edition, edition) == 0 && strcmp(eventResults[i].medal, "-") != 0) {
            // Find country in filtered list
            int found = 0;
            for (j = 0; j < filteredCount && !found; j++) {
                // If country noc matches, add to the medals of the filtered country
                if (strcmp(filteredCountries[j].noc, eventResults[i].countryNoc) == 0) {
                    // Set that the country has been found
                    found = 1;

                    // Add to the number of medals 
                    filteredCountries[j].totalMedals++;

                    // Add to the type of medal 
                    if (strcmp(eventResults[i].medal, "Gold") == 0) {
                        filteredCountries[j].goldMedals++;
                    }
                    else if (strcmp(eventResults[i].medal, "Silver") == 0) { 
                        filteredCountries[j].silverMedals++; 
                    }
                    else if (strcmp(eventResults[i].medal, "Bronze") == 0) {
                        filteredCountries[j].bronzeMedals++;
                    }
                }
            }

            // If new country, add it
            if (!found && filteredCount < COUNTRIES_SIZE) {
                // Copy the country noc
                strcpy(filteredCountries[filteredCount].noc, eventResults[i].countryNoc);

                filteredCountries[filteredCount].totalMedals = 1;
                
                if (strcmp(eventResults[i].medal, "Gold") == 0) { 
                    // Set gold medals to 1
                    filteredCountries[filteredCount].goldMedals = 1;
                }
                else if (strcmp(eventResults[i].medal, "Silver") == 0) {
                    // Set silver medals to 1
                    filteredCountries[filteredCount].silverMedals = 1;
                }
                else if (strcmp(eventResults[i].medal, "Bronze") == 0) { 
                    // Set bronze medals to 1
                    filteredCountries[filteredCount].bronzeMedals = 1; 
                }

                // Add to the count of filtered countries
                filteredCount++;
            }
        }
    }

    // If there's no entries with the edition specified, display an error message, otherwise display the histogram
    if (filteredCount == 0) {
        printf("%s is not a valid edition.\n", edition);
    }
    else {
        // Variables for iteration and bar length
        int i;
        int j;
        int barLength;

        // Sort the filtered countries based on medals
        sortCountries(filteredCountries, filteredCount);

        // Print the histogram
        printf("\nTop 10 Countries for %s by Total Medals\n", edition);
        for (i = 0; i < 10 && i < filteredCount; i++) {
            // Each # represents 10 medals
            barLength = filteredCountries[i].totalMedals / 10; 
            printf("%s ", filteredCountries[i].noc);

            // Print the bar
            for (j = 0; j < barLength; j++) {
                printf("#");
            }

            // Print the number of medals
            printf(" (%d medals)\n", filteredCountries[i].totalMedals);
        }
    }
}

void sortCountries(struct Country countries[], int arrSize)
{
    // Variables for iteration
    int i, j;

    for (i = 0; i < arrSize - 1; i++) {
        for (j = 0; j < arrSize - i - 1; j++) {
            // Compare the countries at j and j + 1, perform swap if necessary
            if (compareCountries(&countries[j], &countries[j + 1]) > 0)  {
                swap(countries, j, j + 1);  
            }
        }
    }
}

void swap(struct Country countries[], int i, int j)
{
    // Perform a swap
    struct Country temp = countries[i];
    countries[i] = countries[j];
    countries[j] = temp;
}

void clearBuffer()
{
    // Empty the buffer
    while (getchar() != '\n');
}
