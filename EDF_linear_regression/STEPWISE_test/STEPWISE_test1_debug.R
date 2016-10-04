factor.scope_debug <- function(factor, scope)
{
    drop <- scope$drop
    add <- scope$add

    if(length(factor) && !is.null(drop)) {# have base model
	nmdrop <- colnames(drop)
	facs <- factor
	if(length(drop)) {
	    nmfac <- colnames(factor)
            ## workaround as in PR#7842.
            ## terms.formula may have flipped interactions
            nmfac0 <- sapply(strsplit(nmfac, ":", fixed=TRUE),
                             function(x) paste(sort(x), collapse=":"))
            nmdrop0 <- sapply(strsplit(nmdrop, ":", fixed=TRUE),
                             function(x) paste(sort(x), collapse=":"))
	    where <- match(nmdrop0, nmfac0, 0L)
	    if(any(!where))
                stop(sprintf(ngettext(sum(where==0),
                                      "lower scope has term %s not included in model",
                                      "lower scope has terms %s not included in model"),
                             paste(sQuote(nmdrop[where==0]), collapse=", ")),
                     domain = NA)
	    facs <- factor[, -where, drop = FALSE]
	    nmdrop <- nmfac[-where]
	} else nmdrop <- colnames(factor)
	#if(ncol(facs) > 1) {
        #    ## check no interactions will be left without margins.
	#    keep <- rep.int(TRUE, ncol(facs))
	#    f <- crossprod(facs > 0)
	#    for(i in seq(keep)) keep[i] <- max(f[i, - i]) != f[i, i]
	#    nmdrop <- nmdrop[keep]
	#}
    } else nmdrop <- character()

    if(!length(add)) nmadd <- character()
    else {
	nmfac <- colnames(factor)
	nmadd <- colnames(add)
	if(!is.null(nmfac)) {
            ## workaround as in PR#7842.
            ## terms.formula may have flipped interactions
            nmfac0 <- sapply(strsplit(nmfac, ":", fixed=TRUE),
                             function(x) paste(sort(x), collapse=":"))
            nmadd0 <- sapply(strsplit(nmadd, ":", fixed=TRUE),
                             function(x) paste(sort(x), collapse=":"))
	    where <- match(nmfac0, nmadd0, 0L)
	    if(any(!where))
                stop(sprintf(ngettext(sum(where==0),
                                      "upper scope has term %s not included in model",
                                      "upper scope has terms %s not included in model"),
                             paste(sQuote(nmdrop[where==0]), collapse=", ")),
                     domain = NA)
	    nmadd <- nmadd[-where]
	    add <- add[, -where, drop = FALSE]
	}
	#if(ncol(add) > 1) {             # check marginality:
	#    keep <- rep.int(TRUE, ncol(add))
	#    f <- crossprod(add > 0)
	#    for(i in seq(keep)) keep[-i] <- keep[-i] & (f[i, -i] < f[i, i])
	#    nmadd <- nmadd[keep]
	#}
    }
    list(drop = nmdrop, add = nmadd)
}


