#include <stdio.h>
#include <conio.h>
/* HERE ARE THE FUNCTIONS  */
#define TOTAL (10)

// For format styling of number with commas
void displayAmount(float amount)
{
  char stringPayment[16];
  sprintf(stringPayment, "%.2f", amount);
  size_t size = strlen(stringPayment);
  int commas = 0;
  if (amount >= 1000)
  {
    for (int i = 0; i < size; i++)
    {
      int index = size - i;
      if (index % 3 == 0 && index != 0 && i < size - 3 && i != 0)
      {
        printf(",");
        commas++;
      }
      printf("%c", stringPayment[i]);
    }
  }
  else
  {
    printf("%.2f", amount);
  }

  for (int i = 14 - (size + commas); i > 0; i--)
  {
    printf(" ");
  }
  return;
}

void fprintAmount(float amount, FILE *file)
{

  char stringPayment[16];
  sprintf(stringPayment, "%.f", amount);
  size_t size = strlen(stringPayment);
  int commas = 0;

  if (amount >= 1000)
  {

    for (int i = 0; i < size; i++)
    {
      int index = size - i;
      if (index % 3 == 0 && index != 0 /* && i < size - 3 && i != 0 */)
      {
        fprintf(file, ",");
        commas++;
      }
      fprintf(file, "%c", stringPayment[i]);
    }
  }
  else if (amount == 0)
  {
    fprintf(file, "%.2f", amount);
    size += 3;
  }
  else
  {
    fprintf(file, "%.f", amount);
  }
  for (int i = 14 - (size + commas); i > 0; i--)
  {
    fprintf(file, " ");
  }
  return;
}

// initializes the files and arrays
void initialize(SalesReport *report, FILE *transaction,
                char transactionFile[], FILE *salesreport,
                char salesreportFile[])
{
  transaction = fopen(transactionFile, "w");
  fprintf(transaction, "%- 13s %- 6s %s\n", "Destination", "Age", "Sales Amount");
  fclose(transaction);

  salesreport = fopen(salesreportFile, "w");
  fprintf(salesreport, "%- 17s %- 9s %- 13s %s\n\n", "Flight", "Quantity", "Amount", "Travel tax");
  fclose(salesreport);

  for (int i = 0; i < TOTAL; i++)
  {
    report->reports[i].amount = 0;
    report->reports[i].quantity = 0;
  }
  report->totalReport.totalQuantity = 0;
  report->totalReport.totalAmount = 0;
  return;
}

// for getting the destinations
int getDest(FILE *file, Category *category, int *totalCategories)
{
  // Read the file and check if its valid; return number (1 / 0)
  int result;

  while ((result = fscanf(file, "%s %d %d", &category->country, &category->price, &category->tax)) == 1)
  {
    return getDest(file, category, totalCategories);
  }

  if (result == EOF)
    return 0;
  else if (result != 3)
  {
    printf("\aError reading data\n");
    return 0;
  }
  else
  {
    *totalCategories += 1;
    return 1;
  }
} //getDest

// Displays and record the destination
void printAndRecordDest(Category *category, int index,
                        Information (*categories)[])
{
  // Display and store the destination in the categories(array of struct)
  int i = index - 1;

  // STORE IT in the categories with respect to their index
  (*categories)[i].primaryKey = index;
  strcpy(((*categories)[i]).country, category->country);
  (*categories)[i].price = category->price;
  (*categories)[i].tax = category->tax * 0.01;

  // PRINT THE DESTINATION CATEGORIES
  printf("%- 4d %- 16s ", index, category->country);
  displayAmount((float)category->price);
  printf("\n");
  return;
} // printDest

// promp the user of another service
int anotherService(void)
{
  // prompt user for another service
  printf("\n\nAnother Service (Y/N)?: ");
  char answer[1];
  scanf("%s", answer);
  if (!strcmp(answer, "Y") || !strcmp(answer, "y"))
  {
    return 1;
  }
  else if (!strcmp(answer, "N") || !strcmp(answer, "n"))
  {
    return 0;
  }
  else
  {
    printf("\nInvalid input, Please try again");
    return anotherService();
  }

} // anotherService

