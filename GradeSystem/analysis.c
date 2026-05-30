/*
 * Member C responsibility:
 * Grade sorting, grade statistics and score distribution analysis.
 */
#include <stdio.h>
#include <string.h>
#include "student.h"
/* Return 1 when two student records should be swapped for the selected order. */
static int shouldSwap(Student left, Student right, SortType type) {
    switch (type) {
        case SORT_BY_TOTAL_DESC: return left.total < right.total;
        case SORT_BY_AVERAGE_DESC: return left.average < right.average;
        case SORT_BY_C_DESC: return left.cScore < right.cScore;
        case SORT_BY_MATH_DESC: return left.mathScore < right.mathScore;
        case SORT_BY_ENGLISH_DESC: return left.englishScore < right.englishScore;
        case SORT_BY_NUM_ASC: return strcmp(left.num, right.num) > 0;
        case SORT_BY_C_ASC: return left.cScore > right.cScore;
        case SORT_BY_MATH_ASC: return left.mathScore > right.mathScore;
        case SORT_BY_ENGLISH_ASC: return left.englishScore > right.englishScore;
        default: return 0;
    }
}
static const char *getSortTypeName(SortType type) {
    switch (type) {
        case SORT_BY_TOTAL_DESC: return "total score descending";
        case SORT_BY_AVERAGE_DESC: return "average score descending";
        case SORT_BY_C_DESC: return "C score descending";
        case SORT_BY_MATH_DESC: return "math score descending";
        case SORT_BY_ENGLISH_DESC: return "English score descending";
        case SORT_BY_NUM_ASC: return "student number ascending";
        case SORT_BY_C_ASC: return "C score ascending";
        case SORT_BY_MATH_ASC: return "math score ascending";
        case SORT_BY_ENGLISH_ASC: return "English score ascending";
        default: return "unknown";
    }
}
void sortStudents(Node *head, SortType type) {
    Node *i, *j;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    /* Swap node data instead of changing links; this is easier for beginners. */
    for (i = head->next; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (shouldSwap(i->data, j->data, type)) {
                swapStudentData(i, j);
            }
        }
    }
    printf("Sorted by %s.\n", getSortTypeName(type));
    displayAllStudents(head);
}
static void showSortMenu(void) {
    printf("\n--- Grade Sort ---\n");
    printf("1. Total score descending\n");
    printf("2. Average score descending\n");
    printf("3. C score descending\n");
    printf("4. Math score descending\n");
    printf("5. English score descending\n");
    printf("6. Student number ascending\n");
    printf("7. C score ascending\n");
    printf("8. Math score ascending\n");
    printf("9. English score ascending\n");
    printf("0. Return\n");
}
void sortMenu(Node *head) {
    int choice, running = 1;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    while (running) {
        showSortMenu();
        choice = readInt("Choose: ", 0, 9);
        if (choice == 0) running = 0;
        else sortStudents(head, (SortType)choice);
    }
}
static int getRangeIndex(float score) {
    /* 0: 90-100, 1: 80-89, 2: 70-79, 3: 60-69, 4: below 60 */
    if (score >= 90.0f) return 0;
    if (score >= 80.0f) return 1;
    if (score >= 70.0f) return 2;
    if (score >= 60.0f) return 3;
    return 4;
}
static void initSubjectStat(SubjectStat *stat) {
    int i;
    stat->maxScore = -1.0f;
    stat->minScore = 101.0f;
    stat->sum = 0.0f;
    stat->passCount = 0;
    stat->excellentCount = 0;
    memset(&stat->maxStudent, 0, sizeof(Student));
    memset(&stat->minStudent, 0, sizeof(Student));
    for (i = 0; i < 5; i++) stat->ranges[i] = 0;
}
/* Update one subject's max, min, sum, pass/excellent count and range count. */
static void updateSubjectStat(SubjectStat *stat, Student stu, float score) {
    int rangeIndex;
    if (score > stat->maxScore) {
        stat->maxScore = score;
        stat->maxStudent = stu;
    }
    if (score < stat->minScore) {
        stat->minScore = score;
        stat->minStudent = stu;
    }
    stat->sum += score;
    if (score >= PASS_SCORE) stat->passCount++;
    if (score >= EXCELLENT_SCORE) stat->excellentCount++;
    rangeIndex = getRangeIndex(score);
    stat->ranges[rangeIndex]++;
}
static void collectStatistics(Node *head,
                              SubjectStat *cStat,
                              SubjectStat *mathStat,
                              SubjectStat *engStat,
                              int *count,
                              float *totalSum) {
    Node *p;
    /* The three subjects share the same collection logic. */
    initSubjectStat(cStat);
    initSubjectStat(mathStat);
    initSubjectStat(engStat);
    *count = 0;
    *totalSum = 0.0f;
    p = head->next;
    while (p != NULL) {
        updateSubjectStat(cStat, p->data, p->data.cScore);
        updateSubjectStat(mathStat, p->data, p->data.mathScore);
        updateSubjectStat(engStat, p->data, p->data.englishScore);
        *totalSum += p->data.total;
        (*count)++;
        p = p->next;
    }
}
static void printSubjectStatLine(const char *subjectName,
                                 SubjectStat stat,
                                 int count) {
    float average = 0.0f, passRate = 0.0f, excellentRate = 0.0f;
    if (count > 0) {
        average = stat.sum / count;
        passRate = stat.passCount * 100.0f / count;
        excellentRate = stat.excellentCount * 100.0f / count;
    }
    printf("%-10s %10.2f %10.2f %10.2f %10d %9.2f%% %10d %9.2f%%\n",
           subjectName,
           stat.maxScore,
           stat.minScore,
           average,
           stat.passCount,
           passRate,
           stat.excellentCount,
           excellentRate);
}
void displaySubjectStatistics(Node *head) {
    SubjectStat cStat, mathStat, engStat;
    int count;
    float totalSum;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    collectStatistics(head, &cStat, &mathStat, &engStat, &count, &totalSum);
    printf("\n--- Subject Statistics ---\n");
    printf("%-10s %10s %10s %10s %10s %10s %10s %10s\n",
           "Subject", "Max", "Min", "Average", "PassNum", "PassRate", "Excellent", "ExRate");
    printf("--------------------------------------------------------------------------------\n");
    printSubjectStatLine("C", cStat, count);
    printSubjectStatLine("Math", mathStat, count);
    printSubjectStatLine("English", engStat, count);
}
static void printRangeLine(const char *subjectName, SubjectStat stat) {
    printf("%-10s %10d %10d %10d %10d %10d\n",
           subjectName,
           stat.ranges[0],
           stat.ranges[1],
           stat.ranges[2],
           stat.ranges[3],
           stat.ranges[4]);
}
void displayDistribution(Node *head) {
    SubjectStat cStat, mathStat, engStat;
    int count;
    float totalSum;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    collectStatistics(head, &cStat, &mathStat, &engStat, &count, &totalSum);
    printf("\n--- Score Distribution ---\n");
    printf("%-10s %10s %10s %10s %10s %10s\n",
           "Subject", "90-100", "80-89", "70-79", "60-69", "Below60");
    printf("--------------------------------------------------------------------\n");
    printRangeLine("C", cStat);
    printRangeLine("Math", mathStat);
    printRangeLine("English", engStat);
}
static void printTopBottomForSubject(const char *subjectName, SubjectStat stat) {
    printf("\n%s max: %.2f, student: %s %s\n",
           subjectName, stat.maxScore, stat.maxStudent.num, stat.maxStudent.name);
    printf("%s min: %.2f, student: %s %s\n",
           subjectName, stat.minScore, stat.minStudent.num, stat.minStudent.name);
}
void displayTopAndBottom(Node *head) {
    SubjectStat cStat, mathStat, engStat;
    int count;
    float totalSum;
    Node *p;
    Student totalMax, totalMin;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    collectStatistics(head, &cStat, &mathStat, &engStat, &count, &totalSum);
    printTopBottomForSubject("C", cStat);
    printTopBottomForSubject("Math", mathStat);
    printTopBottomForSubject("English", engStat);
    p = head->next;
    totalMax = p->data;
    totalMin = p->data;
    while (p != NULL) {
        if (p->data.total > totalMax.total) totalMax = p->data;
        if (p->data.total < totalMin.total) totalMin = p->data;
        p = p->next;
    }
    printf("\nTotal max: %.2f, student: %s %s\n",
           totalMax.total, totalMax.num, totalMax.name);
    printf("Total min: %.2f, student: %s %s\n",
           totalMin.total, totalMin.num, totalMin.name);
}
void displayClassSummary(Node *head) {
    Node *p;
    int count = 0, allPassCount = 0, allExcellentCount = 0;
    float totalSum = 0.0f;
    Student bestAverage, worstAverage;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    p = head->next;
    /* Initialize best/worst students with the first real node. */
    bestAverage = p->data;
    worstAverage = p->data;
    while (p != NULL) {
        totalSum += p->data.total;
        if (p->data.cScore >= PASS_SCORE &&
            p->data.mathScore >= PASS_SCORE &&
            p->data.englishScore >= PASS_SCORE) {
            allPassCount++;
        }
        if (p->data.average >= EXCELLENT_SCORE) allExcellentCount++;
        if (p->data.average > bestAverage.average) bestAverage = p->data;
        if (p->data.average < worstAverage.average) worstAverage = p->data;
        count++;
        p = p->next;
    }
    printf("\n--- Class Summary ---\n");
    printf("Student count: %d\n", count);
    printf("Class total average: %.2f\n", totalSum / (count * 3.0f));
    printf("All-subject pass count: %d\n", allPassCount);
    printf("Average excellent count: %d\n", allExcellentCount);
    printf("Best average student: %s %s %.2f\n",
           bestAverage.num, bestAverage.name, bestAverage.average);
    printf("Lowest average student: %s %s %.2f\n",
           worstAverage.num, worstAverage.name, worstAverage.average);
}
static void showStatisticMenu(void) {
    printf("\n--- Grade Statistics ---\n");
    printf("1. Subject max/min/average/pass/excellent\n");
    printf("2. Score distribution\n");
    printf("3. Top and bottom students\n");
    printf("4. Class summary\n");
    printf("5. All statistics\n");
    printf("0. Return\n");
}
void statisticMenu(Node *head) {
    int choice, running = 1;
    if (isListEmpty(head)) {
        printf("No student record.\n");
        return;
    }
    while (running) {
        showStatisticMenu();
        choice = readInt("Choose: ", 0, 5);
        switch (choice) {
            case 1: displaySubjectStatistics(head); break;
            case 2: displayDistribution(head); break;
            case 3: displayTopAndBottom(head); break;
            case 4: displayClassSummary(head); break;
            case 5:
                displaySubjectStatistics(head);
                displayDistribution(head);
                displayTopAndBottom(head);
                displayClassSummary(head);
                break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n"); break;
        }
    }
}