## a slightly simplified version of stepAIC().
step_debug <- function(object, scope, scale = 0,
		 direction = c("both", "backward", "forward"),
		 trace = 1, keep = NULL, steps = 1000, k = 2, ...)
{
    mydeviance <- function(x, ...)
    {
        dev <- deviance(x)
        if(!is.null(dev)) dev else extractAIC(x, k=0)[2L]
    }

    cut.string <- function(string)
    {
	if(length(string) > 1L)
	    string[-1L] <- paste0("\n", string[-1L])
	string
    }
    re.arrange <- function(keep)
    {
	namr <- names(k1 <- keep[[1L]])
	namc <- names(keep)
	nc <- length(keep)
	nr <- length(k1)
	array(unlist(keep, recursive = FALSE), c(nr, nc), list(namr, namc))
    }

    step_debug.results <- function(models, fit, object, usingCp=FALSE)
    {
	change <- sapply(models, "[[", "change")
	rd <- sapply(models, "[[", "deviance")
        dd <- c(NA, abs(diff(rd)))
	rdf <- sapply(models, "[[", "df.resid")
	ddf <- c(NA, diff(rdf))
	AIC <- sapply(models, "[[", "AIC")
	heading <- c("Stepwise Model Path \nAnalysis of Deviance Table",
		     "\nInitial Model:", deparse(formula(object)),
		     "\nFinal Model:", deparse(formula(fit)),
		     "\n")
	aod <- data.frame(Step = I(change), Df = ddf, Deviance = dd,
                          "Resid. Df" = rdf, "Resid. Dev" = rd, AIC = AIC,
                          check.names = FALSE)
        if(usingCp) {
            cn <- colnames(aod)
            cn[cn == "AIC"] <- "Cp"
            colnames(aod) <- cn
        }
	attr(aod, "heading") <- heading
        ##stop gap attr(aod, "class") <- c("anova", "data.frame")
	fit$anova <- aod
	fit
    }

    Terms <- terms(object)
    object$call$formula <- object$formula <- Terms
    md <- missing(direction)
    direction <- match.arg(direction)
    backward <- direction == "both" | direction == "backward"
    forward  <- direction == "both" | direction == "forward"
    if(missing(scope)) {
	fdrop <- numeric()
        fadd <- attr(Terms, "factors")
        if(md) forward <- FALSE
    }
    else {
	if(is.list(scope)) {
	    fdrop <- if(!is.null(fdrop <- scope$lower))
		attr(terms(update.formula(object, fdrop)), "factors")
	    else numeric()
	    fadd <- if(!is.null(fadd <- scope$upper))
		attr(terms(update.formula(object, fadd)), "factors")
	}
        else {
	    fadd <- if(!is.null(fadd <- scope))
		attr(terms(update.formula(object, scope)), "factors")
	    fdrop <- numeric()
	}
    }
    models <- vector("list", steps)
    if(!is.null(keep)) keep.list <- vector("list", steps)
    n <- nobs(object, use.fallback = TRUE)  # might be NA
    fit <- object
    bAIC <- extractAIC(fit, scale, k = k, ...)
    edf <- bAIC[1L]
    bAIC <- bAIC[2L]
    if(is.na(bAIC))
        stop("AIC is not defined for this model, so 'step' cannot proceed")
    if(bAIC == -Inf)
        stop("AIC is -infinity for this model, so 'step' cannot proceed")
    nm <- 1
    ## Terms <- fit$terms
    if(trace) {
	cat("Start:  AIC=", format(round(bAIC, 2)), "\n",
	    cut.string(deparse(formula(fit))), "\n\n", sep = "")
        flush.console()
    }

    ## FIXME think about df.residual() here
    models[[nm]] <- list(deviance = mydeviance(fit), df.resid = n - edf,
			 change = "", AIC = bAIC)
    if(!is.null(keep)) keep.list[[nm]] <- keep(fit, bAIC)
    usingCp <- FALSE
    while(steps > 0) {
	steps <- steps - 1
	AIC <- bAIC
	ffac <- attr(Terms, "factors")
	scope <- factor.scope_debug(ffac, list(add = fadd, drop = fdrop))
	aod <- NULL
	change <- NULL
	if(backward && length(scope$drop)) {
	    aod <- drop1(fit, scope$drop, scale = scale,
                         trace = trace, k = k, ...)
	    rn <- row.names(aod)
	    row.names(aod) <- c(rn[1L], paste("-", rn[-1L], sep=" "))
            ## drop zero df terms first: one at time since they
            ## may mask each other
	    if(any(aod$Df == 0, na.rm=TRUE)) {
		zdf <- aod$Df == 0 & !is.na(aod$Df)
		change <- rev(rownames(aod)[zdf])[1L]
	    }
	}
	if(is.null(change)) {
	    if(forward && length(scope$add)) {
		aodf <- add1(fit, scope$add, scale = scale,
                             trace = trace, k = k, ...)
		rn <- row.names(aodf)
		row.names(aodf) <- c(rn[1L], paste("+", rn[-1L], sep=" "))
		aod <-
                    if(is.null(aod)) aodf
                    else rbind(aod, aodf[-1, , drop = FALSE])
	    }
	    attr(aod, "heading") <- NULL
	    ## need to remove any terms with zero df from consideration
	    nzdf <- if(!is.null(aod$Df))
		aod$Df != 0 | is.na(aod$Df)
	    aod <- aod[nzdf, ]
	    if(is.null(aod) || ncol(aod) == 0) break
	    nc <- match(c("Cp", "AIC"), names(aod))
	    nc <- nc[!is.na(nc)][1L]
	    o <- order(aod[, nc])
	    if(trace) print(aod[o, ])
	    if(o[1L] == 1) break
	    change <- rownames(aod)[o[1L]]
	}
	usingCp <- match("Cp", names(aod), 0L) > 0L
        ## may need to look for a `data' argument in parent
	fit <- update(fit, paste("~ .", change), evaluate = FALSE)
        fit <- eval.parent(fit)
        nnew <- nobs(fit, use.fallback = TRUE)
        if(all(is.finite(c(n, nnew))) && nnew != n)
            stop("number of rows in use has changed: remove missing values?")
        Terms <- terms(fit)
	bAIC <- extractAIC(fit, scale, k = k, ...)
	edf <- bAIC[1L]
	bAIC <- bAIC[2L]
	if(trace) {
	    cat("\nStep:  AIC=", format(round(bAIC, 2)), "\n",
		cut.string(deparse(formula(fit))), "\n\n", sep = "")
            flush.console()
        }
        ## add a tolerance as dropping 0-df terms might increase AIC slightly
	if(bAIC >= AIC + 1e-7) break
	nm <- nm + 1
        ## FIXME: think about using df.residual() here.
	models[[nm]] <-
	    list(deviance = mydeviance(fit), df.resid = n - edf,
		 change = change, AIC = bAIC)
	if(!is.null(keep)) keep.list[[nm]] <- keep(fit, bAIC)
    }
    if(!is.null(keep)) fit$keep <- re.arrange(keep.list[seq(nm)])
    step_debug.results(models = models[seq(nm)], fit, object, usingCp)
}



