#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

__int128_t modpow(__int128_t base, __int128_t exp, __int128_t modulus) {
  base %= modulus;
  __int128_t result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return (result + modulus) % modulus;
}

void compute_lincomb(char **commands, int com_count, __int128_t len, __int128_t *coeffs) {
  coeffs[0] = 1;
  coeffs[1] = 0;
  for (int j = 0; j < com_count; j++) {
    if (strstr(commands[j], "stack") != NULL) {
      coeffs[0] *= -1;
      coeffs[1] *= -1;
      coeffs[1] -= 1;
    } else if (strstr(commands[j], "cut") != NULL) {
      int cut_num = atoi(strrchr(commands[j], ' ') + 1);
      coeffs[1] -= cut_num;
    } else if (strstr(commands[j], "increment") != NULL) {
      int incr_num = atoi(strrchr(commands[j], ' ') + 1);
      coeffs[0] *= incr_num;
      coeffs[1] *= incr_num;
    }
    coeffs[0] %= len;
    coeffs[1] %= len;
  }
  coeffs[0] = (coeffs[0] + len) % len;
  coeffs[1] = (coeffs[1] + len) % len;
}

int main() {
  FILE *fp = fopen("input.txt" , "r");

  char **commands = calloc(200, sizeof(char*));
  for (int j = 0; j < 200; j++) commands[j] = calloc(25, sizeof(char));

  int com_count = 0;
  while (fgets(commands[com_count++], 25, fp) != NULL);
  fclose(fp);

  // Part 1
  __int128_t len = 10007;
  __int128_t coeffs[2];
  compute_lincomb(commands, com_count, len, coeffs);
  printf("x(k + 1) = %ld x(k) + %ld\n", (__int64_t) coeffs[0], (__int64_t) coeffs[1]);

  __int128_t start_index = 2019;
  __int128_t end_index = (start_index*coeffs[0] + coeffs[1]) % len;
  printf("%ld ends up at position %ld\n", (__int64_t)start_index, (__int64_t)end_index);

  puts("");
  // Part 2
  len = 119315717514047;
  compute_lincomb(commands, com_count, len, coeffs);
  // inversing: x' = ax + b ==> x' = a^(-1) (x - b)
  coeffs[0] = modpow(coeffs[0], len - 2, len);
  coeffs[1] = ((len - coeffs[1])*coeffs[0]) % len;
  printf("x(k + 1) = %ld x(k) + %ld\n", (__int64_t) coeffs[0], (__int64_t) coeffs[1]);

  __int128_t iters = 101741582076661;
  __int128_t big_coeffs[2];
  // scaling iterations: given x(k + 1) = ax(k) + b, x(k) = a^k x(0) + b (1 + a + ... + a^k)
  big_coeffs[0] = modpow(coeffs[0], iters, len);
  big_coeffs[1] = ((modpow(coeffs[0], iters, len) - 1)*modpow(coeffs[0] - 1, len - 2, len)) % len;
  big_coeffs[1] = (big_coeffs[1]*coeffs[1]) % len;
  printf("x(%ld) = %ld x(0) + %ld\n", (__int64_t) iters, (__int64_t) big_coeffs[0], (__int64_t) big_coeffs[1]);

  end_index = 2020;
  start_index = (((end_index*big_coeffs[0]) % len) + big_coeffs[1]) % len;
  printf("the card ending at position %ld is %ld\n", (__int64_t)end_index, (__int64_t)start_index);

  return 0;
}