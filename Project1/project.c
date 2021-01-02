#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <conio.h>

// IMPORTING OTHER CODES
#include "codes/structs.h"
#include "codes/functions.c"

#define TOTAL (10)

#define destinationsFile "assets/destination.txt"
#define transactionsFile "assets/historyTransaction.txt"
#define reportFile "assets/salesReport.txt"

#define themeColor "COLOR 10"

/* ===================== FUNCTIONS (from functions.c)========================= */
int getDest(FILE *, Category *, int *);
void printAndRecordDest(Category *, int, Information (*)[]);

int anotherService(void);

void run(FILE *, CurrentUser *, int, Information (*)[], SalesReport *, char[]);

void promptUser(CurrentUser *, int);
void outputUser(CurrentUser *, Information (*)[]);

void recordTransaction(FILE *, CurrentUser *, SalesReport *, char[], Information (*)[]);

void printReport(FILE *, int, Information (*)[], SalesReport *, char[]);

void initialize(SalesReport *, FILE *, char[], FILE *, char[]);

void displayAmount(float);
void fprintAmount(float, FILE *);

/* ===============================MAIN PROGRAM========================= */
int main(void)
{
  Documents Files;          // Files
  Category currentCategory; // For getting information
  static unsigned int totalCategories = 0;
  Information categories[TOTAL]; // To track the informations of destinations with an array
  SalesReport reportTrack;       // To track the final data while the program executing

  // Prepare the files for the user
  initialize(&reportTrack, Files.appendTransaction, transactionsFile, Files.salesReport, reportFile);

  // Open and check the file
  Files.getDestinations = fopen(destinationsFile, "r");
  if (!Files.getDestinations)
  {
    printf("\aError opening %s file\n", destinationsFile);
    return 100;
  }

  // Print the destination categories
  static int i = 0;

  printf("\nFLIGHT DESTINATION\n");
  while (getDest(Files.getDestinations, &currentCategory, &totalCategories))
  {
    printAndRecordDest(&currentCategory, ++i, &categories);
  }

  // Prompt the user, display and record the transaction, and track the final data
  CurrentUser currentUser;
  do
  {
    //Run(invoke) all the necessary functions to call per transactions
    run(Files.appendTransaction, &currentUser,
        totalCategories, &categories,
        &reportTrack, transactionsFile);

  } while (anotherService());

  // display the final data and record it to the file
  printReport(Files.salesReport, totalCategories,
              &categories, &reportTrack, reportFile);

  return 0;
}


