/**
 * @file main.c
 * @author BeNest
 * @brief メインファイル
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @mainpage 
 * @section 概要
 * 本プログラムは２つの日付の差を計算するプログラムである
 * 
 * @subsection 実行方法
 * 
 * `$ datediff`
 * 
 * @section 動作・機能
 * 
 * @subsection 処理フロー
 * 
 *

  @startuml
  skinparam conditionStyle diamond
  start

  :days = 0;

  while (date1 == date2?) is (=)
    :date1 = date1 + 1day;
    :days = days + 1;
  endwhile (<>)

  stop
  @enduml

 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/datediff.h"

bool inputdate(bool start, int *year, int *month, int *day) {
    char *str = start ? "開始" : "終了";
    char buff[LINE_MAX];

    printf("%s年を入力して下さい:", str);
    scanf("%s", buff);
    *year = atoi(buff);

    printf("%s月を入力して下さい:", str);
    scanf("%s", buff);
    *month = atoi(buff);

    printf("%s日を入力して下さい:", str);
    scanf("%s", buff);
    *day = atoi(buff);

    return checkdate(*year, *month, *day);
}

void inputdates(int *year1, int *month1, int *day1, int *year2, int *month2, int *day2) {
    for(;; printf("不正な日付です\n")) {
        for(; !inputdate(true, year1, month1, day1); printf("不正な日付です\n")) {
            ;
        }
        for(; !inputdate(false, year2, month2, day2); printf("不正な日付です\n")) {
            ;
        }
        if(compdate(*year1, *month1, *day1, *year2, *month2, *day2) <= 0) {
            break;
        }
    }
}

void displaydiff(int diffdays) {
    printf("経過日数は%d日です\n", diffdays);
}

int main(int argc, char *argv[]) {
    int y1, m1, d1;
    int y2, m2, d2;

    // input
    inputdates(&y1, &m1, &d1, &y2, &m2, &d2);

    // processing
    int diffdays = datediff(y1, m1, d1, y2, m2, d2);

    // output
    displaydiff(diffdays);

    return 0;
}