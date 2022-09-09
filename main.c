#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory Inefficient Struct (Multiple of 8 Bytes Would Be Ideal)
typedef struct word
{
    int size;          // 4 Bytes
    char memo[5];      // 5 Bytes - Extra Byte Needed for '\0'
    struct word* next; // 8 Bytes (Unnecessary Implementation)
} Word;

// Initialize Dictionary
Word list[100]; // 24*100 Bytes
Word* orig;     // 8 Bytes
Word* curr;     // 8 Bytes
Word* last;     // 8 Bytes
Word* end1;     // 8 Bytes
Word* end2;     // 8 Bytes
Word* temp;     // 8 Bytes
int spawns = 0; // 4 Bytes

Word* createWord(int size, char memo[5])
{
    if (spawns < 100)
    {
        // Assign Size
        list[spawns].size = size;
        
        // Assign Memo
        strcpy(list[spawns].memo, memo);
        list[spawns].memo[size] = '\0';
        
        // Assign Next
        if (spawns != 0)
        {
            list[spawns - 1].next = &list[spawns];
        }
        else
        {
            orig = &list[0];
        }
        
        // Update Address
        last = &list[spawns];
        
        // Update Counter
        spawns++;
        
        // Return Success
        return &list[spawns - 1];
    }
    else
    {
        // Return Failure
        return NULL;
    }
}

// Gets Nth Element of Linked List
Word* openBook(int index)
{
    curr = orig;
    
    for (int i = 0; i < index; i++)
    {
        if (curr -> next != NULL)
        {
            curr = curr -> next;
        }
    }
    
    return curr;
}

