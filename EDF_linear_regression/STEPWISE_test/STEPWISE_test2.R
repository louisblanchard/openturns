
### DATA

data <- rbind(
c(676 ,33 ,5.00 ,1441.67 ,35185.5 ,16.4524) ,
c(516 ,35 ,4.75 ,1299.19 ,28170.4 ,13.9852) ,
c(1052 ,32 ,4.20 ,1154.27 ,26455.0 ,15.3276) ,
c(868 ,30 ,4.40 ,1045.15 ,25072.9 ,17.3128) ,
c(1008 ,33 ,5.55 ,521.62 ,31664.2 ,22.3312) ,
c(436 ,33 ,5.05 ,1273.02 ,25491.7 ,12.2778) ,
c(544 ,36 ,4.25 ,1346.35 ,20877.3 ,17.8225) ,
c(680 ,30 ,4.45 ,1253.88 ,25621.3 ,14.3516) ,
c(640 ,38 ,4.75 ,1242.65 ,27587.3 ,13.6826) ,
c(492 ,30 ,4.60 ,1281.95 ,26511.7 ,11.7566) ,
c(984 ,30 ,4.10 ,553.69 ,7886.5 ,9.8820) ,
c(1400 ,37 ,3.45 ,494.74 ,14596.0 ,16.6752) ,
c(1276 ,33 ,3.45 ,525.97 ,9826.8 ,12.3730) ,
c(1736 ,36 ,4.10 ,571.14 ,11978.4 ,9.4058) ,
c(1004 ,30 ,3.50 ,408.64 ,10368.6 ,14.9302) ,
c(396 ,30 ,3.25 ,646.65 ,17307.4 ,31.2865) ,
c(352 ,27 ,3.35 ,514.03 ,12822.0 ,30.1652) ,
c(328 ,29 ,3.20 ,350.73 ,8582.6 ,28.5901) ,
c(392 ,34 ,3.35 ,496.29 ,12369.5 ,19.8795) ,
c(236 ,36 ,3.30 ,580.92 ,14731.9 ,18.5056) ,
c(392 ,30 ,3.25 ,535.82 ,15060.6 ,22.1344) ,
c(268 ,28 ,3.25 ,490.34 ,11056.3 ,28.6101) ,
c(252 ,31 ,3.20 ,552.39 ,8118.9 ,23.1908) ,
c(236 ,31 ,3.20 ,661.32 ,13009.5 ,24.6917) ,
c(340 ,35 ,3.35 ,672.15 ,15003.7 ,22.6758) ,
c(2436 ,29 ,7.10 ,528.65 ,10225.0 ,0.3729) ,
c(2216 ,35 ,7.35 ,563.13 ,8024.2 ,0.2703) ,
c(2096 ,35 ,7.45 ,497.96 ,10393.0 ,0.3205) ,
c(1660 ,30 ,7.45 ,458.38 ,8711.6 ,0.2648) ,
c(2272 ,30 ,7.40 ,498.25 ,10239.6 ,0.2105) ,
c(824 ,26 ,4.85 ,936.26 ,20436.0 ,18.9875) ,
c(1196 ,29 ,4.60 ,894.79 ,12519.9 ,20.9687) ,
c(1960 ,25 ,5.20 ,941.36 ,18979.0 ,23.9841) ,
c(2080 ,26 ,4.75 ,1038.79 ,22986.1 ,19.9727) ,
c(1764 ,26 ,5.20 ,898.05 ,11704.5 ,21.3864) ,
c(412 ,25 ,4.55 ,989.87 ,17721.0 ,23.7063) ,
c(416 ,26 ,3.95 ,951.28 ,16485.2 ,30.5589) ,
c(504 ,26 ,3.70 ,939.83 ,17101.3 ,26.8415) ,
c(492 ,27 ,3.75 ,925.42 ,17849.0 ,27.7292) ,
c(636 ,27 ,4.15 ,954.11 ,16949.6 ,21.5699) ,
c(1756 ,24 ,5.60 ,720.72 ,11344.6 ,19.6531) ,
c(1232 ,27 ,5.35 ,782.09 ,14752.4 ,20.3295) ,
c(1400 ,26 ,5.50 ,773.30 ,13649.8 ,19.5880) ,
c(1620 ,28 ,5.50 ,829.26 ,14533.0 ,20.1328) ,
c(1560 ,28 ,5.40 ,856.96 ,16892.2 ,19.2420)
)

DATA = data.frame(data)
names(DATA) = c("BIO","SAL","pH","K","Na","Zn")

### SAVE DATA 
write.csv(DATA, file="DATA_test2.csv",row.names=FALSE)


### MIN/MAX MODELS
model_min <- lm( BIO~ 1 , data=DATA)
model_max <- lm( BIO~SAL+pH+K+Na+Zn , data=DATA)


### STEPWISE PROCEDURE
## Forward
model_0 <- lm( BIO~ 1 , data=DATA)
#AIC
lm_forward_AIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=2)
#BIC
lm_forward_BIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=log(45))

## Backward
model_0 <- lm( BIO~SAL+pH+K+Na+Zn , data=DATA)
#AIC
lm_backward_AIC <- step( model_max , scope=list( lower=model_min , upper=model_max) , direction="backward" , k=2)
#BIC
lm_backward_BIC <- step( model_max , scope=list( upper=model_max , lower=model_min) , direction="backward" , k=log(45))
## Both
model_0 <- lm( BIO~ 1 , data=DATA)
#AIC
lm_both_AIC <- step( model_0 , scope=list( lower=model_min , upper=model_max) , direction="both" , k=2)
#BIC
lm_both_BIC <- step( model_0 , scope=list( upper=model_max , lower=model_min) , direction="both" , k=log (45))


### ANOVA

summary(lm_forward_AIC)
AIC(lm_forward_AIC)

summary(lm_forward_BIC)
BIC(lm_forward_BIC)

summary(lm_backward_AIC)
AIC(lm_backward_AIC)

summary(lm_backward_BIC)
BIC(lm_backward_BIC)

summary(lm_both_AIC)
AIC(lm_both_AIC)

summary(lm_both_BIC)
BIC(lm_both_BIC)

