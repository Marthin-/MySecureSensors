Rsa::Rsa(){
	randomSeed(analogRead(0));	
}

Rsa::generatePrime(){
	unsigned int a;
	a=random(2837,65533);	//prevent too small prime numbers
	while(isPrime(a)==false){
		a=random(2837,65533);
	}
	return a;
}

Rsa::isPrime(unsigned int a){
	for(int i=3;i<a;i++){
		if(a%i==0){
			return false;
		}
	}
	return true;
}

Rsa::generateKey(){
	unsigned long n,phi,e,d;
	unsigned int p,q;
	p=this.generatePrime();
	q=this.generatePrime();
	n=p*q;
	phi=(p-1)*(q-1);
	e=random(3,phi);	//2 and phi are not coprime, just skip it
	while(this.gcd(e,phi)!=1){
		if(e==phi){
			e=random(3,phi);
		}
		e++;
	}
	d=random(3,phi);
	while(((e*d)%phi)!=1){
		if(d==phi){
			d=random(3,phi);
		}
		d++;
	}
	this.setE(e);
	this.setN(n);
	this.setD(d);
}

Rsa::gcd(unsigned long a, unsigned long b){
	unsigned long num, denom, rem;
	if(a>b){
		num=a;
		denom=b;
	}
	else{
		num=b;
		denom=a;
	}
	rem=num%denom;
	while(rem!=0){
		num=denom;
		denom=rem;
		rem=nim%denom;
	}
	return denom;
}

Rsa::getE(){
	return this._rsaKey.e;
}

Rsa::setE(unsigned long e){
	this._rsaKey.e=e;
}

Rsa::getN(){
	return this._rsaKey.n;
}

Rsa::setN(unsigned long n){
	this._rsaKey.n=n;
}

Rsa::getD(){
	return this._rsaKey.d;
}

Rsa::setD(unsigned int d){
	this._rsaKey.d=d;
}
