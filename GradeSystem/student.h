/*
 * Student Grade Management System
 * Course Design Source File
 *
 * Project division for three students:
 * Member A:
 *     main.c
 *     Main function, main menu, second-level menus, help page,
 *     member division display and whole process control.
 *
 * Member B:
 *     student.c
 *     Linked-list creation, addition, deletion, search, modification,
 *     insertion, display, input validation and common list tools.
 *
 * Member C:
 *     file.c and analysis.c
 *     Data file reading, saving, backup, export, grade sorting,
 *     grade statistics and score distribution analysis.
 *
 * Data storage:
 *     The system uses a singly linked list in memory.
 *     The system uses data.txt as the persistent data file.
 *
 * Data format in data.txt:
 *     studentNumber studentName cScore mathScore englishScore
 *
 * Example:
 *     2024001 ZhangSan 88 92 80
 */
#ifndef STUDENT_H
#define STUDENT_H
#include <stdio.h>
/* Basic length limits. One extra byte is reserved for '\0'. */
#define NUM_LEN 20
#define NAME_LEN 20
#define LINE_LEN 256
/* File names used by the file management module. */
#define DATA_FILE "data.txt"
#define BACKUP_FILE "backup.txt"
#define EXPORT_FILE "export.txt"
/* Grade standards used by query and statistics functions. */
#define PASS_SCORE 60.0f
#define EXCELLENT_SCORE 90.0f
/* Subject codes are used when one function needs to handle several scores. */
#define SUBJECT_C 1
#define SUBJECT_MATH 2
#define SUBJECT_ENGLISH 3
#define SUBJECT_TOTAL 4
#define SUBJECT_AVERAGE 5
/* One student's complete score information. */
typedef struct Student {
    char num[NUM_LEN];
    char name[NAME_LEN];
    float cScore;
    float mathScore;
    float englishScore;
    float total;
    float average;
} Student;
/* Singly linked-list node. The program uses an extra head node. */
typedef struct Node {
    Student data;
    struct Node *next;
} Node;
/* Sorting choices used by analysis.c. */
typedef enum SortType {
    SORT_BY_TOTAL_DESC = 1,
    SORT_BY_AVERAGE_DESC,
    SORT_BY_C_DESC,
    SORT_BY_MATH_DESC,
    SORT_BY_ENGLISH_DESC,
    SORT_BY_NUM_ASC,
    SORT_BY_C_ASC,
    SORT_BY_MATH_ASC,
    SORT_BY_ENGLISH_ASC
} SortType;
/* Statistics for one subject. ranges[0..4] means 90+, 80+, 70+, 60+, below 60. */
typedef struct SubjectStat {
    float maxScore;
    float minScore;
    float sum;
    int passCount;
    int excellentCount;
    int ranges[5];
    Student maxStudent;
    Student minStudent;
} SubjectStat;
/* ---------- Common input helpers, mainly used by all modules ---------- */
void clearInputBuffer(void);
void pauseScreen(void);
int askYesNo(const char *prompt);
int readInt(const char *prompt, int minValue, int maxValue);
float readFloat(const char *prompt, float minValue, float maxValue);
float readScore(const char *prompt);
void readString(const char *prompt, char *buffer, int size);
/* ---------- Linked-list basic tools, Member B ---------- */
Node *createList(void);
Node *createNode(Student stu);
void freeList(Node *head);
void clearList(Node *head);
int isListEmpty(Node *head);
int getListLength(Node *head);
Node *getLastNode(Node *head);
Node *getNodeByIndex(Node *head, int index);
Node *findPrevByNum(Node *head, const char *num);
Node *findByNum(Node *head, const char *num);
int isNumExist(Node *head, const char *num);
int isNameExist(Node *head, const char *name);
int appendStudent(Node *head, Student stu);
int insertBeforeNum(Node *head, const char *beforeNum, Student stu);
int removeByNum(Node *head, const char *num, Student *removed);
void swapStudentData(Node *a, Node *b);
/* ---------- Student data tools, Member B ---------- */
void calculateScore(Student *stu);
int isScoreValid(float score);
int isNumFormatValid(const char *num);
int isNameFormatValid(const char *name);
float getStudentScore(Student stu, int subject);
const char *getSubjectName(int subject);
Student inputStudent(Node *head, int checkDuplicate);
void inputScores(Student *stu);
void copyStudent(Student *dest, Student src);
/* ---------- Student record operations, Member B ---------- */
void addStudent(Node *head);
void deleteStudent(Node *head);
void deleteByNumMenu(Node *head);
void deleteFailedStudents(Node *head);
void deleteAllStudents(Node *head);
void searchStudent(Node *head);
void searchByNumMenu(Node *head);
void searchByNameMenu(Node *head);
void searchByFuzzyNameMenu(Node *head);
void searchByScoreRangeMenu(Node *head);
void searchFailedStudentsMenu(Node *head);
void searchExcellentStudentsMenu(Node *head);
void modifyStudent(Node *head);
void insertStudent(Node *head);
void displayAllStudents(Node *head);
void displayStudentDetail(Student stu);
void displaySimpleStudents(Node *head);
void printStudentHeader(void);
void printStudent(Student stu);
void printStudentDetail(Student stu);
/* ---------- Demonstration and help functions, Member A and B ---------- */
void initSampleData(Node *head);
void showProjectHelp(void);
void showMemberDivision(void);
void showFileFormatHelp(void);
/* ---------- File functions, Member C ---------- */
int loadFromFile(Node *head, const char *filename);
int saveToFile(Node *head, const char *filename);
int backupDataFile(const char *sourceName, const char *backupName);
int exportReport(Node *head, const char *filename);
int createEmptyDataFile(const char *filename);
int fileExists(const char *filename);
void fileMenu(Node *head);
/* ---------- Analysis functions, Member C ---------- */
void sortMenu(Node *head);
void statisticMenu(Node *head);
void sortStudents(Node *head, SortType type);
void displayTopAndBottom(Node *head);
void displaySubjectStatistics(Node *head);
void displayDistribution(Node *head);
void displayClassSummary(Node *head);
#endif
