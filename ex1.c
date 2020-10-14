#include <stdio.h>
#include <stdlib.h>

typedef struct page{
    int id;
    int counter;
} Page;

Page* findPage(Page* pages, int id, int size){
    for (int i = 0; i < size; i++){
        if (pages[i].id == id){
            return pages + i;
        }
    }
    return NULL;
}

Page* findFreeSpace(Page* pages, int size){
    int minCounter = 1<<31;
    int id = 0;

    for (int i = 0; i < size; i++){
        if (pages[i].id == 0){
            return pages + i;
        }else if(pages[i].counter < minCounter){
            minCounter = pages[i].counter;
            id = i;
        }
    }
    return pages + id;
    
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage: [./program_name] [num_of_frames][input_file_name]\n");
        return 1;
    }

    int pageCount = strtol(argv[1], NULL, 10);

    Page* pages = calloc(pageCount, sizeof(Page));
    if (!pages){
        printf("Failed to create pages");
        return 1;
    }

    FILE* file = fopen(argv[2], "r");
    if (!file){
        printf("Cannot read file");
        return 1;
    }

    int hits = 0;
    int misses = 0;
    while (1){
        int pageId;
        if (fscanf(file, "%d", &pageId) != 1){
            break;
        }

        Page* page = findPage(pages, pageId, pageCount);
        if (page){
            hits++;
        } else{
            misses++;
            page = findFreeSpace(pages, pageCount);
            page->counter = 0;
            page->id = pageId;
        }

        for (int i = 0; i < pageCount; i++)
        {
            pages[i].counter /= 2;
        }
        
        page->counter |= 1 << 31;
    }

    printf("Hits: %d\n", hits);
    printf("Misses: %d\n", misses);
    printf("Hit/Miss Ratio: %f\n", (double)hits/misses);

    free(pages);


    fclose(file);

}
