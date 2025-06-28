#define RAND_NUM 32767
int rng_table[RAND_NUM];

void RandIndexIncrement(int *global_rand_i)
{
  (*global_rand_i)++;
  if (*global_rand_i>=RAND_NUM) {
    *global_rand_i=0;
  }
}

int myrand(int *global_rand_i)
{
  RandIndexIncrement(global_rand_i);
  return rng_table[*global_rand_i];
}

