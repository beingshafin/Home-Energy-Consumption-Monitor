#include <stdio.h>
#include <string.h>

#define BOLD    "\033[1m"
#define RESET   "\033[0m\033[37m"

#define TEXT1   "\033[0m\033[37m" //white
#define TEXT2   "\033[1m\033[38;2;255;247;176m" //YELLOW
#define TEXT3   "\033[0m\033[38;2;203;226;250m" //very light blue
#define LOADING "\033[1m\033[38;2;152;207;255m" //BLUE
#define TITLE   "\033[1m\033[38;2;120;229;255m" //CYAN


#define GREEN   "\033[1m\033[38;2;172;255;182m"
#define RED     "\033[1m\033[38;2;224;135;142m"

#define YELLOW  "\033[1m\033[38;2;255;247;176m"
#define BLUE    "\033[1m\033[38;2;152;207;255m"
#define DARK_BLUE"\033[1m\033[38;2;88;177;255m"
#define MAGENTA "\033[1m\033[38;2;242;169;255m"
#define CYAN    "\033[1m\033[38;2;120;229;255m"

typedef struct EnergyUsage {
    char date[11]; // YYYY-MM-DD
    double kwh;
    double app[3]; // 0: HVAC, 1: Lighting, 2: Water Heater
} eu;

eu record[365];
int day = 0;
int quit = 0;

void clear() {
    printf("\033[2J\033[H");
}

void printTitle() {
    printf(BOLD);

    printf(CYAN);
    printf("€€ª  €€ª €€€€€€€ª  €€€€€€ª €€€ª   €€€ª\n");
    printf(GREEN);
    printf("€€∫  €€∫ €€…ÕÕÕÕº €€…ÕÕÕÕº €€€€ª €€€€∫\n");
    printf(YELLOW);
    printf("€€€€€€€∫ €€€€€ª   €€∫      €€…€€€€…€€∫\n");
    printf(BLUE);
    printf("€€…ÕÕ€€∫ €€…ÕÕº   €€∫      €€∫»€€…º€€∫\n");
    printf(MAGENTA);
    printf("€€∫  €€∫ €€€€€€€ª »€€€€€€ª €€∫ »Õº €€∫\n");
    printf(RED);
    printf("»Õº  »Õº »ÕÕÕÕÕÕº  »ÕÕÕÕÕº »Õº     »Õº\n");

    printf(CYAN);
    printf("   Home Energy Consumption Monitor\n");

    printf(RESET "\n");
}




void printDetails(int i) {
    printf(DARK_BLUE "\n%d) " BLUE "%s\n", i + 1, record[i].date); //s record header

    printf(TEXT3 "   Total kWh: " TEXT1 "%.2lf\n", record[i].kwh);      //s main value
    printf(TEXT3 "   HVAC: " TEXT1 "%.2lf\n", record[i].app[0]);       //s appliance
    printf(TEXT3 "   Lighting: " TEXT1 "%.2lf\n", record[i].app[1]);   //s appliance
    printf(TEXT3 "   Water Heater: " TEXT1 "%.2lf\n", record[i].app[2]); //s appliance
}

void view() {
    if (day == 0) {
        printf(RED "No records found.\n" RESET); //s error
        return;
    }

    printf(TITLE "\n*** All Energy Records ***\n" RESET); //s section title

    for (int i = 0; i < day; i++) {
        printDetails(i);
    }
}

void search() {
    char src[11];

    if (day == 0) {
        printf(RED "\nNo records to search.\n" RESET); //s error
        return;
    }

    printf(TITLE "\nEnter Date to search (YYYY-MM-DD): " RESET); //s input prompt
    scanf(" %s", src);

    for (int i = 0; i < day; i++) {
        if (strcmp(record[i].date, src) == 0) {
            printf(GREEN "\nFound Record:\n" RESET); //s success
            printDetails(i);
            return;
        }
    }

    printf(RED "Record not found.\n" RESET); //s error
}

