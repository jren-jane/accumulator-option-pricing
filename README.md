# accumulator-option-pricing
The .py files price an accumulator option using Monte Carlo simulation and calculate its profit and loss distribution given different volatilities. The .m files provides an analytical function to approximate the value of an accumulator option. 

## The option
An accumulator is settled periodically throughout its term. At each settlement:

- If the market price of the underlying security is above the predetermined knock-out price, the contract is terminated.
- If the market price of the underlying security is between the knock-out price and the strike price, the buyer "accumulates" the underlying security at the strike price.
- If the market price of the underlying security is below the strike price, the buyer is obligated to buy the underlying security at the strike price at 2 (or more) times of the predetermined amount.

## The analytical pricing method
- In the case that the knock-out barrier is extremely high, there will be no knock-out. And the accumulator is equivalent to a series of call and put options on forwards. The value of a forward contract can be calculated by risk-neutral pricing. (See file acc_XH.m)
- The approximation formula for an accumulator option is taken from a paper by Kin Lam, except that possible earlier payoff when a knock-out happens during a settlement period is considered. (See file ana_acc.m)


## Results
![img](https://github.com/jren-jane/accumulator-option-pricing-monte-carlo/blob/259b56847ec0cf76ece44554f56fe2fc3274c200/myplot.png)
