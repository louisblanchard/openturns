
### Initial Design Matrix
X <- rbind(
c(0.8993728 , 0.5254674 , 0.43965207),
c(0.7353028 , 0.1053970 , 0.07031078),
c(0.5627121 , 0.8861007 , 0.15961450),
c(0.2085024 , 0.4172028 , 0.02295918),
c(0.1743386 , 0.6587370 , 0.20820335)
)

### SAVE DATA
write.csv(X, file="DATA.csv",row.names=FALSE, col.names=FALSE)

### Matrix inversions with one removed column
A = solve(t(X)%*%X)

for ( i in 1:3) {
    Xi <- X[,-i]
    print(solve(t(Xi)%*%Xi)-(A[-i,-i]-(A[-i,i]%*%t(A[-i,i]))/A[i,i]))
}


### Matrix inversion with one added column
#Added column
x <- c( 0.7972436 , 0.2782935 , 0.8539293 , 0.1541903 , 0.5713438 )

B = A%*%t(X)
D = B%*%x
C = t(x)%*%x - x%*%X%*%D

Xx <- cbind(X,x)
solve(t(Xx)%*%Xx) - rbind( cbind(A+D%*%t(D)/C[1,1] , -D/C[1,1]) , cbind(-t(D)/C[1,1] , 1/C[1,1]) )
