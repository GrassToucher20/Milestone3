#ifndef PROJECT_H
#define PROJECT_H
/******************************************************************************
 *
 * File: project.h
 * Author: Nevo Aftabi
 * Student Number: 172865214
 *
 * Description: This file has the neccesary macros and structures for the project, and function declarations on reading data from files
 *
 *****************************************************************************/

// Define a macro that allows unsafe functions
#define _CRT_SECURE_NO_WARNINGS

// Libraries needed for the project
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Macros defining the size of C strings in the program
#define EDITION_SIZE 24
#define COUNTRY_NOC_SIZE 3
#define MEDAL_SIZE 10
#define COUNTRY_NAME_SIZE 37
#define LINE_SIZE 1000

// Macros for number of records in the CSV files
#define OLYMPIC_ATHLETE_EVENT_SIZE 316835
#define COUNTRIES_SIZE 236

// Macros defining file read success and failure
#define FILE_READ_FAIL 0
#define FILE_READ_SUCCESS 1

// Macros for the filepaths
#define OLYMPIC_ATHLETE_EVENT_RESULTS_FILEPATH "Olympic_Athlete_Event_Results.csv"
#define OLYMPICS_COUNTRIES_FILEPATH "Olympics_Country.csv"

// The number of fields that need to be read in
#define NUM_OLYMPIC_ATHLETE_EVENT_RESULT_FIELDS 3
#define NUM_COUNTRY_FIELDS 2
#define SEASON_SIZE 6

// This struct represents the data stored in a single row in the file
struct OlympicAthleteEventResult
{
	char edition[EDITION_SIZE + 1];
	char countryNoc[COUNTRY_NOC_SIZE + 1];
	char medal[MEDAL_SIZE + 1];
};


// This struct represents the data stored in a single row in the countries CSV file
struct Country
{
	char noc[COUNTRY_NOC_SIZE + 1];
	char country[COUNTRY_NAME_SIZE + 1];
	int totalMedals;
	int goldMedals;
	int silverMedals;
	int bronzeMedals;
};

/*
This functions reads in the olympic athlete event results file and assigns data to eventResults
eventResults: an array of results. It is assumed that it has the size OLYMPIC_ATHLETE_EVENT_RESULT_SIZE
countries: an array of countries. It is assumed it is of size COUNTRIES_SIZE
edition: The full edition of the olympics the user wants
fileReadSuccess: A pointer to a variable saying if the file read was successful or not
Returns value: The number of records stored in the eventResults array
*/
int readOlympicAthleteEventResults(struct OlympicAthleteEventResult eventResults[], struct Country countries[], const char* userEdition, int* fileReadSuccess);

/*
This function reads in the olympics countries into olympicsCountries 
olympicsCountries: an array that the country data will be assigned to
fileReadSuccess: A pointer to a variable saying if the file read was successful or not
Returns value: The number of records read in from the file
*/
int readOlympicsCountry(struct Country olympicsCountries[], int* fileReadSuccess);

/*
This is a comparator function.
It compares 2 countries based on number of medals, breaking ties with gold, silver, and bronze
country1: A pointer to the first country
country2: A pointer to the second country
Returns: The difference between the medals of the two countries
*/
int compareCountries(struct Country* country1, struct Country* country2);

/*
This function prints a histogram of the top 10 countries' metals to the screen
countries: an array of countries
eventResults: an array of the event results
edition: edition of the olympics the user wants results from
olympicAthleteEventResultsSize: the number of useful elements in the eventResults array
*/
void printHistogram(struct Country countries[], struct OlympicAthleteEventResult eventResults[], const char* edition, int olympicAthleteEventResultsSize);

/*
This function sorts the countries using the comprator function
countries: an array of countries
arrSize: the size of the filtered countries array
*/
void sortCountries(struct Country countries[], int arrSize);

/* 
Swaps the places of the two items in the array
countries: an array of countries
*/
void swap(struct Country countries[], int i, int j);

/*
Clear the buffer
*/
void clearBuffer();

#endif