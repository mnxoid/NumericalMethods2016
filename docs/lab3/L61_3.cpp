#include <math.h>
#include <iostream>
using namespace std;
#define nd 500

int n, halfn, kflag, i, kl, kr;
long double xi, h, eps, emax, e, mu1, mu2, beta1, beta2, df, dk, dq, sqk, del, dk1, dk2, dk3;

void coef(long double *a, long double *d, long double *f) {
	/* Пiдпрограма обчислення коефiцiєнтiв a(i),d(i),f(i)
   однорiдної консервативної рiзницевої схеми */
	for (int i = 1; i <= n; i++) a[i] = 1.25;
	for (int i = 0; i <= n; i++) {
		d[i] = 0.5;
		f[i] = -0.25;
	}
}

void progonka(long double *a, long double *b, long double *c, long double *f) {
	/* Пiдпрограма розв'язування методом прогонки системи
   N+1  лiнiйних алгебраїчних рiвнянь з тридiагональною
   матрицею вигляду 

  -c(0)*y(0)+b(0)*y(1) = - f(0),   i=0,
  a(i)*y(i-1)-c(i)*y(i)+b(i)*y(i+1) = - f(i), i=1,2,...,N-1,
  a(N)*y(N-1)-c(N)*y(N) = - f(N),  i=N. */
	long double q;
	b[0] = b[0] / c[0];
	c[0] = f[0] / c[0];
	for (int i = 1; i <= n - 1; i++) {
		q = c[i] - a[i] * b[i - 1];
		b[i] = b[i] / q;
		c[i] = (f[i] + a[i] * c[i - 1]) / q;
	}
	f[n] = (f[n] + a[n] * c[n - 1]) / (c[n] - a[n] * b[n - 1]);
	for (int i = n - 1; i >= 0; i--) f[i] = b[i] * f[i + 1] + c[i];
}

void cds(long double *a, long double *b, long double *c, long double *d, long double *f, long double *y, long double *y2) {
	/* Пiдпрограма розв'язування крайової задачi для одновимiрного
  стацiонарного диференцiального рiвняння теплопровiдностi
  (дифузiї) з допомогою однорiдної консервативної рiзницевої
  схеми:
  a(i+1) * (y(i+1)-y(i))/h - a(i) * (y(i)-y(i-1))/h-
    - d(i) * y(i) = - f(i),  i=1,...,N
  У випадку крайових умов 1-го роду  y(0) = mu1,  y(N) = mu2
  Для граничних умов 3-го роду рiзницевi умови мають вигляд:
  a(1)*(y(1)-y(0))/h = (beta1+0.5*h*d(0))-(mu1+0.5*h*f(0)),
    -a(N)*(y(N)-y(N-1))/h = (beta2+0.5*h*d(N))-(mu2+0.5*h*f(N))
  Можливi рiзнi комбiнацiї граничних умов 1-го i 3-го роду
  Для практичної оцiнки похибки використовується правило Рунге
  Вхiднi параметри:
  N - початкова кiлькiсть частин розбиття вiдрiзка [0,1]
      точками сiтки
  eps - потрiбна точнiсть
  kl - вказiвник типу граничних умов на лiвому кiнцi вiдрiзка
  kl=0 - умова 1-го роду, kl=1 - умова 3-го роду
  kr - вказiвник типу граничних умов на правому кiнцi вiдрiзка
  kr=0 - умова 1-го роду, kr=1 - умова 3-го роду
  mu1,mu2,beta1,beta2- параметри, якi визначають крайовi умови
  nd - цiле число, яке використовується для опису масивiв
  Вихiднi параметри:
  N - кiлькiсть частин розбиття iнтервалу, при якiй
      задовiльняеться задана точнiсть
  y(i), i=0,1,...N - розв'язок рiзницевої задачi
  kflag - вказiвник вихiдних результатiв
       kflag=1 - одержано розв'язок з потрiбною точнiстю
       kflag=2 - результат з потрiбною точнiстю при n.le.nd
                 не одержано; величина nd задана не
                 достатньо великою або eps занадто мале    
  Пiдпрограма cds використовує наступнi пiдпрограми:
  coef, яка здiйснює обчислення коефiцiентiв рiзницевоi схеми, 
  progonka, яка розв'язує методом прогонки триточкове рiзницеве рiвняння */
	int js;
	long double em;
	js = 0;
	kflag = -1;
	while (kflag <= 0) {
		h = 1.0 / n;
		coef(a, d, f);
		if (kl == 0) {
			b[0] = 0.0;
			c[0] = 1.0;
			f[0] = mu1;
		}
		else {
			b[0] = a[1] / h;
			c[0] = b[0] + 0.5*h*d[0] + beta1;
			f[0] = 0.5*h*f[0] + mu1;
		}
		for (int i = 1; i <= n - 1; i++){
			a[i] = a[i] / pow(h, 2);
			b[i] = a[i + 1] / pow(h, 2);
			c[i] = a[i] + b[i] + d[i];
		}
		if (kr == 0) {
			a[n] = 0.0;
			c[n] = 1.0;
			f[n] = mu2;
		}
		else {
			a[n] = a[n] / h;
			c[n] = a[n] + 0.5*h*d[n] + beta2;
			f[n] = 0.5*h*f[n] + mu2;
		}
		progonka(a, b, c, f);
		if (js == 1) {
			for (int i = 0; i <= n; i++) y[i] = f[i];
			em = 0.0;
			for (int i = 0; i <= halfn; i++) {
				e = fabs(y[2 * i] - y2[i]);
				if (e > em) em = e;
			}
			if (em > 3.0 * eps) {
				for (int i = 0; i <= n; i++) y2[i] = y[i];
				n = n * 2;
				halfn = halfn * 2;
			}
			else kflag = 1;
		}
		else {
			n = n * 2;
			halfn = halfn * 2;
			js = 1;
			for (int i = 0; i <= n; i++) y2[i] = f[i];
		}
		if (n > nd) kflag = 2;
	}
}

