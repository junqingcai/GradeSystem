/*
 * Member C responsibility:
 * Read records from data.txt, save records, backup data and export report.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
/* Check whether the data file already exists before backup or loading. */
int fileExists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;
    fclose(fp);
    return 1;
}
/* Create an empty data file when data.txt does not exist at first run. */
int createEmptyDataFile(const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Cannot create file %s.\n", filename);
        return 0;
    }
    fclose(fp);
    return 1;
}
/* Convert one text line into a Student structure and validate all fields. */
static int parseStudentLine(const char *line, Student *stu) {
    char extra[LINE_LEN];
    if (line == NULL || stu == NULL) return 0;
    memset(stu, 0, sizeof(Student));
    if (sscanf(line, "%19s %19s %f %f %f %255s",
               stu->num, stu->name,
               &stu->cScore, &stu->mathScore, &stu->englishScore,
               extra) != 5) {
        return 0;
    }
    if (!isNumFormatValid(stu->num) || !isNameFormatValid(stu->name)) return 0;
    if (!isScoreValid(stu->cScore) ||
        !isScoreValid(stu->mathScore) ||
        !isScoreValid(stu->englishScore)) {
        return 0;
    }
    calculateScore(stu);
    return 1;
}
int loadFromFile(Node *head, const char *filename) {
    FILE *fp;
    char line[LINE_LEN];
    Student stu;
    int count = 0, lineNumber = 0, badCount = 0, duplicateCount = 0;
    if (head == NULL || filename == NULL) return 0;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("%s not found. Creating an empty data file.\n", filename);
        createEmptyDataFile(filename);
        return 0;
    }
    /* Reloading should replace old memory data, so clear the list first. */
    clearList(head);
    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNumber++;
        if (line[0] == '\n' || line[0] == '\0') continue;
        if (!parseStudentLine(line, &stu)) {
            printf("Line %d has invalid format and is skipped.\n", lineNumber);
            badCount++;
            continue;
        }
        if (!appendStudent(head, stu)) {
            printf("Line %d is duplicate or cannot be loaded.\n", lineNumber);
            duplicateCount++;
            continue;
        }
        count++;
    }
    fclose(fp);
    printf("Loaded %d records. Invalid: %d, duplicate: %d.\n",
           count, badCount, duplicateCount);
    return count;
}
int backupDataFile(const char *sourceName, const char *backupName) {
    FILE *src, *dst;
    int ch, count = 0;
    if (sourceName == NULL || backupName == NULL) return 0;
    src = fopen(sourceName, "r");
    if (src == NULL) {
        printf("Source file %s does not exist. Backup failed.\n", sourceName);
        return 0;
    }
    dst = fopen(backupName, "w");
    if (dst == NULL) {
        fclose(src);
        printf("Cannot create backup file %s.\n", backupName);
        return 0;
    }
    /* Character-by-character copy is simple and suitable for freshman C. */
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
        count++;
    }
    fclose(src);
    fclose(dst);
    printf("Backed up %d characters to %s.\n", count, backupName);
    return 1;
}
int saveToFile(Node *head, const char *filename) {
    FILE *fp;
    Node *p;
    int count = 0;
    if (head == NULL || filename == NULL) return 0;
    /* Backup old data before overwriting data.txt. */
    if (fileExists(filename)) backupDataFile(filename, BACKUP_FILE);
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Cannot open %s. Save failed.\n", filename);
        return 0;
    }
    p = head->next;
    /* Only basic fields are saved; total and average are recalculated after loading. */
    while (p != NULL) {
        fprintf(fp, "%s %s %.2f %.2f %.2f\n",
                p->data.num, p->data.name,
                p->data.cScore, p->data.mathScore,
                p->data.englishScore);
        count++;
        p = p->next;
    }
    fclose(fp);
    printf("Saved %d records to %s.\n", count, filename);
    return count;
}
static void writeReportHeader(FILE *fp) {
    fprintf(fp, "Student Grade Management System Report\n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "%-15s %-15s %10s %10s %10s %10s %10s\n",
            "Num", "Name", "C", "Math", "English", "Total", "Average");
    fprintf(fp, "--------------------------------------------------------------------------------\n");
}
static void writeReportStudent(FILE *fp, Student stu) {
    fprintf(fp, "%-15s %-15s %10.2f %10.2f %10.2f %10.2f %10.2f\n",
            stu.num, stu.name, stu.cScore, stu.mathScore,
            stu.englishScore, stu.total, stu.average);
}
int exportReport(Node *head, const char *filename) {
    FILE *fp;
    Node *p;
    int count = 0;
    if (head == NULL || filename == NULL) return 0;
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Cannot create export file %s.\n", filename);
        return 0;
    }
    writeReportHeader(fp);
    p = head->next;
    while (p != NULL) {
        writeReportStudent(fp, p->data);
        count++;
        p = p->next;
    }
    fprintf(fp, "--------------------------------------------------------------------------------\n");
    fprintf(fp, "Total records: %d\n", count);
    fclose(fp);
    printf("Exported %d records to %s.\n", count, filename);
    return count;
}
static void showFileMenu(void) {
    printf("\n--- File Management ---\n");
    printf("1. Save to data.txt\n");
    printf("2. Reload from data.txt\n");
    printf("3. Backup data.txt to backup.txt\n");
    printf("4. Export report to export.txt\n");
    printf("5. Show file format help\n");
    printf("0. Return\n");
}
void fileMenu(Node *head) {
    int choice;
    int running = 1;
    while (running) {
        showFileMenu();
        choice = readInt("Choose: ", 0, 5);
        switch (choice) {
            case 1: saveToFile(head, DATA_FILE); break;
            case 2:
                if (askYesNo("Reload will overwrite unsaved memory data. Continue? (y/n): ")) {
                    loadFromFile(head, DATA_FILE);
                }
                break;
            case 3: backupDataFile(DATA_FILE, BACKUP_FILE); break;
            case 4: exportReport(head, EXPORT_FILE); break;
            case 5: showFileFormatHelp(); break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n"); break;
        }
    }
}
