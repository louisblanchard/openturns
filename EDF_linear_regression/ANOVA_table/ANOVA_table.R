
# Output variables : weight of the plants

ctl <- c(4.17 ,5.58 ,5.18 ,6.11 ,4.50 ,4.61 ,5.17 ,4.53 , 5.33 ,5.14)
trt <- c(4.81 ,4.17 ,4.41 ,3.59 ,5.87 ,3.83 ,6.03 ,4.89 , 4.32 ,4.69)

# Input variables :
# − group " Ctl " : control group ( plant with standard conditions )
# − group " Trt " : Treatment group ( plant with nutritionally enriched environment )

group <- gl( 2, 10, 20, labels = c("Ctl","Trt"))
weight <- c(ctl,trt)

lm.D9 <- lm(weight ~ group)
summary(lm.D9)

### SAVE DATA
DATA = data.frame(cbind(ctl,trt))
names(DATA) = c("ctl","trt")
write.csv(DATA, file="DATA.csv",row.names=FALSE)