int main(void)
{
    // Input Storage Variable
    char line[256]; // 256 Bytes (16x16)
    char copy[256]; // 256 Bytes (16x16)
    char left[256]; // 256 Bytes (16x16)
    
    // Request & Store Input
    printf("\nEnter Sentence (Maximum of 256 Characters):\n");
    scanf("%256[^\n]s", line);
    
    // Temporary Placeholders
    int mark = 0; // Word Start Point
    char hold[5]; // Temp Word Holder
    
    // Word Addition Algorithm
    for (int i = 0; i < strlen(line); i++)
    {
        if (isalpha(line[i]) == 0 || i == strlen(line) - 1)
        {
            if (mark != i)
            {
                if (isalpha(line[i]) == 0)
                {
                    // String Manipulation
                    strcpy(copy, line);
                    strncpy(hold, copy + mark, 4);
                    
                    if (i - mark < 4)
                    {
                        hold[i - mark] = '\0';
                    }
                    else
                    {
                        hold[4] = '\0';
                    }
                    
                    // Function Call
                    createWord(i - mark, hold);
                }
                else
                {
                    // String Manipulation
                    strcpy(copy, line);
                    strncpy(hold, copy + mark, 4);
                    
                    if (i - mark + 1 < 4)
                    {
                        hold[i - mark + 1] = '\0';
                    }
                    else
                    {
                        hold[4] = '\0';
                    }
                    
                    // Function Call
                    createWord(i - mark + 1, hold);
                }
            }
            else
            {
                if (i == strlen(line) - 1 && isalpha(line[i]) != 0)
                {
                    // String Manipulation
                    strcpy(copy, line);
                    strncpy(hold, copy + mark, 1);
                    hold[1] = '\0';
                    
                    // Function Call
                    createWord(1, hold);
                }
            }
            
            // Update Marker
            mark = i + 1;
        }
    }
    
    if (orig == NULL)
    {
        orig = &list[0];
    }
    
    // List Sorting Algorithm (Selection Sort) - Consider Heap Sort For Large Lists
    for (int i = 0; i < spawns; i++)
    {
        // Place: Smallest Element Index (Unsorted Section)
        int place = i;
        
        end1 = openBook(i);
        temp = end1;
        
        // Find Index & Address of Smallest Unsorted Element
        for (int j = i + 1; j < spawns; j++)
        {
            end2 = openBook(j);
            
            if (strcasecmp(temp -> memo, end2 -> memo) > 0)
            {
                temp = end2;
                place = j;
            }
        }
        
        end2 = openBook(place);
        
        // Switch Indicies & Addresses of Unsorted Elements
        if (place != i)
        {
            if (end1 != orig)
            {
                // Store Temp Element
                temp = end2 -> next;
                
                // Reassign Pointers
                openBook(i - 1) -> next = end2;
                end2 -> next = end1 -> next;
                openBook(place - 1) -> next = end1;
                end1 -> next = temp;
            }
            else
            {
                // Store Temp Element
                temp = end2 -> next;
                
                // Reassign Pointers
                orig = end2;
                end2 -> next = end1 -> next;
                openBook(place - 1) -> next = end1;
                end1 -> next = temp;
            }
        }
    }
    
    // Input Loop
    while (1)
    {
        // Query Command
        printf("\nWould you like to add (A), search (S), delete (D), or close (C)?\n");
        getchar();
        scanf("%256[^\n]s", left);
        
        if (left[0] == 'a' || left[0] == 'A')
        {
            // Word Addition
            printf("\nInput Word (Addition):\n");
            getchar();
            scanf("%256[^\n]s", line);
            
            last = openBook(spawns);
            mark = 0;
            
            if (spawns < 100)
            {
                for (int i = 0; i < strlen(line); i++)
                {
                    if (isalpha(line[i]) == 0 || i == strlen(line) - 1)
                    {
                        if (mark != i || (i == strlen(line) - 1 && isalpha(line[i]) != 0))
                        {
                            // Adds Word
                            if (isalpha(line[i]) == 0)
                            {
                                // String Manipulation
                                strcpy(copy, line);
                                strncpy(hold, copy + mark, 4);
                                
                                if (i - mark < 4)
                                {
                                    hold[i - mark] = '\0';
                                }
                                else
                                {
                                    hold[4] = '\0';
                                }
                                
                                // Impromptu Assignment
                                strcpy(list[spawns].memo, hold);
                                list[spawns].size = i - mark;
                                last -> next = &list[spawns];
                                last = &list[spawns];
                                
                                // Update Counter
                                spawns++;
                            }
                            else if (mark == i && i == strlen(line) - 1)
                            {
                                // String Manipulation
                                strcpy(copy, line);
                                strncpy(hold, copy + mark, 1);
                                hold[1] = '\0';
                                
                                // Impromptu Assignment
                                strcpy(list[spawns].memo, hold);
                                list[spawns].size = 1;
                                last -> next = &list[spawns];
                                last = &list[spawns];
                                
                                // Update Counter
                                spawns++;
                            }
                            else
                            {
                                // String Manipulation
                                strcpy(copy, line);
                                strncpy(hold, copy + mark, 4);
                                
                                if (i - mark + 1 < 4)
                                {
                                    hold[i - mark + 1] = '\0';
                                }
                                else
                                {
                                    hold[4] = '\0';
                                }
                                
                                // Impromptu Assignment
                                strcpy(list[spawns].memo, hold);
                                list[spawns].size = i - mark + 1;
                                last -> next = &list[spawns];
                                last = &list[spawns];
                                
                                // Update Counter
                                spawns++;
                            }
                            
                            // Prepare Sort Algo.
                            end1 = orig;
                            end2 = last;
                            int pure = 1;
                            
                            // Sorts Word
                            for (int j = 0; j < spawns; j++)
                            {
                                end1 = openBook(j);
                                if (strcasecmp(end1 -> memo, end2 -> memo) > 0)
                                {
                                    if (end1 != end2)
                                    {
                                        if (end1 != orig)
                                        {
                                            openBook(j - 1) -> next = end2;
                                            end2 -> next = end1;
                                            openBook(spawns - 1) -> next = NULL;
                                            break;
                                        }
                                        else
                                        {
                                            orig = end2;
                                            end2 -> next = end1;
                                            openBook(spawns - 1) -> next = NULL;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        else
                        {
                            mark++;
                        }
                    }
                }
            }
        }
        else if (left[0] == 's' || left[0] == 'S')
        {
            // Initialize Settings
            int sense = -1;
            
            // Sensitivity Queries
            while (1)
            {
                printf("\nCase Sensitive Search? (y/n)\n");
                getchar();
                scanf("%256[^\n]s", line);
                
                if (line[0] == 'y' || line[0] == 'Y')
                {
                    sense = 1;
                    break;
                }
                else if (line[0] == 'n' || line[0] == 'N')
                {
                    sense = 0;
                    break;
                }
                else
                {
                    printf("\nInvalid Input. Try Again!\n");
                }
            }
            
            // Get Search Keyword
            printf("\nInput Word (Search):\n");
            getchar();
            scanf("%256[^\n]s", left);
            
            // Find Word Beg
            int beg = 0;
            for (int i = 0; i < strlen(left); i++)
            {
                if (isalpha(left[i]) == 0)
                {
                    beg = i + 1;
                }
                else
                {
                    break;
                }
            }
            
            // Find Word End
            int end = beg;
            for (int i = beg; i < strlen(left); i++)
            {
                if (isalpha(left[i]) != 0)
                {
                    end = i + 1;
                }
                else
                {
                    break;
                }
            }
            
            // String Manipulation
            strcpy(line, "");
            if (end - beg < 4)
            {
                strncpy(line, left + beg, end - beg);
                line[end - beg] = '\0';
            }
            else
            {
                strncpy(line, left + beg, 4);
                line[4] = '\0';
            }
            
            // Word Search Results
            mark = 0;
            printf("\nResults:\n");
            
            for (int i = 0; i < spawns; i++)
            {
                temp = openBook(i);
                
                for (int j = 0; j < strlen(line); j++)
                {
                    if (sense == 1)
                    {
                        if (temp -> memo[j] != line[j])
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (tolower(temp -> memo[j]) != tolower(line[j]))
                        {
                            break;
                        }
                    }
                    
                    if (j == strlen(line) - 1)
                    {
                        printf("Found Word: %s with length %d\n", temp -> memo, temp -> size);
                        mark = 1;
                    }
                }
            }
            
            if (mark == 0)
            {
                printf("No Match Found.\n");
            }
        }
        else if (left[0] == 'd' || left[0] == 'D')
        {
            // Dictionary Reset
            curr = orig;
            
            // Clears All Data
            for (int i = 0; i < spawns; i++)
            {
                temp = curr -> next;
                curr -> size = 0;
                strcpy(curr -> memo, "");
                curr -> next = NULL;
                curr = temp;
            }
            
            // Resets Markers
            orig = &list[0];
            last = &list[0];
            spawns = 0;
            printf("\nDictionary Cleared!\n");
        }
        else if (left[0] == 'c' || left[0] == 'C')
        {
            // End Input Loop
            printf("\nRecorded Entries:\n");
            
            mark = 0;
            for (int i = 0; i < spawns; i++)
            {
                printf("%d %s\n", openBook(i) -> size, openBook(i) -> memo);
                mark = 1;
            }
            if (mark == 0) { printf("Empty Dictionary!\n"); }
            
            printf("\nProgram Closed\n");
            break;
        }
        else
        {
            // Error Message
            printf("\nUnknown Command\n");
        }
    }
    
    return 0;
}