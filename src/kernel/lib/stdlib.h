/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#pragma once

#include "string.h"

/**
 * itoa
 *   brief: Convert n to string and copy to s
 *   parameters:
 *     - n: integer to convert
 *     - s: output string
 */
void itoa(int n, char s[]);

/**
 * uitoa
 *   brief: Convert n to string and copy to s
 *   parameters:
 *     - n: unsigned integer to convert
 *     - s: output string
 */
void uitoa(unsigned n, char s[]);

/**
 * uitoh
 *   brief: Convert n to a string (in hex format) and copy to s
 *   parameters:
 *     - n: unsigned integer to convert
 *     - s: output string
 */
void uitoh(unsigned n, char s[]);
