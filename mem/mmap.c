#include "mmap.h"

#include <man.h>

// Installs a MemMmapBlk at a location
MemMmapBlk *mem_mmap_install(void *ptr, MemFramer *framer) {
	// XXX assumption that ptr is aligned?

	MemMmapBlk *blk = ptr;
	memset(blk, 0, sizeof(MemMmapBlk));

	blk->framer = framer;

	return blk;
}

// Allocates a new entry
MemMmapEntry *mem_mmap_alloc_entry(MemMmapBlk *blk) {
	// Try to find a free entry in the current mmap block
	for (size_t i = 0; i < 127; i++)
		if (blk->entries[i].type == 0)
			return &blk->entries[i];

	// If not, ask the next mmap block for a free entry
	MemMmapBlk *nextblk = (MemMmapBlk*)blk->nextblk;
	if (nextblk)
		return mem_mmap_alloc_entry(nextblk);
	else {
		// If no mmap block, allocate a new one
		void *frame = mem_framer_alloc(blk->framer, 1);
		if (frame == nil)
			return nil;

		nextblk = mem_mmap_install(frame, blk->framer);
		if (nextblk == nil)
			return nil;
		
		// Set new mmap block as the next one
		blk->nextblk = (u64)nextblk;

		// Ask mmap block for a free entry
		return mem_mmap_alloc_entry(nextblk);
	}
}

// Frees a mmap entry and frees the associated page if the mmap block
// is empty
void mem_mmap_free_entry(MemMmapBlk *blk, MemMmapEntry *entry) {
	// Mark the entry as free
	entry->type = MEM_MMAP_FREE;

	// Find whether the owning blk is empty
	MemMmapBlk *ownerblk = (MemMmapBlk*)((u64)entry - ((u64)entry % 4096));
	for (size_t i = 0; i < 127; i++)
		if (ownerblk->entries[i].type)
			return;

	// Here, the ownerblk is empty

	// Find the previous block
	MemMmapBlk *cur = blk;
	while (cur) {
		if (cur->nextblk == (u64)ownerblk)
			break;
		cur = (MemMmapBlk*)cur->nextblk;	
	}

	if (cur == nil)
		return;
	
	// Set the next block of the previous block
	cur->nextblk = ownerblk->nextblk;

	// Free the ownerblk
	mem_framer_free(ownerblk->framer, ownerblk);
}

// Finds 
u64 mem_mmap_find_location(MemMmapBlk *blk, u64 begin) {
	if (blk->first == 0)
		return 0;
	
	MemMmapEntry *prev = nil;
	MemMmapEntry *next = (MemMmapEntry*)blk->first;
	while (next && begin > next->begin) {
		prev = next;
		next = (MemMmapEntry*)next->next;
	}

	return (u64)prev;
}

char mem_mmap_fix_overlap(MemMmapBlk *blk) {
	if (blk->first == 0)
		return 0;

	MemMmapEntry *entry = (MemMmapEntry*)blk->first;
	if (entry->next == 0)
		return 0;
	
	MemMmapEntry *next = (MemMmapEntry*)entry->next;
	while (entry && next) {
		// Check if we are in an overlap situation
		if (entry->end >= next->end)
			break;

		entry = next;
		next = (MemMmapEntry*)next->next;
	}

	// If no overlap, exit
	if (next == nil)
		return 0;

	// Here, we are in a situation such that entry overlaps
	// on next

	// XXX check the validity of nonstrict inequality
	if (entry->type >= next->type) {
		entry->next = next->next;

		mem_mmap_free_entry(blk, next);
		return 1;
	}

	MemMmapEntry *contd = mem_mmap_alloc_entry(blk);
	memcpy(contd, entry, sizeof(MemMmapEntry));
	entry->end = next->begin;
	contd->begin = next->end;

	contd->next = next->next;
	next->next = (u64)contd;

	return 1;
}

char mem_mmap_fix_length(MemMmapBlk *blk) {
	if (blk->first == 0)
		return 0;
	
	MemMmapEntry *entry = (MemMmapEntry*)blk->first;
	if (entry->next == 0)
		return 0;
	
	MemMmapEntry *next = (MemMmapEntry*)entry->next;
	while (entry && next) {
		if (next->begin < entry->end)
			break;

		entry = next;
		next = (MemMmapEntry*)next->next;
	}

	if (next == nil)
		return 0;

	if (entry->type >= next->type)
		next->begin = entry->end;
	else
		entry->end = next->begin;
	
	return 1;
}

char mem_mmap_collate(MemMmapBlk *blk) {
	if (blk->first == 0)
		return 0;
	
	MemMmapEntry *entry = (MemMmapEntry*)blk->first;
	if (entry->next == 0)
		return 0;
	
	MemMmapEntry *next = (MemMmapEntry*)entry->next;
	while (entry && next) {
		if (entry->type == next->type && entry->data == next->data && entry->end == next->begin)
			break;
		
		entry = next;
		next = (MemMmapEntry*)next->next;
	}

	if (next == nil)
		return 0;

	entry->end = next->end;
	entry->next = next->next;

	mem_mmap_free_entry(blk, next);

	return 1;
}

char mem_mmap_collect_zero(MemMmapBlk *blk) {
	if (blk->first == 0)
		return 0;
	
	MemMmapEntry *entry = (MemMmapEntry*)blk->first;
	if (entry->begin == entry->end || entry->type == MEM_MMAP_UNMARK) {
		blk->first = entry->next;
		mem_mmap_free_entry(blk, entry);
		return 1;
	}

	MemMmapEntry *next = (MemMmapEntry*)entry->next;
	while (entry && next) {
		if (next->begin == next->end || next->type == MEM_MMAP_UNMARK)
			break;

		entry = next;
		next = (MemMmapEntry*)next->next;
	}

	if (next == nil)
		return 0;

	entry->next = next->next;
	mem_mmap_free_entry(blk, next);
	return 1;
}

void mem_mmap_mark(MemMmapBlk *blk, u64 begin, u64 end, u32 type, u32 data) {
	// Allocate a new entry
	MemMmapEntry *entry = mem_mmap_alloc_entry(blk);

	// Place it at its according location in the linked list
	u64 location = mem_mmap_find_location(blk, begin);
	if (location == 0) {
		entry->next = blk->first;
		blk->first = (u64)entry;
	} else {
		MemMmapEntry *prev = (MemMmapEntry*)location;
		entry->next = prev->next;
		prev->next = (u64)entry;
	}

	entry->begin = begin;
	entry->end = end;
	entry->type = type;
	entry->data = data;


	// Iteratively clean the map
	while (mem_mmap_fix_overlap(blk));
	while (mem_mmap_fix_length(blk));
	while (mem_mmap_collate(blk));
	while (mem_mmap_collect_zero(blk));
}