#include <stdio.h>

/* predefined exchange rates */
#define usd_to_euro 0.77
#define usd_to_aus 0.96
#define usd_to_yen 95.90
#define usd_to_yuan 6.21
#define usd_to_rupee 54.32

void main() {
  float usd_amount; /* USD amount*/
  float fc_amount;  /* Foreign currency amount*/
  float rate;
  int currency;

  /* Enter amount in USD */
  printf("Please enter USD amount:\n");
  scanf("%f", &usd_amount);

  printf("Please choose the currency:\n");
  printf("1 - Euro.(%f)\n", usd_to_euro);
  printf("2 - Australia Dollar.(%f)\n", usd_to_aus);
  printf("3 - Japanese Yen.(%f)\n", usd_to_yen);
  printf("4 - Chinese Yuan.(%f)\n", usd_to_yuan);
  printf("5 - Indian Rupee.(%f)\n", usd_to_rupee);
  printf("6 - Other\n");

  scanf("%d", &currency);

  /* determine the rate */
  if (currency == 1)
    rate = usd_to_euro;
  else if (currency == 2)
    rate = usd_to_aus;
  else if (currency == 3)
    rate = usd_to_yen;
  else if (currency == 4)
    rate = usd_to_yuan;
  else if (currency == 5)
    rate = usd_to_rupee;
  else {
    printf("Enter the exchange rate USD -> Foreign Currency\n");
    scanf("%f", &rate);
  }

  /* Convert currency */
  if (rate > 0) {
    fc_amount = usd_amount * rate;
  }

  printf("Result: %.2f", fc_amount);
}