###DATA

residuals <- c( -0.582116782 , 1.086693510 , -0.399721205 , -0.633946243 , 0.395231729 ,
0.772610570 , -0.136194380 ,2.355799536 , -1.497328420 ,1.476005224 ,
0.352861564 ,0.214819947 , -1.267903208 ,0.050707356 , -0.697438980 ,
-0.532180229 ,0.685168909 , -0.316377628 ,1.108160517 ,0.497704896 ,
-0.970988724 , -0.210001297 , -0.294700591 , -1.041841128 , -1.216907586 ,
-0.071338268 ,0.682056977 ,0.861399989 , -0.472175238 ,1.309422121 ,
-0.230521064 ,0.192003185 , -0.779781975 ,0.410257691 ,0.843074912 ,
0.329530230 , -1.076554359 , -0.643986791 , -0.266615364 ,1.222148502 ,
1.917578182 , -0.729778393 ,1.750892164 , -0.585054533 , -0.810867729 ,
0.502805341 ,1.015855860 , -1.437759680 ,0.522319464 , -1.615017743 ,
0.009919569 , -0.370516643 , -0.878451566 ,0.629287259 , -1.546401701 ,
0.998819455 ,0.017004364 , -1.810681465 , -0.957616677 ,1.947087645 ,
-0.236604869 , -1.385877144 , -0.966861677 ,0.178435873 , -0.744111564 ,
0.263742599 , -0.864748050 , -1.586871329 , -0.543235503 , -2.022662858 ,
0.045421599 , -0.756500297 , -0.056899207 , -0.284722401 , -1.281168212 ,
-0.949955767 , -0.538904914 ,0.461559101 , -1.074395880 , -0.137625005 ,
1.074069198 ,1.286458611 , -0.420004514 , -0.688083629 ,0.008659029 ,
0.933305154 ,0.617793902 ,0.494433712 , -0.962344190 ,0.164384225 ,
1.446946222 , -1.814079142 , -0.897941868 ,0.899752771 , -0.262299458 ,
-1.621032799 ,0.779573028 ,2.572128388 ,0.564858745 , -0.385026248)

X1 <- c( 0.034706426 , 0.290767936 , 0.728156592 , 0.115909193 , 0.582720575 , 0.425280199 ,
0.236493143 ,0.141287319 ,0.600181255 ,0.401246054 ,0.673018230 ,0.257985681 ,
0.108834297 ,0.835923289 ,0.890142926 ,0.790246545 ,0.190852507 ,0.975805469 ,
0.565479056 ,0.908807890 ,0.504653023 ,0.740122601 ,0.354002776 ,0.153366482 ,
0.375221622 ,0.664329209 ,0.174817312 ,0.519426709 ,0.571352495 ,0.676933288 ,
0.929398339 ,0.873418029 ,0.997689419 ,0.302589654 ,0.778566483 ,0.067799081 ,
0.951812766 ,0.272334047 ,0.194561137 ,0.749473229 ,0.712693499 ,0.893107841 ,
0.755870604 ,0.015183525 ,0.441494394 ,0.191027371 ,0.609888250 ,0.206676293 ,
0.772025797 ,0.339300146 ,0.828846681 ,0.409719667 ,0.302344681 ,0.845720146 ,
0.323177859 ,0.184123734 ,0.268983908 ,0.691131240 ,0.562709426 ,0.052174745 ,
0.365856788 ,0.721637621 ,0.906206014 ,0.834441869 ,0.074260692 ,0.249086319 ,
0.680189428 ,0.168344676 ,0.286133859 ,0.459302204 ,0.269282414 ,0.948795165 ,
0.756694982 ,0.915244874 ,0.263998489 ,0.925717850 ,0.216722638 ,0.791369997 ,
0.059409349 ,0.730182722 ,0.401483874 ,0.667414125 ,0.716030358 ,0.423562593 ,
0.205078666 ,0.563724972 ,0.314290256 ,0.501724939 ,0.043623632 ,0.061959042 ,
0.413848649 ,0.149256541 ,0.615735705 ,0.549187532 ,0.099320023 ,0.397049573 ,
0.456026603 ,0.405879044 ,0.928600144 ,0.004797661)

