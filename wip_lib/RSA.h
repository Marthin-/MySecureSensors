#ifndef RSA_h
#define RSA_h

struct RSAKey{
	unsigned long e;
	unsigned long n;
	unsigned long d;
};

class RSA {
	public:
	RSA RSA();
	long int generatePrime();
	boolean isPrime(long int a);
	void generateKey(RSAKey key);
	unsigned long gcd(unsigned long a, unsigned long b);
	unsigned long getE();
	void setE(unsigned long e);
	unsigned long getN();
	void setN(unsigned long n);
	unsigned long getD();
	void setD(unsigned long d);
	private:
	RSAKey _rsaKey;
}
#endif