/* Текст головної програми розв’язування заданої задачі
(див. контрольний приклад) */
void main() {
	long double *a, *b, *c, *d, *f, *y, *y2;
	a = new long double[nd]; b = new long double[nd]; c = new long double[nd]; d = new long double[nd];
	f = new long double[nd]; y = new long double[nd]; y2 = new long double[nd];
	/* Задання початкових даних */  
	n = 10;
	halfn = n / 2;
	eps = 0.001;
	kl = 1; kr = 1;
	mu1 = -1.0; mu2 = 0.0; beta1 = 1.0; beta2 = 1.0;
	/* Обчислення чисельного розв'язку задачi y(i) */
	cds(a, b, c, d, f, y, y2);
	if (kflag == 1) {
		h = 1.0 / n;
		emax = 0.0;
		/* Обчислення точного розв'язку задачi y1(i) у вузлах сiтки
     i похибки emax у сiтковiй нормi C */
		dk = 1.0+0.5*0.5; dq = 0.5; df = -0.5*0.5; sqk = sqrt(dk*dq);
		dk1 = sqk - 1.0; dk2 = sqk + 1.0; dk3 = sqrt(dq/dk);
		del = pow(dk1, 2)*exp(-dk3) - pow(dk2, 2)*exp(dk3);
		for (int i = 0; i <= n; i++) {
			xi = i*h;
			y2[i]=(((df/dq+1.0)*dk1*exp(-dk3)+df/dq*dk2)*exp(xi*dk3)+((df/dq+1.0)*dk2*exp(dk3)+df/dq*dk1)*exp(-xi*dk3))/del+df/dq;
			e = abs(y[i] - y2[i]);
			if (e > emax) emax = e;
		}
		cout.precision(10);
		cout << "emax= " << emax << endl;
	}
	else cout << "kflag= " << kflag << endl;
	system("pause");
}