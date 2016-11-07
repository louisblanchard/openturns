
require(stats)
require(graphics)
## Analysis of the life−cycle savings data
## given in Belsley, Kuh and Welsch.

LifeCycleSavings <- read.table('LifeCycleSavings.csv', header=TRUE, sep=",")

lm.model1 <- lm(sr ~ pop15 + pop75 + dpi + ddpi , data=LifeCycleSavings)
plot(lm.model1 , which =1:6)

lm.model2 <- lm(sr^4 ~ pop75 + dpi , data=LifeCycleSavings)
plot(lm.model2 , which =1:6)

