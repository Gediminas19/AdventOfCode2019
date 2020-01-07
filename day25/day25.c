#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../intcode_comp/intcode_comp.h"

char *directions[] = {"north", "east", "south", "west"};

void recurse(intcode_comp *comp, char *dangerous, int backdir, int dist, int *steps, int *security_steps) {
  run(comp);

  char into[1000];
  ascii_out_str(comp, into);
  printf("%s", into);

  char *itemer = strstr(into, "Items here:");
  while (itemer != NULL) {
    char *start_read = strchr(itemer, '-') + 2;
    int item_len = strcspn(start_read, "\n");

    char item[item_len + 1];
    strncpy(item, start_read, item_len);
    item[item_len] = '\0';
    if (strstr(dangerous, item) == NULL) {
      char take[50] = "take ";
      ascii_in_ln(comp, strcat(take, item));
      printf("%s\n", take);

      run(comp);
      ascii_out(comp);
    }

    itemer = strchr(start_read, '-');
  }

  for (int j = 0; j < 4; j++) {
    if (j != backdir && strstr(into, directions[j]) != NULL) {
      if (strstr(into, "Security Checkpoint") != NULL) {
        security_steps[0] = dist;
        memcpy(security_steps + 1, steps, dist*sizeof(int));
        security_steps[dist + 1] = j;
        return;
      }

      ascii_in_ln(comp, directions[j]);
      printf("%s\n", directions[j]);

      steps[dist] = j;
      recurse(comp, dangerous, (j + 2)%4, dist + 1, steps, security_steps);

      ascii_in_ln(comp, directions[(j + 2)%4]);
      printf("%s\n", directions[(j + 2)%4]);

      run(comp);
      ascii_out(comp);
    }
  }
}

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);

  char dangerous[] = "photons,infinite loop,escape pod,molten lava,giant electromagnet";
  int steps[10];
  int security_steps[10];
  recurse(comp, dangerous, -1, 0, steps, security_steps);

  printf("Full Sweep Completed! Security checkpoint is %d steps away\n", security_steps[0]);

  for (int j = 1; j <= security_steps[0]; j++) {
    ascii_in_ln(comp, directions[security_steps[j]]);
    printf("%s\n", directions[security_steps[j]]);

    run(comp);
    ascii_out(comp);
  }

  ascii_in_ln(comp, "inv");
  puts("inv");

  run(comp);

  char into[1000];
  ascii_out_str(comp, into);
  printf("%s", into);

  int item_count = 0;
  char *items[20];

  char *inventor = strstr(into, "inventory:");
  while (inventor != NULL) {
    char *start_read = strchr(inventor, '-') + 2;
    int item_len = strcspn(start_read, "\n");

    items[item_count] = calloc(item_len + 1, sizeof(char));
    strncpy(items[item_count++], start_read, item_len);

    inventor = strchr(start_read, '-');
  }

  int ways = 1 << item_count;
  printf("There are %d ways to choose which of the %d items to carry\n", ways, item_count);
  puts("Now we will test them until we are allowed through");

  // Each number from 0 to 2^item_count - 1 has a binary representation with
  // item_count places with 0 or 1 each; we drop the item if we see a 1
  for (int way = 0; way < ways; way++) {
    // First drop the items to drop
    for (int k = 0; k < item_count; k++) {
      if ((way >> k) & 1 == 1) {
        char drop[50] = "drop ";
        ascii_in_ln(comp, strcat(drop, items[k]));
        printf("%s\n", drop);

        run(comp);
        ascii_out(comp);
      }
    }
    // Now attempt to proceed
    ascii_in_ln(comp, directions[security_steps[security_steps[0] + 1]]);
    printf("%s\n", directions[security_steps[security_steps[0] + 1]]);

    run(comp);

    char into[1000];
    ascii_out_str(comp, into);
    printf("%s", into);

    if (strstr(into, "proceed") != NULL) {
      puts("Success!");

      free(work);
      free_comp(comp);
      return 0;
    }

    // If failed, pick up all items again
    for (int k = 0; k < item_count; k++) {
      char take[50] = "take ";
      ascii_in_ln(comp, strcat(take, items[k]));
      printf("%s\n", take);

      run(comp);
      ascii_out(comp);
    }
  }

  free(work);
  free_comp(comp);
  return 0;
}