// prompt the user for transaction
void promptUser(CurrentUser *user, int totalCategories)
{
  // prompt user to get input data
  printf("\n%- 30s: ", "Enter your flight destination");
  scanf("%d", &user->destination);
  if (user->destination > totalCategories || user->destination < 1)
  {
    printf("\n!!Invalid input, please try again!!");
    return promptUser(user, totalCategories);
  }
  printf("%- 30s: ", "Enter your age");
  scanf("%d", &user->age);
  if (user->age < 0 || user->age > 300)
  {
    printf("\n!!Invalid input, please try again!!");
    return promptUser(user, totalCategories);
  }
  return;
} // promptUser

// displays the transaction
void outputUser(CurrentUser *user, Information (*categories)[])
{
  int age = user->age;
  int i = user->destination - 1;
  Information destination = (*categories)[user->destination - 1];

  if (age == 0)
  {
    user->payment = 0;
  }
  else if (age >= 2 && age <= 5)
  {
    user->payment = destination.price * (float)0.90;
  }
  else if (age > 60)
  {
    user->payment = destination.price * (float)0.80;
  }
  else
  {
    user->payment = destination.price;
  }

  printf("\n\n%- 13s: Php ", "Ticket amount");
  displayAmount(user->payment);
  printf("\n%- 13s: Php ", "Travel tax");
  displayAmount((user->payment) * destination.tax);

  return;
} // ouputUser

// records the transaction in file
void recordTransaction(FILE *file, CurrentUser *user,
                       SalesReport *reportTrack, char fileName[],
                       Information (*categories)[])
{
  int index = user->destination - 1;

  file = fopen(fileName, "a");

  fprintf(file, "\n%- 12s %- 7d ", (*categories)[index].country, user->age);
  fprintAmount(user->payment, file);
  fclose(file);

  // For Report
  reportTrack->reports[index].quantity += 1;
  reportTrack->reports[index].amount += user->payment;

  // For Total Report
  reportTrack->totalReport.totalQuantity += 1;
  reportTrack->totalReport.totalAmount += user->payment;
  return;

} // recordTransaction

// Run all the necessary functions for the program
void run(FILE *append, CurrentUser *currentUser,
         int totalCategories, Information (*categories)[],
         SalesReport *reportTrack, char fileName[])
{
  promptUser(currentUser, totalCategories);
  outputUser(currentUser, categories);
  recordTransaction(append, currentUser, reportTrack, fileName, categories);
  return;
}

// Displays the sales report and record it in the file
void printReport(FILE *file, int totalCategories,
                 Information (*categories)[],
                 SalesReport *reportTrack, char fileName[])
{
  float totalTax = 0;
  float currentTax;
  file = fopen(fileName, "a");



  printf("\n\tPrinting Flight Ticketing Report...\n\n");
  sleep(1);
  printf("%- 17s %- 9s %- 13s %s\n\n", "Flight", "Quantity", "Amount", "Travel tax");
  for (int i = 0; i < totalCategories; i++)
  {

    currentTax = reportTrack->reports[i].amount * (*categories)[i].tax;
    printf("%- 16s %- 10d ", (*categories)[i].country, reportTrack->reports[i].quantity);
    displayAmount(reportTrack->reports[i].amount);
    displayAmount(currentTax);
    printf("\n");

    fprintf(file, "%- 16s %- 10d ", (*categories)[i].country, reportTrack->reports[i].quantity);
    fprintAmount(reportTrack->reports[i].amount, file);
    fprintAmount(currentTax, file);
    fprintf(file, "\n");
    totalTax += currentTax;
    
  }
  for(int i = 0; i < 18; i++)
  {
    printf("---"); 
  }
  printf("\n");
  fprintf(file, "\n%- 16s %- 10d ", "TOTAL", reportTrack->totalReport.totalQuantity);
  fprintAmount(reportTrack->totalReport.totalAmount, file);
  fprintAmount(totalTax, file); 
  

  printf("%- 16s %- 10d ", "TOTAL", reportTrack->totalReport.totalQuantity);
  displayAmount(reportTrack->totalReport.totalAmount);
  displayAmount(totalTax);
  printf("\n");
  fclose(file);
  return;

} //printReport
//XD
