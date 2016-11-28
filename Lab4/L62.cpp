#include "stdafx.h"
#include "L62.h"


L62::L62()
{
}


L62::~L62()
{
}

void L62::coef(long double *a, long double *d, long double *f) {
	/* Пiдпрограма обчислення коефiцiєнтiв a(i),d(i),f(i)
	однорiдної консервативної рiзницевої схеми */
	for (int i = 1; i <= n; i++) {
		xi = i*h;
		a[i] = 1.0 + pow(xi - 0.5*h, 2);
	}
	for (int i = 0; i <= n; i++) {
		xi = i*h;
		d[i] = xi;
		f[i] = -pow(xi, 2);
	}
}

void L62::progonka(long double *a, long double *b, long double *c, long double *f) {
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
	for (int i = n - 1; i >= 0; i--) {
		f[i] = b[i] * f[i + 1] + c[i];
	}
}

void L62::cds(long double *a, long double *b, long double *c, long double *d, long double *f, long double *y, long double *y2) {
	/* Пiдпрограма розв'язування крайової задачi для одновимiрного
	стацiонарного диференцiального рiвняння теплопровiдностi
	(дифузiї) з допомогою однорiдної консервативної рiзницевої
	схеми:
	a(i+1) * (y(i+1)-y(i))/h - a(i) * (y(i)-y(i-1))/h-
	- d(i) * y(i) = - f(i),  i=1,...,N */
	int js = 0;
	long double em;
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
			c[0] = b[0] + 0.5 * h * d[0] + beta1;
			f[0] = 0.5 * h * f[0] + mu1;
		}

		for (int i = 1; i <= n - 1; i++) {
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
			for (int i = 0; i <= n; i++) {
				y[i] = f[i];
			}
			em = 0.0;

			hn = n / 2;
			for (int i = 0; i <= hn; i++) {
				e = fabs(y[2 * i] - y2[i]);
				if (e > em) em = e;
			}

			if (em > 3.0 * eps) {
				for (int i = 0; i <= n; i++) y2[i] = y[i];
				n = n * 2;
			}
			else kflag = 1;

		}
		else {
			n = n * 2;
			js = 1;
			for (int i = 0; i <= n; i++) y2[i] = f[i];
		}

		if (n > nd) kflag = 2;
	}

}

/* Текст головної програми розв’язування заданої задачі
(див. контрольний приклад) */
void L62::main() {
	ofstream fout("result.txt", ios_base::out | ios_base::trunc);
	/* Задання початкових даних */
	long double *a, *b, *c, *d, *f, *y, *y2;
	a = new long double[nd]; b = new long double[nd]; c = new long double[nd]; d = new long double[nd];
	f = new long double[nd]; y = new long double[nd]; y2 = new long double[nd];
	/* Обчислення чисельного розв'язку задачi y(i) */
	cds(a, b, c, d, f, y, y2);
	cout.precision(11);
	fout.precision(11);
	if (kflag == 1) {
		h = 1.0 / n;
		/* Обчислення точного розв'язку задачi y1(i) у вузлах сiтки
		i похибки emax у сiтковiй нормi C */
		emax = 0.0;
		j = 0;
		int z = n / 2;

		for (int i = 0; i <= z; i++) {
			xi = j * h;
			cout << "xi= " << xi << "\t   |   ";
			fout << "xi= " << xi << "\t   |   ";
			cout << "y[" << j << "]= " << y[j] << endl;
			fout << "y[" << j << "]= " << y[j] << endl;
			j = j + 2;
		}

	}
	else cout << "kflag= " << kflag;

	fout.close();
	delete[] a; delete[] b; delete[] c; delete[] d;
	delete[] f; delete[] y; delete[] y2;
	system("pause");
}