X2 <- c( 0.26628690 , 0.01241699 , 0.75438715 , 0.87601195 , 0.01928122 , 0.25275297 ,
0.97491678 ,0.65326134 ,0.91499302 ,0.17071265 ,0.48152207 ,0.78341929 ,
0.95530031 ,0.56651492 ,0.73761982 ,0.61031407 ,0.62198340 ,0.69094466 ,
0.90037481 ,0.70770950 ,0.86459608 ,0.50745606 ,0.84468661 ,0.24766689 ,
0.45465460 ,0.57884709 ,0.97375677 ,0.69101525 ,0.76102114 ,0.75018138 ,
0.54557384 ,0.82096495 ,0.14076841 ,0.79646289 ,0.32374772 ,0.77338138 ,
0.91653015 ,0.97543707 ,0.30503079 ,0.58161811 ,0.47905593 ,0.58650534 ,
0.18944902 ,0.03407237 ,0.30781296 ,0.57724160 ,0.38090949 ,0.81526304 ,
0.64074527 ,0.89569018 ,0.78865702 ,0.80873360 ,0.48430452 ,0.47232857 ,
0.52831781 ,0.43737121 ,0.20905434 ,0.87058018 ,0.77842816 ,0.83239461 ,
0.88787506 ,0.74299715 ,0.13913586 ,0.06147424 ,0.19876305 ,0.94273318 ,
0.03161110 ,0.79914097 ,0.47929388 ,0.81347808 ,0.36649933 ,0.87879832 ,
0.05123150 ,0.86895233 ,0.92767153 ,0.17544749 ,0.22973628 ,0.39198412 ,
0.26949126 ,0.22304721 ,0.27694411 ,0.18944683 ,0.57508738 ,0.14926171 ,
0.70490317 ,0.26298672 ,0.10382995 ,0.35948080 ,0.43646719 ,0.71746655 ,
0.42678654 ,0.80324029 ,0.43047432 ,0.42907930 ,0.38329857 ,0.94655437 ,
0.27962646 ,0.48026150 ,0.80380411 ,0.15159296)

X3 <- c( 0.73700441 , 0.83464767 , 0.29429898 , 0.13571695 , 0.82722080 , 0.68132443 ,
0.96321855 ,0.47516257 ,0.60136929 ,0.31985735 ,0.51652630 ,0.65223091 ,
0.50081124 ,0.14232327 ,0.99712337 ,0.47616430 ,0.53835344 ,0.97220189 ,
0.20258498 ,0.23205429 ,0.85983013 ,0.06481128 ,0.04485789 ,0.24531053 ,
0.83946215 ,0.48352452 ,0.93755372 ,0.66433304 ,0.31542606 ,0.76462561 ,
0.94593671 ,0.65948305 ,0.01180484 ,0.74815281 ,0.88827075 ,0.66917065 ,
0.31502777 ,0.95432393 ,0.05726531 ,0.97433543 ,0.16090385 ,0.89968876 ,
0.27791746 ,0.43024295 ,0.14161640 ,0.66571473 ,0.18774323 ,0.60500628 ,
0.63963862 ,0.48505229 ,0.45613114 ,0.46707805 ,0.26295001 ,0.48495453 ,
0.61371822 ,0.80101866 ,0.02439784 ,0.64238118 ,0.36631516 ,0.21061044 ,
0.33942867 ,0.34431393 ,0.10427393 ,0.56194337 ,0.75057255 ,0.93369190 ,
0.07446672 ,0.36236549 ,0.16583604 ,0.68613010 ,0.26690523 ,0.92575998 ,
0.32155168 ,0.08719010 ,0.72193744 ,0.55486123 ,0.21257323 ,0.38171922 ,
0.81508578 ,0.87240239 ,0.69391951 ,0.16167284 ,0.66197479 ,0.56374490 ,
0.56385026 ,0.07477541 ,0.96933487 ,0.65637162 ,0.30376192 ,0.71401441 ,
0.33303853 ,0.34806343 ,0.15540656 ,0.06900335 ,0.05596738 ,0.34106706 ,
0.16070992 ,0.56598071 ,0.36514950 ,0.44716733)


