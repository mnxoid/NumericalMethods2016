      program test1
      implicit real*8(a-h,o-z)
      dimension y(10,13),ymax(10),error(10),pw(100),
     1          fsave(20),iwork(10)      
      common/stcom1/t,h,hmin,hmax,eps,n,mf,kflag,jstart,maxord
      common/stcom2/hused,nqused
      common/stcom3/ml,mu
      common/stcom4/nstep,nfun,njac
      nydim=10
      eps=1.d-2
      kb=0
401   continue
      n=2
      t=0.0d0
      tend=4.d0
      y(1,1)=0.d0
      y(2,1)=0.d0
      h=5.0d-3
      hmax=tend
      hmin=1.d-15
      jstart=0
      mf=21
      maxord=5
      write(0,20) mf,eps
20    format(//3x,'mf=',i2/,' eps='d11.3)
      nstep=0
      nfun=0
      njac=0
      do 30 i=1,n
30    ymax(i)=1.d0
40    continue
      call stiff(y,ymax,error,pw,fsave,iwork,nydim)
      if(kflag.eq.0)go to 60
      write(0,50) kflag
50    format(/'  kflag=',i2/)
      stop
60    continue
      if(dabs(tend-t).le.1.d-15) go to 90
      if(tend-t-h) 80,40,40
80    e=tend-t
      s=e/h
      do 85 i=1,n
      do 85 j=1,jstart
85    y(i,1)=y(i,1)+y(i,j+1)*s**j
      t=t+e
      go to 60
90    continue
      write(0,556) h,t,(y(i,1),i=1,n)
556   format(1x,5d16.8)     
      write(0,95) nstep,nfun,njac
95    format(/'  nstep=',i4,' nfun= ',i5,' njac=',i4)
      kb=kb+1
      if(kb.ge.3) go to 402
      eps=eps*1.d-2
      go to 401
402   continue
      stop
      end
      subroutine diffun (n,t,y,ydot)
      implicit real*8 (a-h,o-z)
      dimension y(1),ydot(1)
      ydot(1)=-2.d3*y(1)+1.d3*y(2)+1.0
      ydot(2)=y(1)-y(2)
      return
      end
      subroutine pederv(n,t,y,pw,nydim)
      implicit real*8 (a-h,o-z)
      dimension y(1),pw(1)
      pw(1)=-2.0d3
      pw(2)=1.d0
      pw(nydim+1)=1.d3
      pw(nydim+2)=-1.d0
      return
      end
      subroutine stiff(y,ymax,error,pw,fsave,iwork,nydim)
      implicit real*8 (a-h,o-z)
      logical evalja,conver
      common/stcom1/ t,h,hmin,hmax,eps,n,mf,kflag,jstart,maxder
      common/stcom2/ hused,nqused
      common/stcom3/ ml,mu
      common/stcom4/ nstep,nfun,njac
      dimension y(nydim,1),el(13),tq(4),ymax(1)
      dimension error(1),pw(1),fsave(1),iwork(1)
      data anoise/1.d-15/
      data dfltzr/1.d-50/,maxite/3/,maxfai/3/
      data rmxini/1.d+4/,rmxnor/10.d0/,rmxfai/2.d0/,idelay/10/
      data rhcorr/0.25d0/,rherr3/0.1d0/,thrshl/0.0d0/,rctest/0.3d0/
      data bias1/1.3d0/,bias2/1.2d0/,bias3/1.4d0/     
      kflag=0
      told=t
      if(jstart)120,100,200
100   hused=0.0d0
      nqused=0
      n1=nydim+1
      n2=nydim+n1
      nsq=n*nydim
      nold=n
      nm1=nydim-1
      noml=ml*nydim
      mwidth=ml+mu+1
      call diffun(n,t,y,fsave)
      nfun=nfun+1
      do 110 i=1,n
110   y(i,2)=fsave(i)*h
      nq=1
      l=2
      idoub=l+1
      rmax=rmxini
      epsj=dsqrt(anoise)
      trend=1.d0
      oldl0=1.d0
      rc=0.0d0
      hold=h
      meth=mf/10
      miter=mf-10*meth
      mfold=mf
      meo=meth
      mio=miter
      evalja=.false.
      if(miter.ne.0)evalja=.true.
      conver=.false.
      call coset(meth,nq,el,tq,maxder)
      lmax=maxder+1
      edn=(tq(1)*eps)**2
      e=(tq(2)*eps)**2
      eup=(tq(3)*eps)**2
      bnd=(tq(4)*eps)**2
      epsold=eps
      goto 200
120   if(mf.eq.mfold) goto 150
      meo=meth
      mio=miter
      meth=mf/10
      miter=mf-10*meth
      mfold=mf
      if(miter.eq.mio) goto 130
      evalja=.false.
      if(miter.ne.0) evalja=.true.
      conver=.false.
130   if(meth.eq.meo)goto 150
      idoub=l+1
      call coset(meth,nq,el,tq,maxder)
      lmax=maxder+1
      rc=rc*el(1)/oldl0
      oldl0=el(1)
140   edn=(tq(1)*eps)**2
      e=(tq(2)*eps)**2
      eup=(tq(3)*eps)**2
      bnd=(tq(4)*eps)**2
      epsold=eps
      goto 160
150   if(eps.ne.epsold)goto 140
160   if(n.eq.nold)goto 180
      idoub=l+1
      evalja=.false.
      if(miter.ne.0)evalja=.true.
      conver=.false.
      if(miter.ne.0) nsq=n*nydim
      nold=n
180   if(h.eq.hold)goto 200
      rh=h/hold
      h=hold
      call rescal(y,nydim,rh,rmax,rc,l,idoub)
200   if((dabs(rc-1.d0).gt.rctest).and.(miter.ne.0)) evalja=.true.
      t=t+h
      call predic(nydim,y,nq)
220   do 230 i=1,n
230   error(i)=0.0d0
      iter=0
      call diffun(n,t,y,fsave(n1))
      nfun=nfun+1
      if(.not.evalja)goto 440
      njac=njac+1
      goto (240,260,310,350,380),miter
240   call pederv(n,t,y,pw,nydim)
      r=-el(1)*h
      do 250 i=1,n
      do 250 j=i,nsq,nydim
250   pw(j)=pw(j)*r
      goto 300
260   sum=0.0d0
      do 270 i=1,n
270   sum=sum+fsave(i+nydim)**2
      r0=dabs(h)*dsqrt(sum)*1.d3*anoise
      j1=-nydim
      do 290 j=1,n
      j1=j1+nydim
      yj=y(j,1)
      r=epsj*ymax(j)
      r=dmax1(r,r0)
      y(j,1)=y(j,1)+r
      factor=-el(1)*h/r
      call diffun(n,t,y,fsave)
      nfun=nfun+1
      do 280 i=1,n
280   pw(i+j1)=(fsave(i)-fsave(i+nydim))*factor
290   y(j,1)=yj
300   do 305 i=1,nsq,n1
305   pw(i)=pw(i)+1.d0
      evalja=.false.
      conver=.false.
      rc=1.d0
      call matdec(n,pw,nydim,iwork)
      goto 460
310   r=el(1)*.1d0
      conver=.false.
      do 320 i=1,n
320   pw(i)=y(i,1)+r*(fsave(i+nydim)*h-y(i,2))
      call diffun(n,t,pw,fsave)
      nfun=nfun+1
      do 330 i=1,n
      r0=pw(i)-y(i,1)
      pw(i)=1.0d0
      factor=r0-el(1)*h*(fsave(i)-fsave(nydim+i))
      fsave(i)=r0/r
      if(dabs(r0).eq.0.0d0)goto 330
      if(factor.eq.0.0d0)goto 560
      pw(i)=r0/factor
      fsave(i)=fsave(i)*pw(i)
330   continue
      evalja=.false.
      rc=1.d0
      d=0.0d0
      do 340 i=1,n
      error(i)=error(i)+fsave(i)
      d=d+(fsave(i)/ymax(i))**2
340   fsave(i)=y(i,1)+el(1)*error(i)
      goto 550
350   call pederv(n,t,y,pw,nydim)
      r=-el(1)*h
      i1=ml+1
      i2=n
      do 370 j=1,mwidth
      do 360 i=i1,i2
360   pw(i)=pw(i)*r
      i1=i1+nydim
      if(j.le.ml) i1=i1-1
      i2=i2+nydim
      if(j.gt.ml) i2=i2-1
370   continue
      goto 420
380   sum=0.0d0
      do 390 i=1,n
390   sum=sum+dabs(fsave(i+nydim))
      r0=dabs(h)*sum*1.d+3*anoise
      j1=noml
      do 410 j=1,n
      j1=j1+nydim
      yj=y(j,1)
      r=epsj*ymax(j)
      r=dmax1(r,r0)
      y(j,1)=y(j,1)+r
      factor=-el(1)*h/r
      call diffun(n,t,y,fsave)
      nfun=nfun+1
      i1=max0(j-mu,1)
      i2=min0(j+ml,n)
      do 400 i=i1,i2
400   pw(j1-nm1*i)=(fsave(i)-fsave(i+nydim))*factor
410   y(j,1)=yj
420   do 430 i=1,n
430   pw(noml+i)=pw(noml+i)+1.d0
      evalja=.false.
      conver=.false.
      rc=1.d0
      call banmat(n,ml,mu,1,0,pw,nydim,fsave,n,det,iwork)
      goto 520
440   if(miter.ne.0) go to (460,460,490,520,520),miter
      d=0.0d0
      do 450 i=1,n
      r=h*fsave(i+nydim)-y(i,2)
      d=d+((r-error(i))/ymax(i))**2
      fsave(i)=y(i,1)+el(1)*r
450   error(i)=r
      goto 550
460   do 470 i=1,n
470   fsave(i+nydim)=fsave(i+nydim)*h-y(i,2)-error(i)
      call matsol(n,pw,nydim,iwork,fsave(n1),fsave)
      d=0.0d0
      do 480 i=1,n
      error(i)=error(i)+fsave(i)
      d=d+(fsave(i)/ymax(i))**2
480   fsave(i)=y(i,1)+el(1)*error(i)
      goto 550
490   do 500 i=1,n
500   fsave(i)=pw(i)*(fsave(i+nydim)*h-y(i,2)-error(i))
      d=0.0d0
      do 510 i=1,n
      error(i)=error(i)+fsave(i)
      d=d+(fsave(i)/ymax(i))**2
510   fsave(i)=y(i,1)+el(1)*error(i)
      goto 550
520   do 530 i=1,n
530   fsave(i)=fsave(i+nydim)*h-y(i,2)-error(i)
      call banmat(n,ml,mu,0,1,pw,nydim,fsave,n,det,iwork)
      d=0.0d0
      do 540 i=1,n
      error(i)=error(i)+fsave(i)
      d=d+(fsave(i)/ymax(i))**2
540   fsave(i)=y(i,1)+el(1)*error(i)
550   if (iter.ne.0) trend=dmax1(0.9d0*trend,d/d1)
      if((d*dmin1(1.d0,2.d0*trend)).le.bnd) goto 590
      d1=d
      iter=iter+1
      if(iter.eq.maxite)goto 560
      call diffun(n,t,fsave,fsave(n1))
      nfun=nfun+1
      goto 440
560   if(.not.conver)goto 570
      evalja=.true.
      goto 220
570   t=told
      rmax=rmxfai
      do 580 j1=1,nq
      do 580 j2=j1,nq
      j=nq-j2+j1
      do 580 i=1,n
580   y(i,j)=y(i,j)-y(i,j+1)
      if(dabs(h).le.(hmin*1.00001d0)) goto 820
      rh=rhcorr
      call rescal(y,nydim,rh,rmax,rc,l,idoub)
      goto 200
590   d=0.0d0
      do 600 i=1,n
600   d=d+(error(i)/ymax(i))**2
      if(miter.ne.0) conver=.true.
      nstep=nstep+1
      if(d.le.e) goto 670
      kflag=kflag-1
      t=told
      do 610 j1=1,nq
      do 610 j2=j1,nq
      j=nq-j2+j1
      do 610 i=1,n
610   y(i,j)=y(i,j)-y(i,j+1)
      rmax=rmxfai
      if(dabs(h).le.(hmin*1.00001d0)) goto 810
      if(kflag.le.-maxfai)goto 650
      pr2=1.d0/(((d/e)**(.5d0/dble(l)))*bias2+dfltzr)
      if(nq.eq.1) goto 630
      sum=0.0d0
      do 620 i=1,n
620   sum=sum+(y(i,l)/ymax(i))**2
      pr1=1.d0/(((sum/edn)**(.5d0/dble(nq)))*bias1+dfltzr)
      if(pr1.gt.pr2)goto 640
630   rh=pr2
      call rescal(y,nydim,rh,rmax,rc,l,idoub)
      goto 200
640   l=nq
      nq=nq-1
      rh=pr1
      call coset(meth,nq,el,tq,maxder)
      lmax=maxder+1
      rc=rc*el(1)/oldl0
      oldl0=el(1)
      edn=(tq(1)*eps)**2
      e=(tq(2)*eps)**2
      eup=(tq(3)*eps)**2
      bnd=(tq(4)*eps)**2
      call rescal(y,nydim,rh,rmax,rc,l,idoub)
      goto 200
650   rh=rherr3
      rh=dmax1(hmin/dabs(h),rh)
      h=h*rh
      call diffun(n,t,y,fsave)
      nfun=nfun+1
      do 660 i=1,n
660   y(i,2)=h*fsave(i)
      if(miter.ne.0)evalja=.true.
      idoub=idelay
      if(nq.eq.1)goto 200
      nq=1
      l=2
      call coset(meth,nq,el,tq,maxder)
      lmax=maxder+1
      oldl0=el(1)
      edn=(tq(1)*eps)**2
      e=(tq(2)*eps)**2
      eup=(tq(3)*eps)**2
      bnd=(tq(4)*eps)**2
      goto 200
670   hused=h
      nqused=nq
      kflag=0
      do 680 j=1,l
      do 680 i=1,n
680   y(i,j)=y(i,j)+el(j)*error(i)
      if(idoub.eq.1)goto 700
      idoub=idoub-1
      if((idoub.gt.1).or.(nq.eq.maxder))goto 840
      do 690 i=1,n
690   y(i,lmax)=error(i)
      goto 840
700   pr3=dfltzr
      if(nq.eq.maxder)goto 720
      sum=0.0d0
      do 710 i=1,n
710   sum=sum+((error(i)-y(i,lmax))/ymax(i))**2
      pr3=1.d0/(((sum/eup)**(.5d0/dble(l+1)))*bias3+dfltzr)
720   pr2=1.d0/(((d/e)**(.5d0/dble(l)))*bias2+dfltzr)
      pr1=dfltzr
      if(nq.eq.1)goto 740
      sum=0.0d0
      do 730 i=1,n
730   sum=sum+(y(i,l)/ymax(i))**2
      pr1=1.d0/(((sum/edn)**(.5d0/dble(nq)))*bias1+dfltzr)
740   if((pr3.gt.pr1).and.(pr3.gt.pr2)) goto 760
      if(pr1.gt.pr2)goto 750
      rh=pr2
      if(rh-thrshl) 800,790,790
750   newq=nq-1
      rh=pr1
      if(rh-thrshl) 800,780,780
760   newq=l
      rh=pr3
      if(rh.lt.thrshl)goto 800
      do 770 i=1,n
770   y(i,newq+1)=error(i)*el(l)/dble(l)
780   nq=newq
      l=nq+1
      call coset(meth,nq,el,tq,maxder)
      lmax=maxder+1
      rc=rc*el(1)/oldl0
      oldl0=el(1)
      edn=(tq(1)*eps)**2
      e=(tq(2)*eps)**2
      eup=(tq(3)*eps)**2
      bnd=(tq(4)*eps)**2
790   call rescal(y,nydim,rh,rmax,rc,l,idoub)
      goto 830
800   idoub=idelay
      goto 840
810   kflag=-1
      goto 840
820   kflag=-2
      goto 840
830   rmax=rmxnor
840   hold=h
      jstart=nq
      return
      end

      subroutine matdec(n,a,ia,irow)
      implicit real*8 (a-h,o-z)
      dimension a(ia,1),irow(1)
      if (n.gt.1) goto 1
      a(1,1)=1.d0/a(1,1)
      irow(1)=1
      return
1     do 2 i=1,n
2     irow(i)=i
      n11=n-1
      do 7 i=1,n11
      big=0.0d0
      do 3 j=i,n
      jr=irow(j)
      size=dabs(a(jr,i))
      if (size.le.big) goto 3
      big=size
      ipiv=j
3     continue
      if (ipiv.eq.i) goto 4
      l=irow(i)
      irow(i)=irow(ipiv)
      irow(ipiv)=l
4     ir=irow(i)
      pivot=1.d0/a(ir,i)
      a(ir,i)=pivot
      i11=i+1
      do 6 j=i11,n
      jr=irow(j)
      fact=pivot*a(jr,i)
      a(jr,i)=fact
      do 5 k=i11,n
5     a(jr,k)=a(jr,k)-fact*a(ir,k)
6     continue
7     continue
      nr=irow(n)
      a(nr,n)=1.d0/a(nr,n)
      return
      end

      subroutine rescal (y,nydim,rh,rmax,rc,l,idoub)
      implicit real*8 (a-h,o-z)
      dimension y(nydim,1)
      common/stcom1/ t,h,hmin,hmax,eps,n,mf,kflag,jstart,maxder
      rh=dmax1(rh,hmin/dabs(h))
      rh=dmin1(rh,hmax/dabs(h),rmax)
      r1=1.d0
      do 180 j=2,l
      r1=r1*rh
      do 180 i=1,n
180   y(i,j)=y(i,j)*r1
      h=h*rh
      rc=rc*rh
      idoub=l+1
      return
      end

      subroutine matsol(n,a,ia,irow,y,x)
      implicit real*8 (a-h,o-z)
      dimension a(ia,1),irow(1),y(1),x(1)
      if (n.gt.1) goto 8
      x(1)=y(1)*a(1,1)
      return
8     ir=irow(1)
      x(1)=y(ir)
      do 10 i=2,n
      ir=irow(i)
      s=y(ir)
      i10=i-1
      do 9 k=1,i10
9     s=s-a(ir,k)*x(k)
10    x(i)=s
      x(n)=x(n)*a(ir,n)
      do 12 i=2,n
      j=n-i+1
      jr=irow(j)
      s=x(j)
      j11=j+1
      do 11 k=j11,n
11    s=s-a(jr,k)*x(k)
12    x(j)=s*a(jr,j)
      return
      end
      subroutine predic(nydim,y,nq)
      implicit real*8 (a-h,o-z)
      dimension y(nydim,1)
      common/stcom1/   t,h,hmin,hmax,eps,n,mf,kflag,jstart,maxder
      do 210 j1=1,nq
      do 210 j2=j1,nq
      j=nq-j2+j1
      do 210 i=1,n
210   y(i,j)=y(i,j)+y(i,j+1)
      return
      end
      subroutine banmat(in,l1,l2,nt,im,a,ia,y,iy,det,int)
      implicit real*8 (a-h,o-z)
      dimension a(ia,ia),y(iy,iy),int(in)
      data xnd/1777.0d+16/
      n=in
      l=im
      ml=l1
      mu=l2
      ll=ml+mu+1
      if (n.le.1.or.ia.lt.n.or.l1.le.0.or.l2.le.0) goto 23
      lu=ll+1
      lp=ll+ml
      n1=n-1
      if (nt.ne.1) goto 9
      sn=1.0d0
      do 3 i=1,ml
      ii=mu+i
      k=ml+1-i
      do 1 j=1,ii
1     a(i,j)=a(i,j+k)
      k=ii+1
      do 2 j=k,ll
2     a(i,j)=0.0d0
3     continue
      do 8 nr=1,n1
      np=nr+1
      lr=nr+ml
      if (lr.gt.n) lr=n
      lc=nr+ll-1
      if (lc.gt.n) lc=n
      kk=lc-nr
      mx=nr
      xm=dabs(a(nr,1))
      do 4 i=np,lr
      if (dabs(a(i,1)).le.xm) goto 4
      mx=i
      xm=dabs(a(i,1))
4     continue
      int(nr)=mx
      if (mx.eq.nr) goto 6
      do 5 i=1,ll
      xx=a(nr,i)
      a(nr,i)=a(mx,i)
5     a(mx,i)=xx
      sn=-sn
6     xm=a(nr,1)
      if (xm.eq.0.0d0) goto 18
      xm=-1.d0/xm
      do 7 i=np,lr
      j=ll+i-nr
      xx=a(i,1)*xm
      a(nr,j)=xx
      call addvec(kk,xx,a(nr,2),ia,a(i,2),ia,a(i,1),ia)
7     a(i,ll)=0.0d0
8     continue
      if (a(n,1).eq.0.0d0) sn=0.0d0
9     det=sn
      if (im.eq.0) return
      do 13 nr=1,n1
      np=nr+1
      lr=nr+ml
      if(lr.gt.n) lr=n
      kk=lr-nr
      if(a(nr,1).eq.0.0d0) goto 18
      if(int(nr).eq.nr) goto 11
      j=int(nr)
      do 10 i=1,l
      xx=y(nr,i)
      y(nr,i)=y(j,i)
10    y(j,i)=xx
11    do 12 i=1,l
12    call addvec(kk,y(nr,i),a(nr,lu),ia,y(np,i),1,y(np,i),1)
13    continue
      xm=a(n,1)
      nr=n
      if(xm.eq.0.0d0) goto 18
      xm=1.d0/xm
      do 14 i=1,l
14    y(n,i)=y(n,i)*xm
      ns=1
15    do 17 nb=ns,n1
      nr=n-nb
      np=nr+1
      lc=nr+ll-1
      if (lc.gt.n) lc=n
      kk=lc-nr
      do 16 i=1,l
16    y(nr,i)=(y(nr,i)-dotpro(kk,a(nr,2),ia,y(np,i),1))/a(nr,1)
17    continue
      return
18    j=nr-1
      det=0.0d0
      do 22 i=1,l
      xx=0.0d0
      if(j.eq.0) goto 20
      do 19 k=1,j
19    if (dabs(y(k,i)).gt.xx)  xx=dabs(y(k,i))
20    xx=xx*1.d-12
      do 21 k=nr,n
      if (dabs(y(k,i)).gt.xx) goto 23
21    y(k,i)=0.0d0
22    y(nr,i)=1.0d0
      ns=n-nr+1
      goto 15
23    det=xnd
      return
      end
      subroutine addvec(kk,xx,a,ia,b,ib,c,ic)
      implicit real*8 (a-h,o-z)
      dimension a(1),b(1),c(1)
      x=xx
      k=kk
      ja=1
      jb=1
      jc=1
      do 1 i=1,k
      c(jc)=b(jb)+x*a(ja)
      ja=ja+ia
      jb=jb+ib
1     jc=jc+ic
      return
      end
      double precision function dotpro(kk,a,ia,b,ib)
      implicit real*8 (a-h,o-z)
      dimension a(1),b(1)
      k=kk
      s=0.0d0
      ja=1
      jb=1
      do 1 i=1,k
      s=s+a(ja)*b(jb)
      ja=ja+ia
1     jb=jb+ib
      dotpro=s
      return
      end

      subroutine coset(meth,nq,el,tq,maxder)
      implicit real*8(a-h,o-z)
      dimension pertst(12,2,3),el(13),tq(4)
      data pertst/1.,1.,2.,1.,.3158,.07407,.01391,.002182,
     1    .0002945,.00003492,.000003692,.0000003524,
     2    1.,1.,.5,.16666667,.041666667,1.,1.,1.,1.,1.,1.,1.,
     3    2.,12.,24.,37.89,53.33,70.08,87.97,106.9,
     4    126.7,147.4,168.8,191.0,
     5    2.,4.5,7.3333333,10.416667,13.7,1.,1.,1.,1.,1.,1.,1.,
     6    12.0,24.0,37.89,53.33,70.08,87.97,106.9,
     7    126.7,147.4,168.8,194.0,1.,
     8    3.,6.,9.1666667,12.5,1.,1.,1.,1.,1.,1.,1.,1./
      el(2)=1.0d0
      go to (1,2),meth
1     maxder=min0(maxder,12)
      go to (101,102,103,104,105,106,107,108,109,110,111,112),nq
2     maxder=min0(maxder,5)
      go to (201,202,203,204,205),nq
101   el(1)=1.0d0
      go to 900
102   el(1)=0.5d0
      el(3)=0.5d0
      go to 900
103   el(1)=0.4166666666666667d0
      el(3)=0.75d0
      el(4)=0.1666666666666667d0
      go to 900
104   el(1)=0.375d0
      el(3)=0.9166666666666667d0
      el(4)=0.3333333333333333d0
      el(5)=0.4166666666666667d-01
      go to 900
105   el(1)=0.3486111111111111d0
      el(3)=0.1041666666666667d 01
      el(4)=0.4861111111111111d0
      el(5)=0.1041666666666667d0
      el(6)=0.8333333333333333d-02
      go to 900
106   el(1)=0.3298611111111111d0
      el(3)=0.1141666666666667d 01
      el(4)=0.625d0
      el(5)=0.1770833333333333d0
      el(6)=0.025d0
      el(7)=0.1388888888888889d-02
      go to 900
107   el(1)=0.3155919312169312d0
      el(3)=0.1225d 01
      el(4)=0.7518518518518519d0
      el(5)=0.2552083333333333d0
      el(6)=0.4861111111111111d-01
      el(7)=0.4861111111111111d-02
      el(8)=0.1984126984126984d-03
      go to 900
108   el(1)=0.3042245370370370d0
      el(3)=0.1296428571428571d 01
      el(4)=0.8685185185185185d0
      el(5)=0.3357638888888889d0
      el(6)=0.7777777777777778d-01
      el(7)=0.1064814814814815d-01
      el(8)=0.7936507936507937d-03
      el(9)=0.2480158730158730d-04
      go to 900
109   el(1)=0.2948680004409171d0
      el(3)=0.1358928571428571d 01
      el(4)=0.9765542328042328d0
      el(5)=0.4171875d0
      el(6)=0.1113541666666667d0
      el(7)=0.01875d0
      el(8)=0.1934523809523810d-02
      el(9)=0.1116071428571429d-03
      el(10)=0.2755731922398589d-05
      go to 900
110   el(1)=0.2869754464285714d0
      el(3)=0.1414484126984127d 01
      el(4)=0.1077215608465608d 01
      el(5)=0.4985670194003527d0
      el(6)=0.1484375d0
      el(7)=0.2906057098765432d-01
      el(8)=0.3720238095238095d-02
      el(9)=0.2996858465608466d-03
      el(10)=0.1377865961199295d-04
      el(11)=0.2755731922398589d-06
      go to 900
111   el(1)=0.2801895964439367d0
      el(3)=0.1464484126984127d 01
      el(4)=0.1171514550264550d 01
      el(5)=0.5793581900352734d0
      el(6)=0.1883228615520282d0
      el(7)=0.4143036265432099d-01
      el(8)=0.6211144179894180d-02
      el(9)=0.6252066798941799d-03
      el(10)=0.4041740152851264d-04
      el(11)=0.1515652557319224d-05
      el(12)=0.2505210838544172d-07
      go to 900
112   el(1)=0.2742655400315991d0
      el(3)=0.1509938672438672d 01
      el(4)=0.1260271164021164d 01
      el(5)=0.6592341820987654d0
      el(6)=0.2304580026455026d0
      el(7)=0.5569724610523222d-01
      el(8)=0.9439484126984127d-02
      el(9)=0.1119274966931217d-02
      el(10)=0.9093915343915344d-04
      el(11)=0.4822530864197531d-05
      el(12)=0.1503126503126503d-06
      el(13)=0.2087675698786810d-08
      go to 900
201   el(1)=1.0d0
      go to 900
202   el(1)=0.6666666666666667d0
      el(3)=0.3333333333333333d0
      go to 900
203   el(1)=0.5454545454545455d0
      el(3)=el(1)
      el(4)=0.9090909090909091d-01
      go to 900
204   el(1)=0.48d0
      el(3)=0.7d0
      el(4)=0.2d0
      el(5)=0.2d-01
      go to 900
205   el(1)=0.4379562043795620d0
      el(3)=0.8211678832116788d0
      el(4)=0.3102189781021898d0
      el(5)=0.5474452554744526d-01
      el(6)=0.3649635036496350d-02
900   do 910 k=1,3
910   tq(k)=pertst(nq,meth,k)
      tq(4)=0.5d0/dble(nq+2)
      return
      end
