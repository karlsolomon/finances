import pandas

def printStats(df, name):
    payMean = f"{df[0].mean():.2f}"
    payStd =    f"{df[0].std():.2f}"    
    timeMean =  f"{df[1].mean():.2f}"
    timeStd =   f"{df[1].std():.2f}"
    print(name, str(payMean) + " +- " + str(payStd), "in " + str(timeMean) + " +- " + str(timeStd) + " months")


loans = pandas.read_csv("../../cpp/build/simulations.csv", header=None)
printStats(loans, "simulations")