X4 <- c( 0.50631288 , 0.88327545 , 0.82242069 , 0.54228130 , 0.53507792 , 0.23059617 ,
0.93327468 ,0.67238015 ,0.86602148 ,0.24583991 ,0.81779221 ,0.17951422 ,
0.54190890 ,0.36463946 ,0.32186283 ,0.93287791 ,0.38895454 ,0.45140537 ,
0.72069688 ,0.47503256 ,0.29015201 ,0.23731014 ,0.46734712 ,0.13753621 ,
0.80474585 ,0.10117281 ,0.86900606 ,0.05236580 ,0.52000397 ,0.90339192 ,
0.98003055 ,0.54253358 ,0.89608205 ,0.50287556 ,0.21216763 ,0.02560553 ,
0.98274516 ,0.84118108 ,0.75645416 ,0.41713635 ,0.69922143 ,0.48320505 ,
0.47325558 ,0.19804112 ,0.19640943 ,0.64230591 ,0.38772198 ,0.39059848 ,
0.23676848 ,0.70180806 ,0.38854767 ,0.99189292 ,0.08003587 ,0.32701360 ,
0.24798809 ,0.59945459 ,0.21561393 ,0.96287863 ,0.09618598 ,0.79052610 ,
0.41606417 ,0.34146277 ,0.86914091 ,0.98900546 ,0.98738888 ,0.18470378 ,
0.62923164 ,0.38267360 ,0.91088795 ,0.57485696 ,0.73893422 ,0.49621020 ,
0.66870881 ,0.87856229 ,0.44961932 ,0.92780769 ,0.76799308 ,0.32035656 ,
0.65346870 ,0.99378484 ,0.95250963 ,0.19916736 ,0.75267644 ,0.14325332 ,
0.81840871 ,0.02439605 ,0.20550406 ,0.84798599 ,0.33296719 ,0.26543535 ,
0.13531582 ,0.25322711 ,0.25118445 ,0.38091998 ,0.38926009 ,0.25834904 ,
0.18724617 ,0.78182731 ,0.17021600 ,0.31960810)


### SAVE DATA 
DATA = data.frame(cbind(residuals,X1,X2,X3,X4))
names(DATA) = c("R","X1","X2","X3","X4")
write.csv(DATA, file="DATA_test1.csv",row.names=FALSE)


### DESIGN
X <- data.frame(cbind(X1,X2,X3,X4))
names(X) <- c("X1","X2","X3","X4")

### LINEAR MODEL
myLinearModel <- function(X) {
    beta <- c (14 , -7 , -17 , -7 , -3 ,13 , -16 , -4 ,12 ,3 ,13 ,20 ,17 , -10 ,7)
    Design <- cbind(rep(1,dim(X)[1]),X[,1],X[,2],X[,3],X[,4], X[,3]^2,X[,4]^2,X[,1]^2,X[,1]*X[,2] , X[,2]*X[,4] , X[,3]*X[,4] , X[,1]*X[,2]*X[,3] , X[,1]^3 ,X[,2]^3 ,X[,4]^3 )
    Y <- Design%*%as.matrix(beta)
    return(Y)
}

### OBSERVATIONS
Y <- myLinearModel(X) + residuals

### MIN/MAX/START MODELS
model_min <- lm(Y~1 , data=X)
model_max <- lm(Y~(X1+X2+X3+X4)^3+I(X1^2)+I(X1^3)+I(X2^2)+I(X2^3)+I(X3^2)+I(X3^3)+I(X4^2)+I(X4^3), data=X)
model_0 <- lm(Y~X1+X2+X3+X4 , data=X)


### STEPWISE PROCEDURE

## Forward
#AIC
lm_forward_AIC <- step_debug( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=2)
#BIC
lm_forward_BIC <- step_debug( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=log(100))


## Backward
#AIC
lm_backward_AIC <- step_debug( model_max , scope=list(lower=model_min , upper=model_max) , direction="backward" , k=2)
#BIC
lm_backward_BIC <- step_debug( model_max , scope=list(upper=model_max , lower=model_min) , direction="backward" , k=log(100))

## Both
#AIC
lm_both_AIC <- step2( model_0 , scope=list(lower=model_min , upper=model_max) , direction="both" , k=2)
#BIC
lm_both_BIC <- step2( model_0 , scope=list(upper=model_max , lower=model_min) , direction="both" , k=log(100))


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

