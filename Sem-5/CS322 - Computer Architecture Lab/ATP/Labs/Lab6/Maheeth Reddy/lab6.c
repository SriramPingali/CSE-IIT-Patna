// AGE PREDICTOR, C program
#include <stdio.h>
// Minimum and Maximum age of a person, initially
#define ILOW 1
#define IHIGH 100

int main(void) {
    // Welcome messages
    printf("\n                 *************************\n");
    printf("Hey, I am the... *     AGE PREDICTOR     *\n");
    printf("                 *************************\n\n");
    printf("I can predict your age with hints from you…\n\n");

    printf("Just answer whether you are\n");
    printf("younger or older than my prediction …\n");
    printf("That’s it….\n");

    // Check whether a particular age is already asked or not
    int asked[IHIGH-ILOW+1] = { 0 };

    int min = ILOW;     // minimum possible value of user's age
    int max = IHIGH;    // maximum possible value of user's age

    while(1) {
        int mid = (min+max)/2;      // average of min and max
        
        // Check whether age in mid is already asked or not
        if (asked[mid-1] != 0) {
            // If asked, tell user they gave misleading input
            printf("\nHey, you are misleading me!\n\n");
            return 0;
        }

        // If not, then note that this age is asked
        asked[mid-1]++;

        // Ask user if prediction is right or wrong
        printf("\nAre you %d years old ?\n",mid);

        // Show menu
        printf("[0] Yes!\n");
        printf("[1] Younger than that\n");
        printf("[2] Older than that\n");

        // Get user input
        int input;
        while(1) {
            printf(">> ");
            scanf("%d",&input);

            // if input is invalid i.e., not 0/1/2, Re-prompt User for input
            if(input >= 0 && input <= 2) break;
            else {
                printf("Invalid option. Try Again.\n");
            }
        }

        switch (input) {
            case 0:
                // If input is 0, then user's age is matching
                // Display exit message and quit
                printf("\nGot it... you are %d years old!\n\n",mid);
                return 0;
            
            case 1:
                // If input is 1, then user's age is lesser than mid
                // So, update max and re-prompt
                max = mid;
                break;
            
            case 2:
                // If input is 2, then user's age is greater than mid
                // So, update min and re-prompt
                min = mid;
                break;
        }
    }
}