#! /usr/bin/env python

import time
import numpy as np
from math import log
from numpy.linalg import norm

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# BACKWARD method
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def compute_HY_backward0(X,Y):
    HY=[]
    [n,p]=X.shape
    for i in range(p):
        Xi = np.delete(X,i,1)
        Ai = np.linalg.inv(np.dot(Xi.T,Xi))
        Hi = np.dot(np.dot(Xi,Ai),Xi.T)
        HY.append(np.dot(Hi,Y))
    return np.array(HY)

#----------------------------------
def compute_HY_backward1(X,Y,A):
    HY=[]
    [n,p]=X.shape
    for i in range(p):
        Xi = np.delete(X,i,1)
        XiY = np.dot(Xi.T,Y) 
        Acol = np.delete(A,i,1)
        Alig = np.delete(Acol,i,0)
        temp = (np.dot(Acol[i,:],XiY)/A[i,i])  
        AiXiY = np.dot(Alig,XiY) -temp*Acol[i,:]  
        HY.append(np.dot(Xi,AiXiY))
    return np.array(HY)

#----------------------------------
def compute_HY_backward2(X,Y,A):
    HY=[]
    [n,p]=X.shape
    B = np.dot(X.T,Y)
    for i in range(p):
        Bi = np.copy(B) 
        Bi[i] = 0.
        E = np.dot(A,Bi)
        V = E - (E[i]/A[i,i])*A[:,i]
        HY.append(np.dot(X,V))
    return np.array(HY)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# FORWARD method
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def compute_HY_forward0(X,Y,x):
    HY=[]
    [n,p]=x.shape
    for i in range(p):
        Xi = np.vstack((X.T,x[:,i])).T
        Ai = np.linalg.inv(np.dot(Xi.T,Xi))
        Hi = np.dot(np.dot(Xi,Ai),Xi.T)
        HY.append(np.dot(Hi,Y))
    return np.array(HY)

#----------------------------------
def compute_HY_forward1(X,Y,A,x):
    HY=[]
    [n,p]=x.shape
    B = np.dot(A,X.T)
    XtY = np.dot(X.T,Y)
    for i in range(p):
        xi = x[:,i]
        D = np.dot(B,xi)
        C = np.dot(xi,xi)-np.dot(np.dot(xi,X),D)
        temp = (np.dot(xi.T,Y)-np.dot(D.T,XtY))/C 
        HY.append(np.dot(B.T,XtY)-temp*(np.dot(X,D)-xi))
    return np.array(HY)

#----------------------------------
def compute_HY_forward2(X,Y,A,x):
    HY=[]
    [n,p]=x.shape
    B = np.dot(X.T,Y)
    M = np.dot(X,A)
    Z = np.dot(M,B)
    for i in range(p):
        xi = x[:,i]
        Bi = np.dot(X.T,xi)
        Di = np.dot(M,Bi)
        temp = np.dot(xi,(Y-Z))/np.dot(xi,(xi-Di))
        HY.append(Z+temp*(xi-Di))
    return np.array(HY)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
n=1000
p=100
X = np.random.rand(n,p)
A = np.linalg.inv(np.dot(X.T,X))
Y = np.random.rand(n)
x = np.random.rand(n,p)

#--------------------------------------------
# BACKWARD test
#--------------------------------------------

t0 = time.time()
HY0 = compute_HY_backward0(X,Y)
t1 = time.time()
HY1 = compute_HY_backward1(X,Y,A)
t2 = time.time()
HY2 = compute_HY_backward2(X,Y,A)
t3 = time.time()
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
print "BACKWARD : (N,P)=[%d,%d] ,  CPU time : HY0 = %.4f s ,  HY1 = %.4f s , HY2 = %.3f s \n"%(n,p,t1-t0,t2-t1,t3-t2)
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
N_HY0  = norm(HY0)
N_abs1 = norm(HY0-HY1)
N_rel1 = N_abs1/N_HY0
N_abs2 = norm(HY0-HY2)
N_rel2 = N_abs2/N_HY0
print "(absolute error) :  norm(HY0-HY1)            = %.5e , norm(HY0-HY2)           = %.5e "%(N_abs1, N_abs2)
print "(relative error) :  norm(HY0-HY1)/norm(HY0)  = %.5e , norm(HY0-HY2)/norm(HY0) = %.5e "%(N_rel1, N_rel2)
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
#--------------------------------------------
# FORWARD test
#--------------------------------------------

t0 = time.time()
HY0 = compute_HY_forward0(X,Y,x)
t1 = time.time()
HY1 = compute_HY_forward1(X,Y,A,x)
t2 = time.time()
HY2 = compute_HY_forward2(X,Y,A,x)
t3 = time.time()
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
print " FORWARD : (N,P)=[%d,%d] ,  CPU time : HY0 = %.4f s ,  HY1 = %.4f s , HY2 = %.3f s "%(n,p,t1-t0,t2-t1,t3-t2)
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
N_HY0  = norm(HY0)
N_abs1 = norm(HY0-HY1)
N_rel1 = N_abs1/N_HY0
N_abs2 = norm(HY0-HY2)
N_rel2 = N_abs2/N_HY0
print "(absolute error) :  norm(HY0-HY1)            = %.5e , norm(HY0-HY2)           = %.5e "%(N_abs1, N_abs2)
print "(relative error) :  norm(HY0-HY1)/norm(HY0)  = %.5e , norm(HY0-HY2)/norm(HY0) = %.5e "%(N_rel1, N_rel2)
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
