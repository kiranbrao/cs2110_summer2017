#include "my_malloc.h"

/* You *MUST* use this macro when calling my_sbrk to allocate the
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* Please use this value as your canary! */
#define CANARY 0x2110CAFE

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you may receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif

/* Our freelist structure - This is where the current freelist of
 * blocks will be maintained. Failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 *
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist;

void* my_malloc(size_t size)
{
  /* YOUR CODE HERE! */
	/*determine the actual size of the requested space plus 
	the metadata for the requested space plus 2 canaries */
	size_t trueSize =  size + 2*sizeof(CANARY) + sizeof(metadata_t);
	if (trueSize > SBRK_SIZE) {
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	if (freelist == NULL) {
		void* newsbrk = my_sbrk(SBRK_SIZE);
		if (newsbrk == NULL) {
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		//Create metadata for newly-generated block
		metadata_t newBlockMD = {SBRK_SIZE, 0, NULL, NULL};
		//Set the starting address of the block equal to the metadata (does not occupy all addresses in the block)
		*((metadata_t*) newsbrk) = newBlockMD;
		//Add address of metadata (also starting address of block) to freelist
		freelist = ((metadata_t*) newsbrk);
	}
	//Find smallest block that is large enough to hold user data -> COULD TURN THIS INTO HELPER
	metadata_t* current = freelist;
	metadata_t* smallest = NULL;
	metadata_t* prev = NULL;
	while(current) {
		if (current->block_size > trueSize) {
			if (!smallest || current->block_size < smallest->block_size){
				smallest = current;
			}
		}
		prev = current;
		current = current->next;
	}

	//If there is no free block that is large enough to hold the requested data size, request another block
	if(!smallest) {
		void* newsbrk = my_sbrk(SBRK_SIZE);
		if (newsbrk == NULL) {
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		//Create metadata for newly-generated block. This block will contain the user data.
		metadata_t newBlockMD = {trueSize, size, NULL, NULL};
		//Set the starting address of the block equal to the metadata 
		*((metadata_t*) newsbrk) = newBlockMD;
		//Determine if there is enough space to fit another metadata structure and at least 1 byte of data in the block
		if ((trueSize + sizeof(metadata_t) + 2*sizeof(CANARY) + 1) < SBRK_SIZE) {
			metadata_t restMD = {SBRK_SIZE - trueSize, 0, NULL, prev};
			/*If trueSize = x, we want to move the pointer by x bytes, 
			so we cast the pointer that has the starting address of the new block from a void* to a char*
			The address is then cast into a metadata pointer and dereferenced to store the restMD metadata structure*/
			*((metadata_t*) ((char*) newsbrk + trueSize)) = restMD;
			//Add the new block to the end of the freelist
			prev->next = ((metadata_t*) ((char*) newsbrk + trueSize));
		} else {
			/*If we can't fit another metadata structure and at least 1 byte of data in the block,
			we change the block size of the initial metadata to take the entire block */
			newBlockMD.block_size = SBRK_SIZE;
		}
		//Write metadata and user data canaries
		/*Starting block address is cast into char* so that if metadata_t size is x, the pointer is only incremented 
		by x using pointer arithmetic (versus 4x if cast into an int*). The pointer is then cast into an int* 
		and dereferenced to store the CANARY, which is an int value. */
		*((int*)((char*) newsbrk + sizeof(metadata_t))) = CANARY;
		*((int*)((char*) newsbrk + trueSize - sizeof(CANARY))) = CANARY;

		ERRNO = NO_ERROR;
		return (void*)((char*) newsbrk + sizeof(metadata_t) + sizeof(CANARY));

	} else {
		smallest->request_size = size;
		metadata_t* prev = smallest->prev;
		metadata_t* next = smallest->next;
		if ((trueSize + sizeof(metadata_t) + 2*sizeof(CANARY) + 1) < smallest->block_size) {
			metadata_t restMD = {smallest->block_size - trueSize, 0, next, prev};
			smallest->block_size = trueSize;
			/*If trueSize = x, we want to move the pointer by x bytes, 
			so we cast the pointer that has the starting address of the new block from a void* to a char*
			The address is then cast into a metadata pointer and dereferenced to store the restMD metadata structure*/
			*((metadata_t*) ((char*) smallest + trueSize)) = restMD;
			if (prev) {
				//Connect previous block in freelist new block
				prev->next = ((metadata_t*) ((char*) smallest + trueSize));
			}
			if (next) {
				//Link next block in freelist back to new block
				next->prev = ((metadata_t*) ((char*) smallest + trueSize));
			}
			if (freelist == smallest) {
				/*Set freelist head equal to the new block 
				if smallest was the only block in freelist or the head of freelist*/
				freelist = ((metadata_t*) ((char*) smallest + trueSize));
			}
		} else {
			if (prev) {
				prev->next = next;
			}
			if (next) {
				next->prev = prev;
			}
			if (freelist == smallest) {
				//If smallest was the head of freelist, make the new head the next free block
				freelist = next;
			}
		}
		//Write metadata and user data canaries
		/*Starting block address is cast into char* so that if metadata_t size is x, the pointer is only incremented 
		by x using pointer arithmetic (versus 4x if cast into an int*). The pointer is then cast into an int* 
		and dereferenced to store the CANARY, which is an int value. */
		*((int*)((char*) smallest + sizeof(metadata_t))) = CANARY;
		*((int*)((char*) smallest + trueSize - sizeof(CANARY))) = CANARY;

		ERRNO = NO_ERROR;
		return (void*)((char*) smallest + sizeof(metadata_t) + sizeof(CANARY));
	}

	return NULL;
}

void my_free(void* ptr)
{
  /* YOUR CODE HERE! */
	metadata_t* trueMD = (metadata_t*)((char*) ptr - sizeof(metadata_t) - sizeof(CANARY)); //address of metadata
	int canary1 = *((int*) ((char*) trueMD + sizeof(metadata_t))); //dereferenced first canary
	//dereferenced second canary
	int	canary2 = *((int*) ((char*) trueMD + sizeof(metadata_t) + sizeof(CANARY) + trueMD->request_size)); 
	if (canary1 != CANARY || canary2 != CANARY) {
		//check to see if either canary has been changed/corrupted
		ERRNO = CANARY_CORRUPTED;
		return; 
	}

	metadata_t* current = freelist;
	//get the address of the metadata directly to the right of the block we are freeing
	metadata_t* rightBlock = (metadata_t*) ((char*) trueMD + trueMD->block_size); 
	metadata_t* prev = NULL;
	char mergedLeft = 0;
	char mergedRight = 0;

	/*iterate through freelist entirely or until the block we are freeing 
	has merged both with its left and right neighbors*/
	while (current && !(mergedLeft && mergedRight)) {
		if (((metadata_t*) ((char*) current + current->block_size) == trueMD)) {
			/*If the current metadata pointer in the freelist 
			is the block directly to the left of the block we are freeing*/
			if (((metadata_t*) ((char*) current + current->block_size) == freelist)) {
				/*If the block we are freeing has already merged right and is the head of freelist,
				we want to make the left neighbor the head of the freelist*/
				freelist = current;
				freelist->prev->next = NULL;
				freelist->prev = NULL;
			}
			mergedLeft = 1;
			current->block_size += trueMD->block_size; //add freed block block size to left neighbor
		} else if (current == rightBlock) {
			/*If the current metadata pointer in the freelist
			is the block directly to the right of the block we are freeing*/
			if (!mergedLeft) {
				//If the block we are freeing has not merged with left neighbor
				trueMD->block_size += current->block_size; //add right block
				trueMD->request_size = 0; //set request size to 0 because block is free
				trueMD->prev = current->prev;
				if (trueMD->prev) {
					trueMD->prev->next = trueMD;
				}
				trueMD->next = current->next;
				if (trueMD->next) {
					trueMD->next->prev = trueMD;
				}
				if (freelist == current) {
					freelist = trueMD;
				}
			} else {
				//If the block we are freeing has merged with left neighbor
				metadata_t* trueStart = current->prev; //start of the merged block
				trueStart->block_size += current->block_size; //add right block 
				trueStart->next = current->next;
				if (trueStart->next) {
					trueStart->next->prev = trueStart;
				}
			}
			mergedRight = 1;
		} 
		prev = current;
		current = current->next;
	}

	if (!mergedLeft && !mergedRight) {
		/*If the block that we are freeing did not merge with its left neighbor
		or right neighbor, just add it to the freelist */
		trueMD->request_size = 0;
		trueMD->next = NULL;
		if (prev) {
			prev->next = trueMD;
		} else {
			freelist = trueMD;
		}
		trueMD->prev = prev;
	}
}

/* MAYBE ADD SOME HELPER FUNCTIONS HERE? */

