#ifndef DATE_DIFF_H_
#define DATE_DIFF_H_

/**
 * @file datediff.h
 * @author BeNest
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>

/**
 * @brief うるう年判定
 * 
 * @param year 判定したい年
 * 
 * @return true：うるう年
 * @return false：うるう年以外
 */
bool checkleap(int year);

/**
 * @brief 日付判定
 * 
 * @param year 年
 * @param month 月
 * @param day 日
 * @return true：有効
 * @return false：無効
 */
bool checkdate(int year, int month, int day);

/**
 * @brief 日付比較
 * 
 * @param year1 年1
 * @param month1 月1
 * @param day1 日1
 * @param year2 年1
 * @param month2 月1
 * @param day2 日1
 * @return -1：年月日1 < 年月日2
 * @return 0: 年月日1 == 年月日2
 * @return 1: 年月日1 > 年月日2
 */
int compdate(int year1, int month1, int day1, int year2, int month2, int day2);

/**
 * @brief 日付を1日進める
 * 
 * @param year 年
 * @param month 月
 * @param day 日
 */
void incdate(int *year, int *month, int *day);

/**
 * @brief 日付の差分計算
 * 
 * @param year1 年1
 * @param month1 月1
 * @param day1 日1
 * @param year2 年1
 * @param month2 月1
 * @param day2 日1
 * @return int 差分[日]
 */
int datediff(int year1, int month1, int day1, int year2, int month2, int day2);

#endif
