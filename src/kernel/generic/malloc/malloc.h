/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <generic/forbia/kernel.h>
#include <generic/forbia/panic.h>
#include <generic/forbia/lock.h>

#include <generic/memory/mem.h>

#include <lib/string.h>

#include "mmu.h"

#define MAX_PAGES 12 /* Max pages in bin */

#define fast_ceil(x, y) ((long long) x + y - 1) / y
#define kmem_align(n) 32 * fast_ceil(n, 32)
#define kmem_page_align(n) PAGE_SIZE * fast_ceil(n, PAGE_SIZE)

#define PAGE_SIZE 4096

struct malloc_page {
    struct malloc_page *next_page; /* NULL if last page */
    uint8_t free;
    uint64_t *base;
} __attribute__((packed));

typedef struct malloc_page malloc_page_t;

struct malloc_bin {
    struct malloc_bin *next_bin; /* NULL if last node */
    struct malloc_page *first_page;
    size_t page_size; /* This bin's page size */
    size_t pages; /* Number of pages in bin */
} __attribute__((packed));

typedef struct malloc_bin malloc_bin_t;

struct malloc_ptr {
    uint64_t size;
    uint64_t *base;
} __attribute__((packed));

typedef struct malloc_ptr malloc_ptr_t;

#define malloc_get_info(ptr) (malloc_ptr_t *) (((uint64_t *) ptr) - 1)

/**
 * kmalloc
 *   brief: attempt to allocate memory of size n
 */
void *kmalloc(size_t n);

/**
 * krealloc
 *   brief: reallocate pointer 
 *          (UNSAFE: does not check if ptr.size - size < 0)
 */
void *krealloc(void *ptr, size_t size);

/**
 * kfree
 *   brief: attempt to free memory
 *          allocated by kmalloc
 */
void *kfree(void *ptr);

/**
 * kalloc_mem_aligned
 *   brief: watermark allocator
 */
void *kalloc_mem_aligned(size_t n);

/**
 * kalloc_mem_page_aligned
 *   brief: allocate page
 */
void *kalloc_mem_page_aligned(size_t n);
