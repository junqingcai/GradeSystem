/*
学生成绩管理系统
课程设计源文件

三人项目分工：

成员 A：
main.c
负责主函数、主菜单、二级菜单、帮助页面、
成员分工展示以及整个程序流程控制。

成员 B：
student.c
负责链表创建、添加、删除、查找、修改、
插入、显示、输入合法性检查以及通用链表工具函数。

成员 C：
file.c 和 analysis.c
负责数据文件读取、保存、备份、导出、成绩排序、
成绩统计以及分数分布分析。

数据存储：
系统在内存中使用单向链表存储数据。
系统使用 data.txt 作为持久化数据文件。

data.txt 中的数据格式：
学号 学生姓名 C语言成绩 数学成绩 英语成绩

示例：
2024001 ZhangSan 88 92 80
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