void findHighest() {
    if (day == 0) {
        printf(RED "No records found.\n" RESET); //s error
        return;
    }

    int mi = 0;
    double mu = record[0].kwh;

    for (int i = 1; i < day; i++) {
        if (record[i].kwh > mu) {
            mu = record[i].kwh;
            mi = i;
        }
    }

    printf(TITLE "\nHighest Consumption Day:\n" RESET); //s section title
    printDetails(mi);
}

void save() {
    FILE *fp = fopen("energy.txt", "w");
    if (!fp) {
        printf(RED "Error saving file.\n" RESET); //s error
        return;
    }

    for (int i = 0; i < day; i++) {
        fprintf(fp, "%s,%.2lf,%.2lf,%.2lf,%.2lf\n",
                record[i].date,
                record[i].kwh,
                record[i].app[0],
                record[i].app[1],
                record[i].app[2]);
    }

    fclose(fp);
    printf(GREEN "%d Records saved successfully.\n" RESET, day); //s success
}

void load() {
    FILE *fp = fopen("energy.txt", "r");
    if (!fp) {
        printf(RED "No existing file found. Starting fresh.\n" RESET); //s warning
        return;
    }

    day = 0;
    while (fscanf(fp, "%[^,],%lf,%lf,%lf,%lf\n",
                  record[day].date,
                  &record[day].kwh,
                  &record[day].app[0],
                  &record[day].app[1],
                  &record[day].app[2]) == 5)
    {
        day++;
        if (day >= 365) break;
    }

    fclose(fp);
    printf(GREEN "Loaded %d records.\n" RESET, day); //s success
}

void add() {
    if (day >= 365) {
        printf(RED "Record list is full.\n" RESET); //s error
        return;
    }

    printf(LOADING "Adding new Energy Record...\n" RESET); //s info

    printf(TEXT2 "\nDate (YYYY-MM-DD): " RESET);
    scanf(" %s", record[day].date);

    printf(TEXT2 "Total kWh: " RESET);
    scanf(" %lf", &record[day].kwh);

    printf(TEXT2 "HVAC Consumption (kWh): " RESET);
    scanf(" %lf", &record[day].app[0]);

    printf(TEXT2 "Lighting Consumption (kWh): " RESET);
    scanf(" %lf", &record[day].app[1]);

    printf(TEXT2 "Water Heater Consumption (kWh): " RESET);
    scanf(" %lf", &record[day].app[2]);

    day++;
    printf(GREEN "\nRecord added successfully.\n" RESET); //s success
}

void menu() {
    int a;

    printTitle();

    printf(MAGENTA "1. " TEXT1 "Load Records\n");
    printf(MAGENTA "2. " TEXT1 "Save Records\n");
    printf(MAGENTA "3. " TEXT1 "Add New Record\n");
    printf(MAGENTA "4. " TEXT1 "View All Records\n");
    printf(MAGENTA "5. " TEXT1 "Search Record\n");
    printf(MAGENTA "6. " TEXT1 "Find Highest Consumption Day\n");
    printf(MAGENTA "7. " TEXT1 "Exit Program\n");

    printf(BLUE "\nEnter a Number [1-7] : " RESET); //s input
    scanf(" %d", &a);

    if (a == 1) load();
    else if (a == 2) save();
    else if (a == 3) add();
    else if (a == 4) view();
    else if (a == 5) search();
    else if (a == 6) findHighest();
    else if (a == 7) {
            clear();
            printf(CYAN "\nGood Bye, Shafin Loves you >_<\n" RESET);
            quit = 1;
    }
    else printf(RED "\nInvalid Choice\n" RESET); //s error
}

int main() {
    while (1) {
        clear();
        menu();
        if (quit) return 0;
        printf(RESET "\nPress Enter to continue..."); //s pause
        getchar();
        getchar();
    }
}
