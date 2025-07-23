#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_HISTORY 100
#define MAX_VARS 50
#define MAX_NAME 50
#define MAX_TYPE 20
#define MAX_VAL 100

// ANSI color codes
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

typedef struct {
    char value[MAX_VAL];
    time_t timestamp;
} History;

typedef struct {
    char name[MAX_NAME];
    char typeLabel[MAX_TYPE];
    History history[MAX_HISTORY];
    int count;
    int currentIndex;
} Variable;

Variable vars[MAX_VARS];
int varCount = 0;

void getTimeStr(time_t t, char *buf) {
    strftime(buf, 30, "%Y-%m-%d %H:%M:%S", localtime(&t));
}

int findVar(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    return -1;
}

void setVariable() {
    char name[MAX_NAME], value[MAX_VAL], type[MAX_TYPE];
    printf(CYAN "Enter variable name: " RESET);
    scanf(" %s", name);
    int idx = findVar(name);

    printf(CYAN "Enter data type label (e.g., int, float, string, bool): " RESET);
    scanf(" %s", type);

    printf(CYAN "Enter value: " RESET);
    scanf(" %[^\n]", value);

    if (idx == -1) {
        idx = varCount++;
        strcpy(vars[idx].name, name);
        vars[idx].count = 0;
    }

    strcpy(vars[idx].typeLabel, type);
    strcpy(vars[idx].history[vars[idx].count].value, value);
    vars[idx].history[vars[idx].count].timestamp = time(NULL);
    vars[idx].currentIndex = vars[idx].count;
    vars[idx].count++;

    printf(GREEN "✔ Variable '%s' updated to '%s'\n" RESET, name, value);
}

void stepBack() {
    for (int i = 0; i < varCount; i++) {
        if (vars[i].currentIndex > 0)
            vars[i].currentIndex--;
    }
    printf(YELLOW "⬅️ Stepped back.\n" RESET);
}

void stepForward() {
    for (int i = 0; i < varCount; i++) {
        if (vars[i].currentIndex < vars[i].count - 1)
            vars[i].currentIndex++;
    }
    printf(GREEN "➡️ Stepped forward.\n" RESET);
}

void showHistory() {
    for (int i = 0; i < varCount; i++) {
        printf("\n🔹 Variable: %s (%s)\n", vars[i].name, vars[i].typeLabel);
        for (int j = 0; j < vars[i].count; j++) {
            char timeStr[30];
            getTimeStr(vars[i].history[j].timestamp, timeStr);
            printf("  Step %d: %s [%s]%s\n", j + 1,
                vars[i].history[j].value, timeStr,
                (j == vars[i].currentIndex ? "  <-- CURRENT" : ""));
        }
    }
}

void exportSession() {
    FILE *f = fopen("debug_aura_session.txt", "w");
    for (int i = 0; i < varCount; i++) {
        fprintf(f, "Variable: %s (%s)\n", vars[i].name, vars[i].typeLabel);
        for (int j = 0; j < vars[i].count; j++) {
            char timeStr[30];
            getTimeStr(vars[i].history[j].timestamp, timeStr);
            fprintf(f, "  Step %d: %s [%s]%s\n", j + 1,
                vars[i].history[j].value, timeStr,
                (j == vars[i].currentIndex ? "  <-- CURRENT" : ""));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf(GREEN "📄 Session exported to 'debug_aura_session.txt'\n" RESET);
}

void smartSummary() {
    printf("\n🧠 Smart Summary:\n");
    for (int i = 0; i < varCount; i++) {
        char timeStr[30];
        getTimeStr(vars[i].history[vars[i].currentIndex].timestamp, timeStr);
        printf("  🧷 %s (%s): %s at %s\n", vars[i].name, vars[i].typeLabel,
            vars[i].history[vars[i].currentIndex].value, timeStr);
    }
}

void searchByValue() {
    char search[MAX_VAL];
    printf(CYAN "Enter value to search: " RESET);
    scanf(" %[^\n]", search);
    int found = 0;

    for (int i = 0; i < varCount; i++) {
        for (int j = 0; j < vars[i].count; j++) {
            if (strcmp(vars[i].history[j].value, search) == 0) {
                char timeStr[30];
                getTimeStr(vars[i].history[j].timestamp, timeStr);
                printf(GREEN "✔ Found in '%s' (%s) at step %d: %s [at %s]%s\n" RESET,
                       vars[i].name, vars[i].typeLabel, j + 1,
                       vars[i].history[j].value, timeStr,
                       (j == vars[i].currentIndex ? " <- CURRENT" : ""));
                found = 1;
            }
        }
    }

    if (!found) {
        printf(RED "❌ No matches found.\n" RESET);
    }
}

void menu() {
    int choice;
    do {
        printf("\n" YELLOW "========== 🔧 DEBUG AURA MENU 🔧 ==========\n" RESET);
        printf(CYAN "1. Set Variable\n");
        printf("2. Step Back\n");
        printf("3. Step Forward\n");
        printf("4. Show History\n");
        printf("5. Export Session\n");
        printf("6. Smart Summary\n");
        printf("7. Exit\n");
        printf("8. Search Variable by Value\n" RESET);
        printf(YELLOW "===========================================\n" RESET);
        printf("Enter choice: ");
        scanf("%d", &choice); getchar();

        switch (choice) {
            case 1: setVariable(); break;
            case 2: stepBack(); break;
            case 3: stepForward(); break;
            case 4: showHistory(); break;
            case 5: exportSession(); break;
            case 6: smartSummary(); break;
            case 7: printf(RED "Goodbye from Debug Aura!\n" RESET); break;
            case 8: searchByValue(); break;
            default: printf(RED "Invalid choice.\n" RESET);
        }
    } while (choice != 7);
}

int main() {
    printf(GREEN "\n💡 Welcome to Debug Aura — Your Reverse Debugger\n" RESET);
    menu();
    return 0;
}