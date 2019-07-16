#include <stdio.h>
#include <piplib/piplib64.h>

int main() {

  PipMatrix * domain, * input_domain;
  PipQuast * solution;
  PipOptions * options;
  PipList * l;
  long long * v;

  options = pip_options_init();
  options->Verbose = 1;
  options->Simplify = 1;
  options->Urs_parms = -1;
  options->Urs_unknowns = -1;
  options->Maximize = 1;
  input_domain = pip_matrix_read(stdin);

  domain = pip_matrix_alloc(input_domain->NbRows+1, input_domain->NbColumns+1);
  for (int i=0; i<input_domain->NbRows; i++) {
    domain->p[i][0] = input_domain->p[i][0];
    domain->p[i][1] = 0;
    for (int j=1; j<input_domain->NbColumns; j++) {
      domain->p[i][j+1] = input_domain->p[i][j];
    }
  }

  for (int i=0; i<domain->NbColumns; i++) {
    domain->p[domain->NbRows-1][i] = 0;
  }

  pip_matrix_free(input_domain);

  v = domain->p[domain->NbRows-1];
  v[0] = 0;
  v[1] = 1;

  for (int i=2; i<domain->NbColumns-1; i++) {
    v[i] = -1;

    options->Maximize = 0;
    solution = pip_solve(domain, NULL, -1, options);
    if (solution != NULL) {
      if (solution->list != NULL || solution->condition != NULL) {
        l = solution->list;
        printf("%lld", l->vector->the_vector[0]);
      }
    }
    pip_quast_free(solution);

    printf(" <= x_%d <= ", i-2);

    options->Maximize = 1;
    solution = pip_solve(domain, NULL, -1, options);
    if (solution != NULL) {
      if (solution->list != NULL || solution->condition != NULL) {
        l = solution->list;
        printf("%lld", l->vector->the_vector[0]);
      }
    }
    pip_quast_free(solution);

    printf("\n");
    v[i] = 0;
  }

  for (int i=2; i<domain->NbColumns-1; i++) {
    for (int j=i+1; j<domain->NbColumns-1; j++) {
      v[i] = -1;
      v[j] = 1;

      options->Maximize = 0;
      solution = pip_solve(domain, NULL, -1, options);
      if (solution != NULL) {
        if (solution->list != NULL || solution->condition != NULL) {
          l = solution->list;
          printf("%lld", l->vector->the_vector[0]);
        }
      }
      pip_quast_free(solution);

      printf(" <= x_%d - x_%d <= ", i-2, j-2);

      options->Maximize = 1;
      solution = pip_solve(domain, NULL, -1, options);
      if (solution != NULL) {
        if (solution->list != NULL || solution->condition != NULL) {
          l = solution->list;
          printf("%lld", l->vector->the_vector[0]);
        }
      }
      pip_quast_free(solution);

      printf("\n");
      v[j] = -1;

      options->Maximize = 0;
      solution = pip_solve(domain, NULL, -1, options);
      if (solution != NULL) {
        if (solution->list != NULL || solution->condition != NULL) {
          l = solution->list;
          printf("%lld", l->vector->the_vector[0]);
        }
      }
      pip_quast_free(solution);

      printf(" <= x_%d + x_%d <= ", i-2, j-2);

      options->Maximize = 1;
      solution = pip_solve(domain, NULL, -1, options);
      if (solution != NULL) {
        if (solution->list != NULL || solution->condition != NULL) {
          l = solution->list;
          printf("%lld", l->vector->the_vector[0]);
        }
      }
      pip_quast_free(solution);

      printf("\n");
      v[i] = 0;
      v[j] = 0;
    }
  }

  pip_options_free(options);
  pip_matrix_free(domain);

  pip_close();

  return 0;

}
