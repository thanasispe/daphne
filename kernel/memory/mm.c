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
 *
 * Memory manager
 */

#include "mm.h"

/* mmap as given by grub */
static mmap_entry_t *mmap_begin;

/* mmap as sanity checked by the kernel */
static mmap_entry_t kmmap[256];
static size_t kmmap_size = 0;
static uint32_t mmap_length = 0;

/* RAM info */
static uint32_t total_ram = 0;
static size_t regions = 0;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

/* Dump mmap entry */
static void dump_entry(mmap_entry_t *entry)
{
	switch (entry->type) {
		case MEMORY_AVAILABLE:
			printk("base_addr=0x%ux, length=%uiB - available", entry->base_addr_low, entry->length_low);
			break;
		case MEMORY_RESERVED:
			printk("base_addr=0x%ux, length=%uiB - reserved", entry->base_addr_low, entry->length_low);
			break;
		case MEMORY_ACPI:
		case MEMORY_NVS:
			printk("base_addr=0x%ux, length=%uiB - acpi", entry->base_addr_low, entry->length_low);
			break;
		case MEMORY_BADRAM:
		case MEMORY_INVALID:
			printk("base_addr=0x%ux, length=%uiB - bad", entry->base_addr_low, entry->length_low);
			break;
	}
}

void mm_init(mmap_entry_t *mmap_addr, uint32_t length)
{
	mmap_entry_t *mmap = mmap_addr;
	mmap_begin = mmap_addr;
	mmap_length = length;
	/* Validate mmap */
	for (size_t i = 0; mmap < (mmap_addr + length); i++) {
		/* 0 length entries */
		if (mmap->length_low == 0x0) {
			mmap->type = MEMORY_INVALID;
		}

		/* Entry overlaps with grub/kernel code */
		if (mmap->base_addr_low + mmap->length_low <= KERN_END) {
			mmap->type = MEMORY_INVALID;
		} else if ((mmap->base_addr_low <= KERN_END) && mmap->type == MEMORY_AVAILABLE) {
			/* If only a part of it does, keep the part that doesn't */
			mmap->base_addr_low += KERN_END + 1;
			mmap->length_low -= KERN_END + 1;
		}

		/* Overlapping entries */
		for (size_t j = 0; i < kmmap_size; j++) {
			/* Entry overlaps with another */
			if (!(mmap->base_addr_low < kmmap[j].base_addr_low) && 
				(mmap->base_addr_low < (kmmap[j].base_addr_low + kmmap[j].length_low))) {
				mmap->type = MEMORY_INVALID;
			}
		}

		/* Append entry to kmmap */
		if (mmap->type != MEMORY_INVALID) {
			if (mmap->type == MEMORY_AVAILABLE) {
				total_ram += mmap->length_low;
				regions++;
			}
			kmmap[kmmap_size] = *mmap;
			kmmap_size++;
		}

		dump_entry(mmap);
		/* Next entry */
		mmap = (mmap_entry_t *) ((uint32_t) mmap + mmap->size + sizeof(mmap->size));
	}
}

mmap_entry_t *mm_get_grub_mmap()
{
	return mmap_begin;
}

mmap_entry_t *mm_get_kernel_mmap()
{
	return kmmap;
}

size_t mm_get_kmmap_size()
{
	return kmmap_size;
}
