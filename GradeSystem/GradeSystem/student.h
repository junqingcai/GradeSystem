/*
学生成绩管理系统
课程设计源文件

三人项目分工：

成员 A：
main.c
负责主函数、主菜单、二级菜单、帮助页面、
成员分工展示以及整个程序流程控制。

成员 B：
student.c和student.h
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
// 基本长度限制，额外保留一个字节用于存放 '\0'
#define NUM_LEN 20
#define NAME_LEN 20
#define LINE_LEN 256
// 文件管理模块所使用的文件名
#define DATA_FILE "data.txt"
#define BACKUP_FILE "backup.txt"
#define EXPORT_FILE "export.txt"
// 查询和统计功能所使用的等级标准
#define PASS_SCORE 60.0f
#define EXCELLENT_SCORE 90.0f
// 当一个函数需要处理多个分数时，会使用科目及其他数据对象的编号
#define SUBJECT_C 1
#define SUBJECT_MATH 2
#define SUBJECT_ENGLISH 3
#define SUBJECT_TOTAL 4
#define SUBJECT_AVERAGE 5
// 一个学生完整的分数信息
typedef struct Student {
    char num[NUM_LEN];
    char name[NAME_LEN];
    float cScore;
    float mathScore;
    float englishScore;
    float total;
    float average;
} Student;
//定义单向列表用于存储多个学生的数据
typedef struct Node {
    Student data;
    struct Node *next;
} Node;
//analysis.c 所使用的排序选项
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
/* 一个科目的统计数据。 ranges[0，1，2，3，4] 分别表示
 90+, 80+, 70+, 60+, 低于 60  */
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
// 通用的输入辅助函数，用于所有模块
void clearInputBuffer(void);
void pauseScreen(void);
int askYesNo(const char *prompt);
int readInt(const char *prompt, int minValue, int maxValue);
float readFloat(const char *prompt, float minValue, float maxValue);
float readScore(const char *prompt);
void readString(const char *prompt, char *buffer, int size);
// 链表基本工具  Member B
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
//学生数据处理工具 Member B
void calculateScore(Student *stu);
int isScoreValid(float score);
int isNumFormatValid(const char *num);
int isNameFormatValid(const char *name);
float getStudentScore(Student stu, int subject);
const char *getSubjectName(int subject);
Student inputStudent(Node *head, int checkDuplicate);
void inputScores(Student *stu);
void copyStudent(Student *dest, Student src);
//学生数据记录操作  Member B
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
//演示与帮助功能, Member A & B
void initSampleData(Node *head);
void showProjectHelp(void);
void showMemberDivision(void);
void showFileFormatHelp(void);
//文件功能 Member C
int loadFromFile(Node *head, const char *filename);
int saveToFile(Node *head, const char *filename);
int backupDataFile(const char *sourceName, const char *backupName);
int exportReport(Node *head, const char *filename);
int createEmptyDataFile(const char *filename);
int fileExists(const char *filename);
void fileMenu(Node *head);
//数据分析功能 Member C
void sortMenu(Node *head);
void statisticMenu(Node *head);
void sortStudents(Node *head, SortType type);
void displayTopAndBottom(Node *head);
void displaySubjectStatistics(Node *head);
void displayDistribution(Node *head);
void displayClassSummary(Node *head);
#endif
