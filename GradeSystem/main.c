/*
 * Member A responsibility:
 * Program entrance, main menu, second-level menus and whole process control.
 */
#include <locale.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "student.h"
static void initConsoleEncoding(void);
static void showMainMenu(void);
static void systemMenu(Node *head);
static void printWelcome(void);
static void printGoodbye(void);
static void initConsoleEncoding(void) {
    /*
     * Fix Chinese garbled text on Windows terminals used by CLion.
     * Source files are saved as UTF-8. On Windows, CMake compiles Chinese
     * string literals to GBK and the console is switched to code page 936.
     * On Linux/macOS, UTF-8 remains the normal encoding.
     */
#ifdef _WIN32
    setlocale(LC_ALL, ".936");
    SetConsoleOutputCP(936);
    SetConsoleCP(936);
#else
    setlocale(LC_ALL, "");
#endif
}
static void printWelcome(void) {
    printf("====================================\n");
    printf("学生成绩管理系统\n");
    printf("内存中使用链表，文件 data.txt 用于保存数据。\n");
    printf("====================================\n");
}
static void printGoodbye(void) {
    printf("====================================\n");
    printf("数据处理结束，链表内存已释放。\n");
    printf("感谢使用学生成绩管理系统。\n");
    printf("====================================\n");
}
static void showMainMenu(void) {
    printf("\n========== 学生成绩管理系统 ==========\n");
    printf("1. 添加学生记录\n");
    printf("2. 删除学生记录\n");
    printf("3. 查询学生记录\n");
    printf("4. 修改学生记录\n");
    printf("5. 插入学生记录\n");
    printf("6. 显示全部记录\n");
    printf("7. 成绩排序\n");
    printf("8. 成绩统计\n");
    printf("9. 文件管理\n");
    printf("10. 系统说明\n");
    printf("0. 退出系统\n");
    printf("====================================\n");
}
static void showSystemMenu(void) {
    printf("\n========== 系统说明 ==========\n");
    printf("1. 查看项目功能说明\n");
    printf("2. 查看三人分工说明\n");
    printf("3. 查看 data.txt 文件格式\n");
    printf("0. 返回主菜单\n");
    printf("==============================\n");
}
static void systemMenu(Node *head) {
    int choice;
    int running = 1;
    (void)head;
    /* The system menu is separated from the main menu to keep main() readable. */
    while (running) {
        showSystemMenu();
        choice = readInt("Choose: ", 0, 3);
        switch (choice) {
            case 1: showProjectHelp(); break;
            case 2: showMemberDivision(); break;
            case 3: showFileFormatHelp(); break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n"); break;
        }
    }
}
int main(void) {
    Node *head = createList();
    int choice;
    int running = 1;
    initConsoleEncoding();
    if (head == NULL) {
        printf("Memory allocation failed. Program cannot start.\n");
        return 1;
    }
    printWelcome();
    /* Initial demonstration data must come from data.txt. */
    printf("Loading initial student data from %s ...\n", DATA_FILE);
    loadFromFile(head, DATA_FILE);
    while (running) {
        showMainMenu();
        choice = readInt("Choose function: ", 0, 10);
        /* Main menu only controls flow; real work is done by other modules. */
        switch (choice) {
            case 1:
                addStudent(head);
                saveToFile(head, DATA_FILE);
                break;
            case 2:
                deleteStudent(head);
                saveToFile(head, DATA_FILE);
                break;
            case 3: searchStudent(head); break;
            case 4:
                modifyStudent(head);
                saveToFile(head, DATA_FILE);
                break;
            case 5:
                insertStudent(head);
                saveToFile(head, DATA_FILE);
                break;
            case 6: displayAllStudents(head); break;
            case 7:
                sortMenu(head);
                if (askYesNo("Save sorted result? (y/n): ")) {
                    saveToFile(head, DATA_FILE);
                }
                break;
            case 8: statisticMenu(head); break;
            case 9: fileMenu(head); break;
            case 10: systemMenu(head); break;
            case 0:
                if (askYesNo("Save before exit? (y/n): ")) {
                    saveToFile(head, DATA_FILE);
                }
                /* Free all linked-list nodes before the program exits. */
                freeList(head);
                printGoodbye();
                running = 0;
                break;
            default: printf("Invalid choice.\n"); break;
        }
    }
    return 0;
}
