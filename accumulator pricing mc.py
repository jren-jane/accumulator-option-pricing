import random
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from scipy import stats as ss
from collections import OrderedDict

# numba speeds up simulations
from numba.experimental import jitclass
from numba import int32, float32


@jitclass(OrderedDict({
    # number of simulations
    'times': int32,
    # time to maturity, usually set to 12 months / 1 year
    'maturity': float32,
    'rf_rate': float32,
    'spot_price': float32,
    'strike_price': float32,
    'knock_out_price': float32,
    'volatility': float32
}))
class FastSimulation:

    def __init__(self, times, maturity, rf_rate, spot_price, strike_price, knock_out_price, volatility):
        self.times = times
        self.maturity = maturity
        self.rf_rate = rf_rate
        self.spot_price = spot_price
        self.strike_price = strike_price
        self.knock_out_price = knock_out_price
        self.volatility = volatility

    def run(self):
        np.random.seed(1)
        # discounted payoffs from different simulations
        disc_payoffs = []
        for i in range(self.times):
            # generate 12 monthly returns from a normal distribution
            # written this way as size parameter is not supported by numba
            returns = [np.random.normal(
                loc=(self.rf_rate-0.5*self.volatility**2)*self.maturity/12,
                scale=np.sqrt(self.maturity/12)*self.volatility)/100 for _ in range(12)]
            # convert returns to a price array
            prices = np.exp(np.asarray(returns)).cumprod() * self.spot_price
            # discounted payoff in each simulation
            disc_payoff = 0
            for j in range(len(prices)):
                if prices[j] > self.knock_out_price:
                    break
                disc_payoff += self.buyer_payoff(prices[j]) * np.exp(-self.rf_rate * self.maturity / 12 * j)
            disc_payoffs.append(disc_payoff)
        return disc_payoffs

    def buyer_payoff(self, share_price):
        "Buyer payoff conditional on the accumulator not terminated"
        if share_price > self.knock_out_price:
            return 0
        payoff = 1000 * (share_price - self.strike_price)
        if self.strike_price <= share_price <= self.knock_out_price:
            return payoff
        else:
            return payoff * 2


v_1 = FastSimulation(1000000, 1, 0.1, 100, 95, 105, 1)
v_2 = FastSimulation(1000000, 1, 0.1, 100, 95, 105, 2)
v_3 = FastSimulation(1000000, 1, 0.1, 100, 95, 105, 3)
v_4 = FastSimulation(1000000, 1, 0.1, 100, 95, 105, 4)
v_5 = FastSimulation(1000000, 1, 0.1, 100, 95, 105, 5)

v = [v_1.run(), v_2.run(), v_3.run(), v_4.run(), v_5.run()]

mean = np.mean(v, axis=1)

std = np.std(v, axis=1)

print('fair value: ', mean)

print('standard error: ', std)

sns.distplot(v_1.run(), hist=False, rug=True, label='volatility=1%')
sns.distplot(v_2.run(), hist=False, rug=True, label='volatility=2%')
sns.distplot(v_3.run(), hist=False, rug=True, label='volatility=3%')
sns.distplot(v_4.run(), hist=False, rug=True, label='volatility=4%')
sns.distplot(v_5.run(), hist=False, rug=True, label='volatility=5%')

plt.show()