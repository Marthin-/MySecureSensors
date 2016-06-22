RSA::RSA(){
	randomSeed(analogRead(0));	
}

RSA::generatePrime(){
	unsigned int a;
	a=random(2837,65533);	//prevent too small prime numbers
	while(isPrime(a)==false){
		a=random(2837,65533);
	}
	return a;
}

RSA::isPrime(unsigned int a){
	for(int i=3;i<a;i++){
		if(a%i==0){
			return false;
		}
	}
	return true;
}

RSA::generateKey(){
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
