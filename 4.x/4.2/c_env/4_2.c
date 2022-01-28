#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define READ  (0)
#define WRITE (1)

#define CHANCIX (0)
#define GAMBLIX (1)
#define MOODIX (2)

#define HEADS (1)
#define TAILS (0)

#define BLACK_SUIT (1)
#define RED_SUIT (0)


int calculate_chancix_value(int basic_price) {

    srand((unsigned int)time(NULL));
    int flipped_coin = rand() % 2;
    int chancix_value = basic_price;

    if (flipped_coin == HEADS) {
        chancix_value += basic_price * 0.05;
    } else if (flipped_coin == TAILS) {
        chancix_value -= basic_price * 0.05;
    }

    printf("The price Chancix evaluates: %d\n", chancix_value);

    return chancix_value;

}

int calculate_gamblix_value(int basic_price) {

    srand((unsigned int)time(NULL));
    int card_suit = rand() % 2;
    int card_num = rand() % 13 + 1;
    int gamblix_value = basic_price;

    if (card_suit == BLACK_SUIT) {
        gamblix_value += basic_price * card_num / 100.0;
    } else if (card_suit == RED_SUIT) {
        gamblix_value -= basic_price * card_num / 100.0;
    }

    printf("The price Gamblix evaluates: %d\n", gamblix_value);

    return gamblix_value;

}

int calculate_moodix_value(int basic_price) {

    time_t seconds;
    struct tm *timeStruct;
    seconds = time(NULL);
    timeStruct = localtime(&seconds);

    int hours = timeStruct->tm_hour;
    int minutes = timeStruct->tm_min;
    int moodix_value = basic_price;

    // add the hours and minutes in Japan time, subtract by 15, and devide by 100
    moodix_value += basic_price * ((hours + 9) % 24 + minutes - 15) / 100.0;

    printf("The price Moodix evaluates: %d\n", moodix_value);

    return moodix_value;

}


int main(int argc, char *argv[]) {

    const int CHILD_PROCESS_NUM = 3;

    pid_t process_ids[CHILD_PROCESS_NUM];

    char pipe_input[256], pipe_output[256];

    int parent2child_pipes[CHILD_PROCESS_NUM][2];
    int child2parent_pipes[CHILD_PROCESS_NUM][2];

    int p_i, close_i;

    int parent2child_pipe_status, child2parent_pipe_status;

    int basic_price, evaluated_price, price_sum, price_average;

    // prompt user input to set the base price
    printf("The basic price is ");
    fgets(pipe_input, 256, stdin);
    printf("The price Evalutrix indicates: %s", pipe_input);

    price_sum = atoi(pipe_input);


    for (p_i = 0; p_i < CHILD_PROCESS_NUM; ++p_i) {

       parent2child_pipe_status = pipe(parent2child_pipes[p_i]);
       child2parent_pipe_status = pipe(child2parent_pipes[p_i]);

       // if either a parent2child pipe or a child2parent pipe fails to open
       if (parent2child_pipe_status < 0 || child2parent_pipe_status < 0) {

           // close all proccesses' exisiting pipes
           for (close_i = 0; close_i < p_i; ++close_i) {

               close(parent2child_pipes[close_i][READ]);
               close(parent2child_pipes[close_i][WRITE]);
               close(child2parent_pipes[close_i][READ]);
               close(child2parent_pipes[close_i][WRITE]);

           }

           // if a child2parent pipe fails to open
	   if (child2parent_pipe_status < 0) {

               // close the most recently opened parent2child_pipe
               close(parent2child_pipes[p_i][READ]);
               close(parent2child_pipes[p_i][WRITE]);

	   }

           return 1;

       }

    }


    for (p_i = 0; p_i < CHILD_PROCESS_NUM; ++p_i) {

        process_ids[p_i] = fork();

        // if a child process fails to be generated
    	if (process_ids[p_i] < 0) {

            // close all processes' pipes
            for (close_i = 0; close_i < CHILD_PROCESS_NUM; ++close_i) {

                close(parent2child_pipes[close_i][READ]);
                close(parent2child_pipes[close_i][WRITE]);
                close(child2parent_pipes[close_i][READ]);
                close(child2parent_pipes[close_i][WRITE]);

            }

            return 1;

        // if the current process is a child process
    	} else if (process_ids[p_i] == 0) {

            close(parent2child_pipes[p_i][WRITE]);
            close(child2parent_pipes[p_i][READ]);

            read(parent2child_pipes[p_i][READ], pipe_input, 256);
            basic_price = atoi(pipe_input);

            evaluated_price = basic_price;

            if (p_i == CHANCIX) {
               // do what Chancix does
               evaluated_price = calculate_chancix_value(basic_price);
            } else if (p_i == GAMBLIX) {
               // do what Gamlix does
               evaluated_price = calculate_gamblix_value(basic_price);
            } else if (p_i == MOODIX) {
               // do what Moodix does
               evaluated_price = calculate_moodix_value(basic_price);
            }

            sprintf(pipe_input, "%d", evaluated_price);

            write(child2parent_pipes[p_i][WRITE], pipe_input, strlen(pipe_input) + 1);

            close(parent2child_pipes[p_i][READ]);
            close(child2parent_pipes[p_i][WRITE]);

            exit(0);

        // if the current process is the parent process
	} else if (process_ids[p_i] > 0) {

            close(parent2child_pipes[p_i][READ]);
            close(child2parent_pipes[p_i][WRITE]);

            write(parent2child_pipes[p_i][WRITE], pipe_input, strlen(pipe_input) + 1);

            read(child2parent_pipes[p_i][READ], pipe_output, 256);

            price_sum += atoi(pipe_output);

            close(parent2child_pipes[p_i][WRITE]);
            close(child2parent_pipes[p_i][READ]);

	}

    }


    price_average = price_sum / (CHILD_PROCESS_NUM + 1);
    printf("The price to declare is %d\n", price_average);
    
}
